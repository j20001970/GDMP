#!/usr/bin/python

import sys
from argparse import ArgumentParser
from os import chdir, path
from shutil import which
from subprocess import run

mediapipe_dir = path.join(path.dirname(__file__), 'mediapipe')

targets = {
    'desktop': '//GDMP/desktop:GDMP',
    'android': '//GDMP/android:GDMP',
    'compile_commands': '//GDMP:refresh_compile_commands',
}

target_commands = {
    'desktop': ['build', '-c', 'opt', '--define', 'GODOT=1'],
    'android': [
        'build', '-c', 'opt', '--define', 'GODOT=1',
        '--crosstool_top=//external:android/crosstool',
        '--host_crosstool_top=@bazel_tools//tools/cpp:toolchain',
        '--copt', '-fPIC',
        '--cpu=arm64-v8a',
    ],
    'compile_commands': ['run']
}

desktop_commands = {
    'linux': [
        '--copt', '-DMESA_EGL_NO_X11_HEADERS',
        '--copt', '-DEGL_NO_X11',
        '--copt', '-fPIC',
    ],
    'win32': [
        '--action_env',
        'PYTHON_BIN_PATH=%s' % sys.executable.replace('\\', '\\\\'),
        '--define', 'MEDIAPIPE_DISABLE_GPU=1'
    ],
}

parser = ArgumentParser()
parser.add_argument('target', choices=list(targets), help='build target')
args = parser.parse_args()

# check bazel executable
bazel_exec = which('bazelisk') or which('bazel')
if bazel_exec is None:
    print("Error: Cannot find bazel, please check bazel is installed and is in environment paths.")
    sys.exit(-1)

bazel_args = [bazel_exec]
bazel_args.extend(target_commands[args.target])

if args.target == 'desktop':
    platform = sys.platform
    bazel_args.extend(desktop_commands[platform])

bazel_args.append(targets[args.target])
chdir(mediapipe_dir)
run(bazel_args)
