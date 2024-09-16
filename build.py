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
    "android": "//GDMP/android:GDMP",
    "desktop": "//GDMP/desktop:GDMP",
    "ios": "//GDMP/ios:GDMP",
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
    if sys.platform == "win32":
        python_path = sys.executable.replace("\\", "\\\\")
        cmd.extend(["--action_env", f"PYTHON_BIN_PATH={python_path}"])
    cmd.extend(args)
    return partial(run, cmd, cwd=MEDIAPIPE_DIR, check=True)


def build_android(args: Namespace, build_args: list[str]) -> list[Callable]:
    build_type: str = args.type
    arch: str = args.arch
    skip_aar: bool = args.android_skip_aar
    cmds = []
    if not path.exists(ANDROID_PROJECT):
        print("Error: android project does not exist.")
        sys.exit(-1)
    src = path.join(MEDIAPIPE_DIR, "bazel-bin/GDMP/android/libGDMP.so")
    if arch:
        jni_dir = path.join(ANDROID_PROJECT, "src/main/jniLibs")
        if path.exists(jni_dir):
            cmds.append(partial(rmtree, jni_dir))
        abi_list = arch.split(",")
        for abi in abi_list:
            src_opencv = path.join(
                MEDIAPIPE_DIR,
                "bazel-mediapipe/external/android_opencv/sdk/native/libs",
                abi,
                "libopencv_java4.so",
            )
            dst_dir = path.join(jni_dir, abi)
            dst = path.join(dst_dir, path.basename(src))
            dst_opencv = path.join(dst_dir, path.basename(src_opencv))
            arg = [arg.format(abi=abi) for arg in build_args]
            cmds.append(bazel_build(arg))
            cmds.append(partial(makedirs, dst_dir, exist_ok=True))
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
    else:
        build_args.extend(TARGET_ARGS[target])
    if target == "android":
        build_args.extend([f"--cpu={arch}", TARGETS[target]])
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
    output: str = args.output
    src = path.join(ANDROID_PROJECT, f"build/outputs/aar/GDMP-{build_type}.aar")
    dst = path.join(output, "GDMP.android.aar")
    copyfile(src, dst)


def copy_desktop(args: Namespace):
    output: str = args.output
    desktop_platform = platform.system().lower()
    desktop_output = path.join(MEDIAPIPE_DIR, "bazel-bin/GDMP/desktop")
    if desktop_platform == "linux":
        src = path.join(desktop_output, "libGDMP.so")
    elif desktop_platform == "windows":
        src = path.join(desktop_output, "GDMP.dll")
    filename = path.basename(src).split(".")
    filename = ".".join([filename[0], desktop_platform, filename[-1]])
    dst = path.join(output, filename)
    if path.exists(dst):
        remove(dst)
    copyfile(src, dst)
    if desktop_platform == "windows":
        opencv_lib = glob.glob(path.join(desktop_output, "opencv_world*.dll"))
        if len(opencv_lib) == 0:
            return
        src = opencv_lib[0]
        copyfile(src, path.join(output, path.basename(src)))


def copy_ios(args: Namespace):
    output: str = args.output
    src = path.join(MEDIAPIPE_DIR, "bazel-bin/GDMP/ios/GDMP.zip")
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
        "--type", choices=["debug", "release"], default="debug", help="build type"
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
