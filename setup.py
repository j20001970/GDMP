#!/usr/bin/env python

import sys
import os
from os import path, unlink
from shutil import rmtree, which
from subprocess import run
import platform
from argparse import ArgumentParser, Namespace
import venv

GODOT_CPP_DIR = path.join(path.dirname(__file__), "godot-cpp")
GODOT_CPP_API_JSON = path.join(GODOT_CPP_DIR, "godot-headers/api.json")

MEDIAPIPE_DIR = path.join(path.dirname(__file__), "mediapipe")
MEDIAPIPE_GDMP_SYMLINK = path.join(MEDIAPIPE_DIR, "GDMP")
MEDIAPIPE_WORKSPACE = path.join(MEDIAPIPE_DIR, "WORKSPACE")

GDMP_MEDIAPIPE_SETUP = path.join(path.dirname(__file__), "mediapipe_setup.diff")
GDMP_SRC_DIR = path.join(path.dirname(__file__), "GDMP")
GDMP_VENV_DIR = path.join(path.dirname(__file__), "venv")
GDMP_VENV_REQUIREMENTS = path.join(path.dirname(__file__), "requirements.txt")

DEFAULT_OPENCV_VERSION = "3.4.10"

current_platform = platform.system().lower()
if current_platform == "windows":
    DEFAULT_OPENCV_INSTALL_PATH_REPLACE = 'path = "C:\\\\opencv\\\\build",'
    OPENCV_INSTALL_PATH_FORMAT = 'path = "{}",'

    OPENCV_BUILD_PATH = path.join(MEDIAPIPE_DIR, "third_party/opencv_windows.BUILD")
    DEFAULT_OPENCV_VERSION_REPLACE = 'OPENCV_VERSION = "3410"'
    OPENCV_VERSION_FORMAT = 'OPENCV_VERSION = "{}"'

if current_platform == "windows":

    def symlink(src_dir, dst_dir):
        # Only works on Windows if the command is passed in this format
        run(f'mklink /J "{dst_dir}" "{src_dir}"', check=True, shell=True)

else:  # Linux/MacOS should work roughly the same

    def symlink(src_dir, dst_dir):
        os.symlink(src_dir, dst_dir, True)


def generate_bindings(api_json_path: str) -> None:
    # Could use a contextmanager but that feels like overkill
    os.chdir(GODOT_CPP_DIR)
    sys.path.append(os.getcwd())

    import binding_generator as bg

    bg.generate_bindings(api_json_path, True)

    sys.path.pop()


def patch_and_symlink() -> None:
    # Patching mediapipe workspace
    if which("git"):
        print("Using git to patch mediapipe workspace.")
        run(
            [
                "git",
                "apply",
                "--unsafe-paths",
                f"--directory={MEDIAPIPE_DIR}",
                GDMP_MEDIAPIPE_SETUP,
            ]
        )
    elif which("patch"):
        print("Using patch to patch mediapipe workspace.")
        run(["patch", "-p1", "-d", MEDIAPIPE_DIR, "-i", GDMP_MEDIAPIPE_SETUP])
    else:
        print(
            "Error: 'git' or 'patch' cannot be found for patching mediapipe workspace."
        )
        sys.exit(-1)

    # Symlink GDMP source code to mediapipe workspace
    if path.isdir(MEDIAPIPE_GDMP_SYMLINK):
        try:
            unlink(MEDIAPIPE_GDMP_SYMLINK)
        except:
            rmtree(MEDIAPIPE_GDMP_SYMLINK)
    try:
        symlink(GDMP_SRC_DIR, MEDIAPIPE_GDMP_SYMLINK)
    except:
        print("Error: Unable to symlink GDMP source to mediapipe workspace.")
        sys.exit(-1)


def modify_file(
    file_path: str, replace_str: str, format_str: str, replacement: str
) -> None:
    print(f"Modifying file {file_path}")

    # TODO could maybe use a raw string?
    formatted_str: str = format_str.format(replacement.replace("\\", "\\\\"))
    print(f"Replacing {replace_str} with {formatted_str}")

    with open(file_path, "r") as f:
        new_text: str = f.read().replace(replace_str, formatted_str)

    with open(file_path, "w") as f:
        f.write(new_text)


def workspace_android_rules() -> None:
    with open(MEDIAPIPE_WORKSPACE, "r") as f:
        content = f.read()
    with open(MEDIAPIPE_WORKSPACE, "a") as f:
        if not "android_sdk_repository" in content:
            f.write('#android_sdk_repository(name = "androidsdk")\n')
        if not "android_ndk_repository" in content:
            f.write('#android_ndk_repository(name = "androidndk", api_level=21)\n')


def create_venv(current_platform: str) -> None:
    if not path.exists(GDMP_VENV_DIR):
        venv.create(GDMP_VENV_DIR, with_pip=True)
        if not path.exists(GDMP_VENV_DIR):
            raise RuntimeError(f"Unable to create venv at path {GDMP_VENV_DIR}")

    pip_bin: str = "{}/bin/pip"
    activate_command: str = "source venv/bin/activate"
    if current_platform == "windows":
        pip_bin = "{}/Scripts/pip.exe"
        activate_command = "source venv/Scripts/activate"

    run(
        [pip_bin.format(GDMP_VENV_DIR), "install", "-r", GDMP_VENV_REQUIREMENTS],
        check=True,
    )

    print(
        f"\n++++++\nPlease activate the venv before building GDMP by running `{activate_command}`\n++++++\n"
    )


if __name__ == "__main__":
    parser = ArgumentParser()

    # Extension generation
    parser.add_argument(
        "--api-json",
        default=GODOT_CPP_API_JSON,
        help="Path to a pre-generated api.json file. Defaults to api.json provided by godot-cpp",
    )
    parser.add_argument(
        "--godot-binary",
        required=False,
        help="Path to the Godot editor binary for generating bindings",
    )

    # Platform specific
    if current_platform == "windows":
        parser.add_argument(
            "--custom-opencv-dir",
            required=False,
            help="Path to custom OpenCV directory in either Linux or Windows path format. The path will be automatically converted",
        )
        parser.add_argument(
            "--custom-opencv-version",
            default=DEFAULT_OPENCV_VERSION,
            help=f"Version of OpenCV to use. Defaults to {DEFAULT_OPENCV_VERSION}",
        )

    args: Namespace = parser.parse_args()

    api_json_path: str = path.abspath(args.api_json)
    if args.godot_binary:
        godot_binary = path.abspath(args.godot_binary)
        os.chdir(path.dirname(__file__))
        run(
            [
                args.godot_binary,
                "--gdnative-generate-json-api",
                "--no-window",
                "api.json",
            ],
            check=True,
        )
        api_json_path = path.join(os.getcwd(), "api.json")

    generate_bindings(api_json_path)

    patch_and_symlink()

    workspace_android_rules()

    if current_platform == "windows":
        if args.custom_opencv_dir:
            modify_file(
                MEDIAPIPE_WORKSPACE,
                DEFAULT_OPENCV_INSTALL_PATH_REPLACE,
                OPENCV_INSTALL_PATH_FORMAT,
                os.path.abspath(os.path.expanduser(args.custom_opencv_dir)),
            )
        if (
            args.custom_opencv_version
            and args.custom_opencv_version != DEFAULT_OPENCV_VERSION
        ):
            modify_file(
                OPENCV_BUILD_PATH,
                DEFAULT_OPENCV_VERSION_REPLACE,
                OPENCV_VERSION_FORMAT,
                args.custom_opencv_version.replace(".", ""),
            )

    create_venv(current_platform)
