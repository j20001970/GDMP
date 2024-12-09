#!/usr/bin/env python

import os
import platform
import sys
from argparse import ArgumentParser
from os import chdir, path
from shutil import which
from subprocess import run

ROOT_DIR = path.join(path.dirname(__file__), "..")

MEDIAPIPE_DIR = path.join(ROOT_DIR, "mediapipe")

COMPILEDB_SRC = path.join(MEDIAPIPE_DIR, "compile_commands.json")
COMPILEDB_DST = path.join(ROOT_DIR, "compile_commands.json")

TARGET = "@GDMP//GDMP:refresh_compile_commands"

current_platform = platform.system().lower()
if current_platform == "windows":

    def symlink(src, dst):
        # Only works on Windows if the command is passed in this format
        run(f'mklink /J "{dst}" "{src}"', check=True, shell=True)

else:  # Linux/MacOS should work roughly the same

    def symlink(src, dst):
        os.symlink(src, dst, True)


if __name__ == "__main__":
    parser = ArgumentParser()
    parser.add_argument("--type", choices=["debug", "release"], default="release")
    args = parser.parse_args()
    if args.type == "release":
        build_type = "opt"
    else:
        build_type = "dbg"

    # check bazel executable
    bazel_exec = which("bazelisk") or which("bazel")
    if bazel_exec is None:
        print(
            "Error: Cannot find bazel, please check bazel is installed and is in environment paths."
        )
        sys.exit(-1)

    bazel_args = [bazel_exec, "run"]

    if sys.platform == "win32":
        bazel_args.extend(action_env)

    bazel_args.extend([TARGET, "--", "-c", build_type])

    if sys.platform == "win32":
        bazel_args.extend(action_env)
        bazel_args.extend(["--define", "MEDIAPIPE_DISABLE_GPU=1"])

    chdir(MEDIAPIPE_DIR)
    run(bazel_args)

    if not path.exists(COMPILEDB_DST):
        symlink(COMPILEDB_SRC, COMPILEDB_DST)
