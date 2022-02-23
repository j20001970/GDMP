#!/usr/bin/python
import os
import subprocess
import sys
from argparse import ArgumentParser
from shutil import which

mediapipe_dir = os.path.join(os.path.dirname(__file__), 'mediapipe')

parser = ArgumentParser()
parser.add_argument('target', choices=['aar', 'android', 'desktop'], help='target to build')
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
            '--copt', '-fPIC', \
            '//mediapipe/GDMP:gdmp'])
    elif args.target.lower() == 'aar':
        bazel_args.extend([\
            '--host_crosstool_top=@bazel_tools//tools/cpp:toolchain', \
            '--define', 'EXCLUDE_OPENCV_SO_LIB=1', \
            '--fat_apk_cpu=arm64-v8a', \
            '--linkopt=-s', \
            '//mediapipe/GDMP/mediapipe_aar/java/com/google/mediapipe:mediapipe_aar'])
    elif args.target.lower() == 'desktop':
        bazel_args.extend([\
            '--copt', '-fPIC', \
            '//mediapipe/GDMP:gdmp'])
    else:
        print("unknown target, exiting.")
        sys.exit(-1)
    os.chdir(mediapipe_dir)
    subprocess.run(bazel_args)
except Exception as e:
    print(e)
    sys.exit(-1)
