#!/usr/bin/env python

import glob
import platform
import sys
import zipfile
from argparse import ArgumentParser, Namespace
from collections.abc import Callable
from functools import partial
from os import environ, makedirs, path, remove
from shutil import copyfile, rmtree, which
from subprocess import run

MEDIAPIPE_DIR = path.join(path.dirname(__file__), "mediapipe")

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

GODOT_PATH: str = None
if "GODOT_PROJECT" in environ:
    godot_path = environ["GODOT_PROJECT"]
    if path.exists(path.join(godot_path, "project.godot")):
        GODOT_PATH = environ["GODOT_PROJECT"]
        print(f"Godot project found on {godot_path}")


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


def build_android(
    project_dir: str, build_type: str, build_args: list[str], abi_list: list[str]
) -> list[Callable]:
    cmds = []
    src = path.join(MEDIAPIPE_DIR, "bazel-bin/GDMP/android/libGDMP.so")
    jni_dir = path.join(project_dir, "src/main/jniLibs")
    if path.exists(jni_dir):
        cmds.append(partial(rmtree, jni_dir))
    for abi in abi_list:
        src_opencv = path.join(
            MEDIAPIPE_DIR,
            "bazel-mediapipe/external/android_opencv/sdk/native/libs",
            abi,
            "libopencv_java3.so",
        )
        dst_dir = path.join(jni_dir, abi)
        dst = path.join(dst_dir, path.basename(src))
        dst_opencv = path.join(dst_dir, path.basename(src_opencv))
        build_args = [arg.format(abi=abi) for arg in build_args]
        cmds.append(bazel_build(build_args))
        cmds.append(partial(makedirs, dst_dir, exist_ok=True))
        cmds.append(partial(copyfile, src, dst))
        cmds.append(partial(copyfile, src_opencv, dst_opencv))
    gradlew_exec = path.join(project_dir, "gradlew")
    gradle_build = [gradlew_exec, "clean", f"assemble{build_type.capitalize()}"]
    cmds.append(partial(run, gradle_build, cwd=project_dir, check=True))
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
        android_project = path.join(path.dirname(__file__), "android")
        if not path.exists(android_project):
            print("Error: android project does not exist.")
            sys.exit(-1)
        if arch is None:
            arch = platform.machine().lower()
        abi_list = arch.split(",")
        build_args.extend(["--cpu={abi}", TARGETS[target]])
        cmds.extend(build_android(android_project, build_type, build_args, abi_list))
        return cmds
    elif target == "ios" and arch != None:
        build_args.append(f"--ios_multi_cpus={arch}")
    build_args.append(TARGETS[target])
    cmd = bazel_build(build_args)
    cmds.append(cmd)
    return cmds


def copy_to_godot(src, dst):
    if GODOT_PATH is None:
        return
    i = input(f"Copy {path.basename(src)} to {dst}? [Y/n] ")
    if len(i) and not i.lower().startswith("y"):
        return
    dst = path.join(GODOT_PATH, dst)
    makedirs(path.dirname(dst), exist_ok=True)
    if path.exists(dst):
        remove(dst)
    copyfile(src, dst)


def copy_android(args: Namespace):
    build_type: str = args.type
    android_project = path.join(path.dirname(__file__), "android")
    src = path.join(android_project, f"build/outputs/aar/GDMP-{build_type}.aar")
    gdap = path.join(android_project, "GDMP.gdap")
    dst = "android/plugins"
    copy_to_godot(src, path.join(dst, "GDMP.android.aar"))
    copy_to_godot(gdap, path.join(dst, path.basename(gdap)))


def copy_desktop(args: Namespace):
    arch: str = args.arch
    if arch is None:
        arch = platform.machine().lower()
    if arch == "amd64":
        arch = "x86_64"
    desktop_platform = platform.system().lower()
    desktop_output = path.join(MEDIAPIPE_DIR, "bazel-bin/GDMP/desktop")
    if desktop_platform == "linux":
        src = path.join(desktop_output, "libGDMP.so")
    elif desktop_platform == "windows":
        src = path.join(desktop_output, "GDMP.dll")
    dst = path.join("addons/GDMP/libs", arch)
    filename = path.basename(src).split(".")
    filename = ".".join([filename[0], desktop_platform, filename[-1]])
    copy_to_godot(src, path.join(dst, filename))
    if desktop_platform == "windows":
        opencv_lib = glob.glob(path.join(desktop_output, "opencv_world*.dll"))
        if len(opencv_lib) == 0:
            return
        src = opencv_lib[0]
        copy_to_godot(src, path.join(dst, path.basename(src)))


def copy_ios(args: Namespace):
    src = path.join(MEDIAPIPE_DIR, "bazel-bin/GDMP/ios/GDMP.zip")
    dst = "addons/GDMP/libs"
    if GODOT_PATH is None:
        return
    i = input(f"Extract {path.basename(src)} to {dst}? [Y/n] ")
    if len(i) and not i.lower().startswith("y"):
        return
    dst = path.join(GODOT_PATH, dst)
    with zipfile.ZipFile(src) as f:
        f.extractall(dst)


def copy_to_project(args: Namespace):
    target: str = args.target
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
    parser.add_argument("--arch", help="library architecture")
    args = parser.parse_args()
    cmds = get_build_cmds(args)
    for cmd in cmds:
        cmd()
    copy_to_project(args)
