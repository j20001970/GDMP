#!/usr/bin/python
import os
import subprocess
import sys
from argparse import ArgumentParser
from shutil import which

mediapipe_dir = os.path.join(os.path.dirname(__file__), 'mediapipe')

parser = ArgumentParser()
parser.add_argument('target', choices=['android', 'desktop'], help='target to build')
args = parser.parse_args()

# check bazel executable
bazel_exec = which('bazelisk') or which('bazel')
if bazel_exec is None:
    print("Cannot find bazel, please check bazel is installed and is in environment paths.")
    sys.exit(-1)

try:
    # whcih target to build
    bazel_args = [bazel_exec, 'build', '-c', 'opt', '--define', 'GODOT=1']
    if args.target.lower() == 'android':
        bazel_args.extend([\
            '--host_crosstool_top=@bazel_tools//tools/cpp:toolchain', \
            '--crosstool_top=//external:android/crosstool', \
            '--cpu=arm64-v8a', \
            '--copt', '-fPIC'])
    elif args.target.lower() == 'desktop':
        if sys.platform.startswith("linux"):
            bazel_args.extend([\
                '--copt', '-DMESA_EGL_NO_X11_HEADERS', \
                '--copt', '-DEGL_NO_X11', \
                '--copt', '-fPIC'])
    else:
        print("unknown target, exiting.")
        sys.exit(-1)
    bazel_args.extend(['//mediapipe/GDMP:gdmp'])
    os.chdir(mediapipe_dir)
    subprocess.run(bazel_args)
except Exception as e:
    print(e)
    sys.exit(-1)
