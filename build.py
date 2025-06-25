#!/usr/bin/env python

import glob
import platform
import sys
import zipfile
from argparse import ArgumentParser, Namespace
from collections.abc import Callable
from functools import partial
from os import makedirs, path, remove
from shutil import copyfile, rmtree, which
from subprocess import run

MEDIAPIPE_DIR = path.join(path.dirname(__file__), "mediapipe")
ANDROID_PROJECT = path.join(path.dirname(__file__), "GDMP", "android", "aar")

TARGETS = {
    "android": "@GDMP//GDMP/android:GDMP",
    "desktop": "@GDMP//GDMP/desktop:GDMP",
    "ios": "@GDMP//GDMP/ios:GDMP",
}

TARGET_ARGS = {
    "android": [
        "--extra_toolchains=@androidndk//:all",
        "--config=android",
        "--copt=-fPIC",
        "--define=OPENCV=source",
    ],
    "desktop": {
        "linux": [
            "--copt=-DMESA_EGL_NO_X11_HEADERS",
            "--copt=-DEGL_NO_X11",
            "--copt=-fPIC",
            "--define=OPENCV=source",
        ],
        "darwin": [
            "--define=OPENCV=source",
            "--define=MEDIAPIPE_DISABLE_GPU=1",
        ],
        "win32": [
            "--conlyopt=/std:c11",
            "--conlyopt=/experimental:c11atomics",
            "--define=OPENCV=source",
            "--define=MEDIAPIPE_DISABLE_GPU=1",
        ],
    },
    "ios": [
        "--apple_generate_dsym=false",
        "--config=ios",
    ],
}


def bazel_build(args: list[str]):
    bazel_exec = which("bazelisk") or which("bazel")
    if bazel_exec is None:
        print(
            "Error: Cannot find bazel, please check bazel is installed and is in PATH."
        )
        sys.exit(-1)
    cmd = [bazel_exec, "build"]
    cmd.extend(args)
    run(cmd, cwd=MEDIAPIPE_DIR, check=True)


def build_android(args: Namespace) -> list[str]:
    target: str = args.target
    arch: str = args.arch
    build_type: str = args.type
    build_aar: bool = args.android_aar
    if build_aar:
        if not path.exists(ANDROID_PROJECT):
            print("Error: android project does not exist.")
            sys.exit(-1)
        gradlew_exec = path.join(ANDROID_PROJECT, "gradlew")
        return [gradlew_exec, "clean", f"assemble{build_type.capitalize()}"]
    if arch.startswith("arm64"):
        arch = "arm64-v8a"
    build_args = TARGET_ARGS[target]
    build_args.append(f"--cpu={arch}")
    return build_args


def build_desktop(args: Namespace) -> list[str]:
    target: str = args.target
    arch: str = args.arch
    build_args = TARGET_ARGS[target][sys.platform]
    if sys.platform == "darwin":
        if arch == "arm64":
            build_args.append("--cpu=darwin_arm64")
        elif arch == "x86_64":
            build_args.append("--cpu=darwin_x86_64")
            build_args.append("--define=xnn_enable_avxvnniint8=false")
    elif sys.platform == "linux":
        if arch == "arm64":
            build_args.append("--copt=-fpermissive")
    return build_args


def build_ios(args: Namespace) -> list[str]:
    target: str = args.target
    arch: str = args.arch
    build_args = TARGET_ARGS[target]
    if arch:
        build_args.append(f"--ios_multi_cpus={arch}")
    return build_args


def build(args: Namespace):
    target: str = args.target
    build_type: str = args.type
    if build_type == "release":
        mode = "opt"
    else:
        mode = "dbg"
    build_args = ["-c", mode]
    if target == "android":
        build_aar: bool = args.android_aar
        cmds = build_android(args)
        if build_aar:
            run(cmds, cwd=ANDROID_PROJECT, check=True)
            return
        else:
            build_args.extend(cmds)
    elif target == "desktop":
        build_args.extend(build_desktop(args))
    elif target == "ios":
        build_args.extend(build_ios(args))
    else:
        build_args.extend(TARGET_ARGS[target])
    build_args.append(TARGETS[target])
    bazel_build(build_args)


