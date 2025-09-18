#!/usr/bin/env python

import glob
import platform
import sys
import zipfile
from argparse import ArgumentParser, Namespace
from os import makedirs, path, remove
from shutil import copyfile, which
from subprocess import run

MEDIAPIPE_DIR = path.join(path.dirname(__file__), "mediapipe")

TARGETS = {
    "android": "@GDMP//GDMP/android:GDMP",
    "desktop": "@GDMP//GDMP/desktop:GDMP",
    "ios": "@GDMP//GDMP/ios:GDMP",
    "web": "@GDMP//GDMP/web:GDMP",
}


def bazel_build(args: list[str]):
    bazel_exec = which("bazelisk") or which("bazel")
    if bazel_exec is None:
        print(
            "Error: Cannot find bazel, please check bazel is installed and is in PATH."
        )
        sys.exit(-1)
    cmd = [bazel_exec, "build", "-c", "opt"]
    cmd.append("--action_env=HERMETIC_PYTHON_VERSION=3.12")
    cmd.extend(args)
    run(cmd, cwd=MEDIAPIPE_DIR, check=True)


def build_android(args: Namespace) -> list[str]:
    arch: str = args.arch
    build_args = [
        "--extra_toolchains=@androidndk//:all",
        "--config=android",
        "--copt=-fPIC",
        "--define=OPENCV=source",
    ]
    if arch.startswith("arm64"):
        arch = "arm64-v8a"
    build_args.append(f"--cpu={arch}")
    return build_args


def build_desktop(args: Namespace) -> list[str]:
    arch: str = args.arch
    desktop_args = {
        "linux": [
            "--copt=-DMESA_EGL_NO_X11_HEADERS",
            "--copt=-DEGL_NO_X11",
            "--copt=-fPIC",
            "--define=OPENCV=source",
        ],
        "darwin": [
            "--apple_generate_dsym=false",
            "--define=OPENCV=source",
            "--define=MEDIAPIPE_DISABLE_GPU=1",
        ],
        "win32": [
            "--conlyopt=/std:c11",
            "--conlyopt=/experimental:c11atomics",
            "--define=OPENCV=source",
            "--define=MEDIAPIPE_DISABLE_GPU=1",
        ],
    }
    build_args = desktop_args[sys.platform]
    if sys.platform == "darwin":
        if arch == "arm64":
            build_args.append("--cpu=darwin_arm64")
            build_args.append("--macos_minimum_os=11.0")
        elif arch == "x86_64":
            build_args.append("--cpu=darwin_x86_64")
            build_args.append("--macos_minimum_os=10.13")
            build_args.append("--define=xnn_enable_avxvnniint8=false")
    elif sys.platform == "linux":
        if arch == "arm64":
            build_args.append("--copt=-fpermissive")
    return build_args


def build_ios(args: Namespace) -> list[str]:
    arch: str = args.arch
    build_args = [
        "--apple_generate_dsym=false",
        "--config=ios",
        "--define=OPENCV=source",
    ]
    return build_args


def build_web(args: Namespace) -> list[str]:
    build_args = [
        "--incompatible_enable_cc_toolchain_resolution",
        "--crosstool_top=@emsdk//emscripten_toolchain:everything",
        "--host_crosstool_top=@bazel_tools//tools/cpp:toolchain",
        "--copt=-D_LARGEFILE64_SOURCE",
        "--copt=-fexceptions",
        "--copt=-pthread",
        "--copt=-sDISABLE_EXCEPTION_CATCHING=1",
        "--copt=-sSIDE_MODULE=1",
        "--copt=-sSUPPORT_LONGJMP='wasm'",
        "--linkopt=-pthread",
        "--linkopt=-sSIDE_MODULE=1",
        "--linkopt=-sSUPPORT_LONGJMP='wasm'",
        "--linkopt=-sWASM_BIGINT",
        "--linkopt=--oformat=wasm",
        "--define=OPENCV=source",
    ]
    return build_args


def build(args: Namespace):
    target: str = args.target
    build_targets = {
        "android": build_android,
        "desktop": build_desktop,
        "ios": build_ios,
        "web": build_web,
    }
    build_args = build_targets[target](args)
    build_args.append(TARGETS[target])
    bazel_build(build_args)


def copy_android(args: Namespace):
    output: str = args.output
    arch: str = args.arch
    src = path.join(MEDIAPIPE_DIR, "bazel-bin/external/GDMP/GDMP/android/libGDMP.so")
    filename = path.basename(src).split(".")
    filename = ".".join([filename[0], "android", filename[-1]])
    dst = path.join(output, filename)
    copyfile(src, dst)
    if arch.startswith("arm64"):
        arch = "arm64-v8a"
    opencv_lib = glob.glob(
        path.join(
            MEDIAPIPE_DIR,
            "bazel-mediapipe/external/android_opencv/sdk/native/libs",
            arch,
            "libopencv_java4.so",
        )
    )
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
    src = path.join(
        MEDIAPIPE_DIR, "bazel-bin/external/GDMP/GDMP/ios/GDMP.xcframework.zip"
    )
    with zipfile.ZipFile(src) as f:
        f.extractall(output)


def copy_web(args: Namespace):
    output: str = args.output
    src = path.join(MEDIAPIPE_DIR, "bazel-bin/external/GDMP/GDMP/web/GDMP.wasm")
    dst = path.join(output, "GDMP.web.wasm")
    copyfile(src, dst)


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
        "web": copy_web,
    }
    copy_actions[target](args)


if __name__ == "__main__":
    parser = ArgumentParser()
    parser.add_argument("target", choices=list(TARGETS), help="build target")
    parser.add_argument("--arch", default="", help="library architecture")
    parser.add_argument("--output", help="build output directory")
    args = parser.parse_args()
    if not args.arch:
        machine = platform.machine().lower()
        if machine in ["amd64", "x86_64"]:
            args.arch = "x86_64"
        elif machine in ["aarch64", "arm64"]:
            args.arch = "arm64"
    build(args)
    copy_output(args)
