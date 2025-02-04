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
ANDROID_PROJECT = path.join(path.dirname(__file__), "android")

TARGETS = {
    "android": "@GDMP//GDMP/android:GDMP",
    "desktop": "@GDMP//GDMP/desktop:GDMP",
    "ios": "@GDMP//GDMP/ios:GDMP",
}

TARGET_ARGS = {
    "android": [
        "--config=android",
        "--copt=-fPIC",
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
            "--define=MEDIAPIPE_DISABLE_GPU=1",
        ],
    },
    "ios": [
        "--apple_generate_dsym=false",
        "--config=ios",
    ],
}


def bazel_build(args: list[str]) -> Callable:
    bazel_exec = which("bazelisk") or which("bazel")
    if bazel_exec is None:
        print(
            "Error: Cannot find bazel, please check bazel is installed and is in PATH."
        )
        sys.exit(-1)
    cmd = [bazel_exec, "build"]
    cmd.extend(args)
    return partial(run, cmd, cwd=MEDIAPIPE_DIR, check=True)


def build_android(args: Namespace, build_args: list[str]) -> list[Callable]:
    build_type: str = args.type
    arch: str = args.arch
    skip_aar: bool = args.android_skip_aar
    output: str = args.output
    cmds = []
    if not path.exists(ANDROID_PROJECT):
        print("Error: android project does not exist.")
        sys.exit(-1)
    src = path.join(MEDIAPIPE_DIR, "bazel-bin/external/GDMP/GDMP/android/libGDMP.so")
    if arch:
        if skip_aar:
            dst_dir = output
        else:
            dst_dir = path.join(ANDROID_PROJECT, "src/main/jniLibs")
        if not dst_dir is None and path.exists(dst_dir):
            cmds.append(partial(rmtree, dst_dir))
        abi_list = arch.split(",")
        for abi in abi_list:
            arg = [arg.format(abi=abi) for arg in build_args]
            cmds.append(bazel_build(arg))
            if dst_dir is None:
                continue
            src_opencv = path.join(
                MEDIAPIPE_DIR,
                "bazel-mediapipe/external/android_opencv/sdk/native/libs",
                abi,
                "libopencv_java4.so",
            )
            dst_jni = path.join(dst_dir, abi)
            dst = path.join(dst_jni, path.basename(src))
            dst_opencv = path.join(dst_jni, path.basename(src_opencv))
            cmds.append(partial(makedirs, dst_jni, exist_ok=True))
            cmds.append(partial(copyfile, src, dst))
            cmds.append(partial(copyfile, src_opencv, dst_opencv))
    if skip_aar:
        return cmds
    gradlew_exec = path.join(ANDROID_PROJECT, "gradlew")
    gradle_build = [gradlew_exec, "clean", f"assemble{build_type.capitalize()}"]
    cmds.append(partial(run, gradle_build, cwd=ANDROID_PROJECT, check=True))
    return cmds


def get_build_cmds(args: Namespace) -> list[Callable]:
    target: str = args.target
    build_type: str = args.type
    arch: str = args.arch
    cmds = []
    if build_type == "release":
        mode = "opt"
    else:
        mode = "dbg"
    build_args = ["-c", mode]
    if target == "desktop":
        build_args.extend(TARGET_ARGS[target][sys.platform])
        if sys.platform == "darwin":
            if arch == "arm64":
                build_args.append("--cpu=darwin_arm64")
            elif arch == "x86_64":
                build_args.append("--cpu=darwin_x86_64")
                build_args.append("--define=xnn_enable_avxvnniint8=false")
    else:
        build_args.extend(TARGET_ARGS[target])
    if target == "android":
        build_args.extend(["--cpu={abi}", TARGETS[target]])
        cmds.extend(build_android(args, build_args))
        return cmds
    elif target == "ios" and arch:
        build_args.append(f"--ios_multi_cpus={arch}")
    build_args.append(TARGETS[target])
    cmd = bazel_build(build_args)
    cmds.append(cmd)
    return cmds


def copy_android(args: Namespace):
    build_type: str = args.type
    skip_aar: bool = args.android_skip_aar
    output: str = args.output
    if skip_aar:
        return
    src = path.join(ANDROID_PROJECT, f"build/outputs/aar/GDMP-{build_type}.aar")
    dst = path.join(output, "GDMP.android.aar")
    copyfile(src, dst)


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
    if desktop_platform == "linux":
        opencv_lib = glob.glob(
            path.join(
                MEDIAPIPE_DIR,
                "bazel-bin/third_party/opencv_cmake/lib",
                "libopencv_*.so.*",
            )
        )
        if len(opencv_lib) == 0:
            return
        for lib in opencv_lib:
            copyfile(lib, path.join(output, path.basename(lib)))
    elif desktop_platform == "macos":
        opencv_lib = glob.glob(
            path.join(
                MEDIAPIPE_DIR,
                "bazel-bin/third_party/opencv_cmake/lib",
                "libopencv_*.dylib",
            )
        )
        if len(opencv_lib) == 0:
            return
        for lib in opencv_lib:
            copyfile(lib, path.join(output, path.basename(lib)))
    elif desktop_platform == "windows":
        opencv_lib = glob.glob(path.join(desktop_output, "opencv_world*.dll"))
        if len(opencv_lib) == 0:
            return
        src = opencv_lib[0]
        copyfile(src, path.join(output, path.basename(src)))


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
        "--android-skip-aar", help="skip building aar for android", action="store_true"
    )
    args = parser.parse_args()
    cmds = get_build_cmds(args)
    for cmd in cmds:
        cmd()
    copy_output(args)
