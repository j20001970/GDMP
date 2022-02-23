#!/usr/bin/python 

import os
from shutil import copytree, rmtree

mediapipe_dir = os.path.join(os.path.dirname(__file__), 'mediapipe')
copy_dir = os.path.join(os.path.dirname(__file__), 'GDMP')

try:
    dest_dir = os.path.join(mediapipe_dir, 'mediapipe', 'GDMP')
    if not os.path.isdir(dest_dir):
        os.symlink(copy_dir, dest_dir, True)
except OSError:
    print("Symlink not available, procceed to copy files.")
    if os.path.isdir(dest_dir):
        rmtree(dest_dir)
    copytree(copy_dir, dest_dir)

try:
    external_dir = os.path.join(mediapipe_dir, "external")
    os.symlink("bazel-out/../../../external", external_dir, True)
except Exception as e:
    print(e)
