#!/usr/bin/env python

import sys
from argparse import ArgumentParser
from os import chdir, path
from shutil import which
from subprocess import run

MEDIAPIPE_DIR = path.join(path.dirname(__file__), "../mediapipe")

TARGET = "//GDMP:refresh_compile_commands"

if __name__ == "__main__":
    parser = ArgumentParser()
    parser.add_argument("--type", choices=["debug", "release"], default="release")
    args = parser.parse_args()
    if args.type == "release":
        build_type = "opt"
    else:
        build_type = "dbg"
    python = sys.executable
    if sys.platform == "win32":
        python = python.replace("\\", "\\\\")
    action_env = ["--action_env", f"PYTHON_BIN_PATH={python}"]

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
