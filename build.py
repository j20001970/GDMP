#!/usr/bin/python
import os
import subprocess
import sys
from argparse import ArgumentParser
from shutil import copyfile, rmtree, which

mediapipe_dir = os.path.join(os.path.dirname(__file__), 'mediapipe', 'mediapipe')
copy_dir = 'GDMP'

parser = ArgumentParser()
parser.add_argument('target', choices=['aar', 'desktop'], help='target to build')
args = parser.parse_args()

# check bazel executable
bazel_exec = which('bazelisk') or which('bazel')
if bazel_exec is None:
    print("Cannot find bazel, please check bazel is installed and is in environment paths.")
    sys.exit(-1)

try:
    os.chdir(os.path.dirname(__file__))
    # whcih target to build
    bazel_args = [bazel_exec, 'build', '-c', 'opt']
    if args.target.lower() == 'aar':
        bazel_args.extend([\
            '--host_crosstool_top=@bazel_tools//tools/cpp:toolchain', \
            '--fat_apk_cpu=arm64-v8a', '--linkopt=-s', \
            'GDMP/mediapipe_aar/java/com/google/mediapipe:mediapipe_aar'])
    elif args.target.lower() == 'desktop':
        bazel_args.extend([\
            '--copt', '-DMESA_EGL_NO_X11_HEADERS', \
            '--copt', '-DEGL_NO_X11', \
            '--copt', '-fPIC', \
            'GDMP/desktop:libgdmp.so'])
    else:
        print("unknown target, exiting.")
        sys.exit(-1)
    for root, dirs, files in os.walk(copy_dir):
        if not os.path.exists(os.path.join(mediapipe_dir, root)):
            os.makedirs(os.path.join(mediapipe_dir, root))
        for file in files:
            if not os.path.exists(os.path.join(mediapipe_dir, root, file)):
                copyfile(os.path.join(root, file), os.path.join(mediapipe_dir, root, file))
    os.chdir(mediapipe_dir)
    subprocess.run(bazel_args)
    rmtree(os.path.join(mediapipe_dir, copy_dir))
except Exception as e:
    print(e)
    sys.exit(-1)
