#!/usr/bin/env python

import sys
from argparse import ArgumentParser, Namespace
from os import chdir, environ, makedirs, path, remove
from shutil import copyfile, which
from subprocess import run

MEDIAPIPE_DIR = path.join(path.dirname(__file__), "mediapipe")

TARGETS = {
    "android": "//GDMP/android:GDMP",
    "desktop": "//GDMP/desktop:GDMP",
}

TARGET_COMMANDS = {
    "android": [
        "--config=android",
        "--copt=-fPIC",
        "--cpu=arm64-v8a",
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
}

GODOT_PATH: str = None
if "GODOT_PROJECT" in environ:
    godot_path = environ["GODOT_PROJECT"]
    if path.exists(path.join(godot_path, "project.godot")):
        GODOT_PATH = environ["GODOT_PROJECT"]
        print(f"Godot project found on {godot_path}")


def get_build_args(args: Namespace):
    target: str = args.target
    build_type: str = args.type
    bazel_exec = which("bazelisk") or which("bazel")
    if bazel_exec is None:
        print(
            "Error: Cannot find bazel, please check bazel is installed and is in PATH."
        )
        sys.exit(-1)
    build_args = [bazel_exec, "build"]
    if sys.platform == "win32":
        python_path = sys.executable.replace("\\", "\\\\")
        build_args.extend(["--action_env", f"PYTHON_BIN_PATH={python_path}"])
    if build_type == "release":
        build_type = "opt"
    else:
        build_type = "dbg"
    build_args.extend(["-c", build_type])
    target_args = TARGET_COMMANDS[target]
    if target == "desktop":
        target_args = target_args[sys.platform]
    build_args.extend(target_args)
    build_args.append(TARGETS[target])
    return build_args


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
    android_project = path.join(path.dirname(__file__), "android")
    if not path.exists(android_project):
        print("Error: android project does not exist.")
        sys.exit(-1)
    src = path.join(MEDIAPIPE_DIR, "bazel-bin/GDMP/android/libGDMP.so")
    jni_dst = path.join(android_project, "src/main/jniLibs")
    jni_dst = path.join(jni_dst, "arm64-v8a")
    jni_dst = path.join(jni_dst, path.basename(src))
    i = input(f"Copy {path.basename(src)} to {path.relpath(jni_dst)}? [Y/n] ")
    if len(i) and not i.lower().startswith("y"):
        return
    makedirs(path.dirname(jni_dst), exist_ok=True)
    copyfile(src, jni_dst)
    if not path.exists(path.join(path.dirname(jni_dst), "libopencv_java3.so")):
        jni_dst = path.relpath(jni_dst, android_project)
        print(f"Please also copy libopencv_java3.so to {jni_dst} before building aar.")
    i = input("Build aar with Gradle? [Y/n] ")
    if len(i) and not i.lower().startswith("y"):
        return
    gradlew_exec = path.join(android_project, "gradlew")
    chdir(android_project)
    ret = run([gradlew_exec, "assembleRelease"]).returncode
    if ret == 0:
        dst = "android/plugins"
        aar = path.join(android_project, "build/outputs/aar/GDMP-release.aar")
        copy_to_godot(aar, path.join(dst, path.basename(aar)))
        gdap = path.join(android_project, "GDMP.gdap")
        copy_to_godot(gdap, path.join(dst, path.basename(gdap)))


def copy_desktop(args: Namespace):
    desktop_outputs = {
        "linux": path.join(MEDIAPIPE_DIR, "bazel-bin/GDMP/desktop/libGDMP.so"),
        "win32": path.join(MEDIAPIPE_DIR, "bazel-bin/GDMP/desktop/GDMP.dll"),
    }
    src = path.join(MEDIAPIPE_DIR, desktop_outputs[sys.platform])
    dst = path.join("addons/GDMP/libs")
    dst = path.join(dst, "x86_64")
    filename = path.basename(src)
    copy_to_godot(src, path.join(dst, filename))


def copy_output(args: Namespace):
    target: str = args.target
    copy_actions = {
        "android": copy_android,
        "desktop": copy_desktop,
    }
    copy_actions[target](args)


if __name__ == "__main__":
    parser = ArgumentParser()
    parser.add_argument("target", choices=list(TARGETS), help="build target")
    parser.add_argument(
        "--type", choices=["debug", "release"], default="debug", help="build type"
    )
    args = parser.parse_args()
    build_args = get_build_args(args)
    chdir(MEDIAPIPE_DIR)
    ret = run(build_args).returncode
    if ret == 0:
        copy_output(args)
