#!/usr/bin/python

import sys
from os import chdir, path
from shutil import which
from subprocess import run

mediapipe_dir = path.join(path.dirname(__file__), 'mediapipe')

target = '//GDMP:refresh_compile_commands'

python = sys.executable
if sys.platform == 'win32':
    python = python.replace('\\', '\\\\')
action_env = ['--action_env', 'PYTHON_BIN_PATH=%s' % python]

# check bazel executable
bazel_exec = which('bazelisk') or which('bazel')
if bazel_exec is None:
    print("Error: Cannot find bazel, please check bazel is installed and is in environment paths.")
    sys.exit(-1)

bazel_args = [bazel_exec, 'run']

if sys.platform == 'win32':
    bazel_args.extend(action_env)

bazel_args.extend([target, '--', '-c', 'opt', '--define', 'GODOT=1'])

if sys.platform == 'win32':
    bazel_args.extend(action_env)
    bazel_args.extend(['--define', 'MEDIAPIPE_DISABLE_GPU=1'])

chdir(mediapipe_dir)
run(bazel_args)
