#!/usr/bin/env python

import sys
import os
from os import path, symlink, unlink
from shutil import rmtree, which
from subprocess import run
import platform
from argparse import ArgumentParser, Namespace
from typing import Callable
import venv

DEFAULT_OPENCV_VERSION = "3.4.10"

current_platform = platform.system().lower()
if current_platform == "windows":
    MEDIAPIPE_WORKSPACE_PATH = "mediapipe/WORKSPACE"
    DEFAULT_OPENCV_INSTALL_PATH_REPLACE = "path = \"C:\\\\opencv\\\\build\","
    OPENCV_INSTALL_PATH_FORMAT = "path = \"{}\","

    OPENCV_BUILD_PATH = "mediapipe/third_party/opencv_windows.BUILD"
    DEFAULT_OPENCV_VERSION_REPLACE = "OPENCV_VERSION = \"3410\""
    OPENCV_VERSION_FORMAT = "OPENCV_VERSION = \"{}\""


def generate_bindings(api_json_path: str) -> None:
    # Could use a contextmanager but that feels like overkill
    os.chdir(path.join(path.dirname(__file__), "godot-cpp"))
    sys.path.append(os.getcwd())

    import binding_generator as bg
    bg.generate_bindings(api_json_path, True)

    sys.path.pop()
    os.chdir("..")


def patch_and_symlink(symlinker: Callable) -> None:
    mediapipe_dir = path.join(path.dirname(__file__), 'mediapipe')

    # Patching mediapipe workspace
    mediapipe_setup = path.join(path.dirname(__file__), 'mediapipe_setup.diff')
    if which('git'):
        print("Using git to patch mediapipe workspace.")
        run(['git', 'apply', '--unsafe-paths',
             '--directory='+mediapipe_dir, mediapipe_setup])
    elif which('patch'):
        print("Using patch to patch mediapipe workspace.")
        run(['patch', '-p1', '-d', mediapipe_dir, '-i', mediapipe_setup])
    else:
        print("Error: 'git' or 'patch' cannot be found for patching mediapipe workspace.")
        sys.exit(-1)

    # Symlink GDMP source code to mediapipe workspace
    src_dir = path.join(path.dirname(__file__), 'GDMP')
    dst_dir = path.join(mediapipe_dir, 'GDMP')
    if path.isdir(dst_dir):
        try:
            unlink(dst_dir)
        except:
            rmtree(dst_dir)
    try:
        symlinker(src_dir, dst_dir)
    except:
        print("Error: Unable to symlink GDMP source to mediapipe workspace.")
        sys.exit(-1)


def modify_file(file_path: str, replace_str: str, format_str: str, replacement: str) -> None:
    print("Modifying file {}".format(file_path))

    # TODO could maybe use a raw string?
    formatted_str: str = format_str.format(replacement.replace("\\", "\\\\"))
    print("Replacing {} with {}".format(
        replace_str, formatted_str))

    with open(file_path, "r") as f:
        new_text: str = f.read().replace(replace_str, formatted_str)

    with open(file_path, "w") as f:
        f.write(new_text)


def create_venv(current_platform: str) -> None:
    venv_path: str = path.join(path.dirname(__file__), "venv")
    if not path.exists(venv_path):
        venv.create(venv_path, with_pip=True)
        if not path.exists(venv_path):
            raise RuntimeError(
                "Unable to create venv at path {}".format(venv_path))

    pip_bin: str = "{}/bin/pip"
    activate_command: str = "source venv/bin/activate"
    if current_platform == "windows":
        pip_bin = "{}/Scripts/pip.exe"
        activate_command = "source venv/Scripts/activate"

    run([pip_bin.format(venv_path), "install",
        "-r", "requirements.txt"], check=True)

    print("\n++++++\nPlease activate the venv before building GDMP by running `{}`\n++++++\n".format(activate_command))


if __name__ == "__main__":
    os.chdir(path.dirname(__file__))

    parser = ArgumentParser()

    # Extension generation
    parser.add_argument(
        "--api-json", required=False, help="Path to a pre-generated api.json file")
    parser.add_argument("--godot-binary", default="godot",
                        help="Path to the Godot editor binary. Defaults to user's PATH if not set. Required if --api-json is not set")

    # Platform specific
    if current_platform == "windows":
        parser.add_argument("--custom-opencv-dir", required=False,
                            help="Path to custom OpenCV directory in either Linux or Windows path format. The path will be automatically converted")
        parser.add_argument("--custom-opencv-version", default=DEFAULT_OPENCV_VERSION,
                            help="Version of OpenCV to use. Defaults to {}".format(DEFAULT_OPENCV_VERSION))

        def symlinker(src_dir, dst_dir):
            # Only works on Windows if the command is passed in this format
            run('mklink /J "%s" "%s"' %
                (dst_dir, src_dir), check=True, shell=True)
    else:  # Linux/MacOS should work roughly the same
        def symlinker(src_dir, dst_dir):
            symlink(src_dir, dst_dir, True)

    args: Namespace = parser.parse_args()

    api_json_path: str = args.api_json
    if not args.api_json:
        run([args.godot_binary, "--gdnative-generate-json-api", "--no-window", "api.json"], check=True)
        api_json_path = path.join(os.getcwd(), "api.json")
    elif not path.isabs(api_json_path):
        api_json_path = path.join(os.getcwd(), api_json_path)

    generate_bindings(api_json_path)

    patch_and_symlink(symlinker)

    if current_platform == "windows":
        if args.custom_opencv_dir:
            modify_file(MEDIAPIPE_WORKSPACE_PATH, DEFAULT_OPENCV_INSTALL_PATH_REPLACE, OPENCV_INSTALL_PATH_FORMAT,
                        os.path.abspath(os.path.expanduser(args.custom_opencv_dir)))
        if args.custom_opencv_version and args.custom_opencv_version != DEFAULT_OPENCV_VERSION:
            modify_file(OPENCV_BUILD_PATH, DEFAULT_OPENCV_VERSION_REPLACE,
                        OPENCV_VERSION_FORMAT, args.custom_opencv_version.replace(".", ""))

    create_venv(current_platform)
