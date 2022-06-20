#!/usr/bin/python 

import os
import subprocess
import sys
from shutil import copy, copytree, rmtree, which

mediapipe_dir = os.path.join(os.path.dirname(__file__), 'mediapipe')

# Patching mediapipe workspace
mediapipe_setup = os.path.join(os.path.dirname(__file__), 'mediapipe_setup.diff')
if which('git'):
    print("Using git to patch mediapipe workspace.")
    subprocess.run(['git', 'apply', '--unsafe-paths', '--directory='+mediapipe_dir, mediapipe_setup])
elif which('patch'):
    print("Using patch to patch mediapipe workspace.")
    subprocess.run(['patch', '-p1', '-d', mediapipe_dir, '-i', mediapipe_setup])
else:
    print("'git' or 'patch' cannot be found for patching mediapipe workspace.")
    sys.exit(-1)

# Copy godot.BUILD to mediapipe/third_party
godot_build = os.path.join(os.path.dirname(__file__), 'godot.BUILD')
third_party_dir = os.path.join(mediapipe_dir, 'third_party')
copy(godot_build, third_party_dir)

# Symlink or copy GDMP source code to mediapipe workspace
try:
    copy_dir = os.path.join(os.path.dirname(__file__), 'GDMP')
    dest_dir = os.path.join(mediapipe_dir, 'mediapipe', 'GDMP')
    if not os.path.isdir(dest_dir):
        os.symlink(copy_dir, dest_dir, True)
except OSError:
    print("Symlink not available, procceed to copy files.")
    if os.path.isdir(dest_dir):
        rmtree(dest_dir)
    copytree(copy_dir, dest_dir)

# Symlink external dependencies
try:
    external_dir = os.path.join(mediapipe_dir, "external")
    os.symlink("bazel-out/../../../external", external_dir, True)
except Exception:
    print("Failed to symlink external dependencies, skipping.")
