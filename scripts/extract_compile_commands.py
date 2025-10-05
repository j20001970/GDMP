#!/usr/bin/env python

import os
import platform
import sys
from os import path
from subprocess import run

sys.path.append(path.join(path.dirname(__file__), ".."))

import build
from config import *

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
    args = build.get_args()
    bazel_exec = get_bazel()
    cmd = [bazel_exec, "run"]
    cmd.append("--action_env=HERMETIC_PYTHON_VERSION=3.12")
    cmd.extend([TARGET, "--"])
    cmd.extend(build.get_build_args(args))
    run(cmd, cwd=MEDIAPIPE_DIR)
    if not path.exists(COMPILEDB_DST):
        symlink(COMPILEDB_SRC, COMPILEDB_DST)