def copy_android(args: Namespace):
    build_type: str = args.type
    build_aar: bool = args.android_aar
    output: str = args.output
    arch: str = args.arch
    if build_aar:
        src = path.join(ANDROID_PROJECT, f"build/outputs/aar/GDMP-{build_type}.aar")
        gdap = path.join(ANDROID_PROJECT, "GDMP.gdap")
        dst = path.join(output, "GDMP.android.aar")
        copyfile(src, dst)
        copyfile(gdap, path.join(output, path.basename(gdap)))
        return
    src = path.join(MEDIAPIPE_DIR, "bazel-bin/external/GDMP/GDMP/android/libGDMP.so")
    filename = path.basename(src).split(".")
    filename = ".".join([filename[0], "android", filename[-1]])
    dst = path.join(output, filename)
    copyfile(src, dst)
    if arch.startswith("arm64"):
        arch = "arm64-v8a"
    opencv_lib = []
    opencv_lib.extend(glob.glob(
        path.join(
            MEDIAPIPE_DIR,
            "bazel-mediapipe/external/android_opencv/sdk/native/libs",
            arch,
            "libopencv_java4.so",
        )
    ))
    for lib in opencv_lib:
        copyfile(lib, path.join(output, path.basename(lib)))


def copy_desktop(args: Namespace):
    output: str = args.output
    desktop_platform = platform.system().lower()
    if desktop_platform == "darwin":
        desktop_platform = "macos"
    desktop_output = path.join(MEDIAPIPE_DIR, "bazel-bin/external/GDMP/GDMP/desktop")
    if desktop_platform == "linux":
        src = path.join(desktop_output, "libGDMP.so")
    elif desktop_platform == "macos":
        src = path.join(desktop_output, "libGDMP.dylib")
    elif desktop_platform == "windows":
        src = path.join(desktop_output, "GDMP.dll")
    filename = path.basename(src).split(".")
    filename = ".".join([filename[0], desktop_platform, filename[-1]])
    dst = path.join(output, filename)
    if path.exists(dst):
        remove(dst)
    copyfile(src, dst)
    opencv_lib = []
    if desktop_platform == "linux":
        opencv_lib = glob.glob(
            path.join(
                MEDIAPIPE_DIR,
                "bazel-bin/third_party/opencv_cmake/lib",
                "libopencv_*.so.*",
            )
        )
    elif desktop_platform == "macos":
        opencv_lib = glob.glob(
            path.join(
                MEDIAPIPE_DIR,
                "bazel-bin/third_party/opencv_cmake/lib",
                "libopencv_*.dylib",
            )
        )
    elif desktop_platform == "windows":
        opencv_lib = glob.glob(
            path.join(
                MEDIAPIPE_DIR,
                "bazel-bin/third_party/opencv_cmake/bin",
                "opencv_*.dll",
            )
        )
    for lib in opencv_lib:
        copyfile(lib, path.join(output, path.basename(lib)))


def copy_ios(args: Namespace):
    output: str = args.output
    src = path.join(MEDIAPIPE_DIR, "bazel-bin/external/GDMP/GDMP/ios/GDMP.zip")
    with zipfile.ZipFile(src) as f:
        f.extractall(output)


def copy_output(args: Namespace):
    target: str = args.target
    output: str = args.output
    if output is None:
        return
    makedirs(output, exist_ok=True)
    copy_actions = {
        "android": copy_android,
        "desktop": copy_desktop,
        "ios": copy_ios,
    }
    copy_actions[target](args)


if __name__ == "__main__":
    parser = ArgumentParser()
    parser.add_argument("target", choices=list(TARGETS), help="build target")
    parser.add_argument(
        "--type", choices=["debug", "release"], default="release", help="build type"
    )
    parser.add_argument("--arch", default="", help="library architecture")
    parser.add_argument("--output", help="build output directory")
    parser.add_argument(
        "--android-aar", help="build android aar plugin", action="store_true"
    )
    args = parser.parse_args()
    if not args.arch:
        machine = platform.machine().lower()
        if machine in ["amd64", "x86_64"]:
            args.arch = "x86_64"
        elif machine in ["aarch64", "arm64"]:
            args.arch = "arm64"
    build(args)
    copy_output(args)
