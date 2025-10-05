#!/usr/bin/env python

import glob
import os
import platform
import sys
from argparse import ArgumentParser, Namespace
from os import makedirs, path
from shutil import which
from subprocess import run
import urllib.request
import stat

from config import *

GODOT_CPP_API_JSON = path.join(GODOT_CPP_DIR, "godot-headers/api.json")
MEDIAPIPE_WORKSPACE = path.join(MEDIAPIPE_DIR, "WORKSPACE")
GDMP_PATCH_DIR = path.join(ROOT_DIR, "patch")

current_platform = platform.system().lower()
if current_platform == "windows":
    DEFAULT_OPENCV_VERSION = "3.4.10"
    DEFAULT_OPENCV_INSTALL_PATH_REPLACE = 'path = "C:\\\\opencv\\\\build",'
    OPENCV_INSTALL_PATH_FORMAT = 'path = "{}",'

    OPENCV_BUILD_PATH = path.join(MEDIAPIPE_DIR, "third_party/opencv_windows.BUILD")
    DEFAULT_OPENCV_VERSION_REPLACE = 'OPENCV_VERSION = "3410"'
    OPENCV_VERSION_FORMAT = 'OPENCV_VERSION = "{}"'

current_arch = platform.machine().lower()
if current_arch in ["aarch64", "arm64"]:
    current_arch = "arm64"
if current_arch in ["amd64", "x86_64"]:
    current_arch = "amd64"


def generate_bindings(api_json_path: str) -> None:
    sys.path.append(GODOT_CPP_DIR)
    import binding_generator as bg

    bg.generate_bindings(api_json_path, True, output_dir=GODOT_CPP_DIR)
    sys.path.pop()


def apply_patch(patch_dir: str) -> None:
    patch_exec = []
    if which("git"):
        print("Using git for applying patches.")
        patch_exec = [
            "git",
            "apply",
            "--unsafe-paths",
            "--directory={dir}",
            "{file}",
        ]
    elif which("patch"):
        print("Using patch for applying patches.")
        patch_exec = ["patch", "-p1", "-d", "{dir}", "-i", "{file}"]
    else:
        print("Error: 'git' or 'patch' cannot be found for applying patches.")
        sys.exit(-1)
    for p, _, _ in os.walk(patch_dir):
        if p == patch_dir:
            continue
        patches = glob.glob(path.join(p, "*.diff"))
        if len(patches) == 0:
            continue
        patches.sort()
        target_dir = path.join(ROOT_DIR, path.relpath(p, patch_dir))
        print(f"Patching {path.relpath(target_dir, ROOT_DIR)}")
        for patch in patches:
            print(f"\tApplying {path.basename(patch)}")
            cmd = [s.format(dir=target_dir, file=patch) for s in patch_exec]
            run(cmd)


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
        if not "androidndk" in content:
            f.write('android_ndk_repository(name = "androidndk", api_level=21)\n')
        if not "android/crosstool" in content:
            f.write(
                'bind(name = "android/crosstool", actual = "@androidndk//:toolchain")\n'
            )


def download_progress_hook(count, block_size, total_size):
    percent = int(count * block_size * 100 / total_size)
    print(f"\rDownloading: {percent}%", end="")


def download_bazel_tools() -> None:
    makedirs(GDMP_BIN_DIR, exist_ok=True)

    print(f"Downloading Bazelisk for {current_platform}({current_arch})")
    bazelisk_url = f"https://github.com/bazelbuild/bazelisk/releases/latest/download/bazelisk-{current_platform}-{current_arch}"
    if current_platform == "windows":
        bazelisk_url += ".exe"
    bazelisk_path = path.join(GDMP_BIN_DIR, "bazelisk")
    if current_platform == "windows":
        bazelisk_path += ".exe"
    urllib.request.urlretrieve(
        bazelisk_url, bazelisk_path, reporthook=download_progress_hook
    )
    print()
    perms = os.stat(bazelisk_path).st_mode
    os.chmod(bazelisk_path, perms | stat.S_IXUSR)

    print(f"Donwloading Buildifier for {current_platform}({current_arch})")
    buildifier_url = f"https://github.com/bazelbuild/buildtools/releases/latest/download/buildifier-{current_platform}-{current_arch}"
    if current_platform == "windows":
        if current_arch == "amd64":
            buildifier_url += ".exe"
        else:
            return  # Buildifier for Windows only available on amd64
    buildifier_path = path.join(GDMP_BIN_DIR, "buildifier")
    if current_platform == "windows":
        buildifier_path += ".exe"
    urllib.request.urlretrieve(
        buildifier_url, buildifier_path, reporthook=download_progress_hook
    )
    print()
    perms = os.stat(buildifier_path).st_mode
    os.chmod(buildifier_path, perms | stat.S_IXUSR)


if __name__ == "__main__":
    parser = ArgumentParser()

    # Extension generation
    parser.add_argument(
        "--api-json",
        default=GODOT_CPP_API_JSON,
        help="Path to a pre-generated api.json file. Defaults to api.json provided by godot-cpp",
    )
    parser.add_argument(
        "--no-download",
        action="store_true",
        help="Do not download additional executables.",
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
    generate_bindings(api_json_path)

    apply_patch(GDMP_PATCH_DIR)

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

    if not args.no_download:
        download_bazel_tools()
