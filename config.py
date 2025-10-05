# /usr/bin/env python

import os
import sys
from os import path
from shutil import which

ROOT_DIR = path.dirname(__file__)
GODOT_CPP_DIR = path.join(ROOT_DIR, "godot-cpp")
MEDIAPIPE_DIR = path.join(ROOT_DIR, "mediapipe")
GDMP_BIN_DIR = path.join(ROOT_DIR, "bin")


def get_bazel() -> str:
    PATH = os.environ.get("PATH")
    if PATH:
        PATH += f":{GDMP_BIN_DIR}"
    else:
        PATH = GDMP_BIN_DIR
    bazel = which("bazelisk", path=PATH) or which("bazel", path=PATH)
    if bazel is None:
        print(
            "Error: Cannot find bazel, please check bazel is installed and is in environment paths."
        )
        sys.exit(-1)
    return bazel
