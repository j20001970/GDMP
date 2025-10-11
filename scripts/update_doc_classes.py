#!/usr/bin/env python

import sys
from os import path
from argparse import ArgumentParser, Namespace
from subprocess import run

sys.path.append(path.join(path.dirname(__file__), ".."))

from config import *

if __name__ == "__main__":
    parser = ArgumentParser()
    parser.add_argument("project", help="godot project path")
    parser.add_argument("--godot", default="godot", help="godot executable")
    args = parser.parse_args()
    cmd = [args.godot, "--gdextension-docs", "--doctool", path.join(ROOT_DIR, "GDMP")]
    run(cmd, cwd=args.project)
