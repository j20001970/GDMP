#!/usr/bin/python

from os import path, symlink, unlink
import subprocess
import sys
from shutil import rmtree, which

mediapipe_dir = path.join(path.dirname(__file__), 'mediapipe')

# Patching mediapipe workspace
mediapipe_setup = path.join(path.dirname(__file__), 'mediapipe_setup.diff')
if which('git'):
    print("Using git to patch mediapipe workspace.")
    subprocess.run(['git', 'apply', '--unsafe-paths',
                   '--directory='+mediapipe_dir, mediapipe_setup])
elif which('patch'):
    print("Using patch to patch mediapipe workspace.")
    subprocess.run(
        ['patch', '-p1', '-d', mediapipe_dir, '-i', mediapipe_setup])
else:
    print("Error: 'git' or 'patch' cannot be found for patching mediapipe workspace.")
    sys.exit(-1)

# Symlink GDMP source code to mediapipe workspace
src_dir = path.join(path.dirname(__file__), 'GDMP')
dst_dir = path.join(mediapipe_dir, 'mediapipe', 'GDMP')
if path.isdir(dst_dir):
    try:
        unlink(dst_dir)
    except:
        rmtree(dst_dir)
try:
    if sys.platform.startswith('win32'):
        subprocess.check_call('mklink /J "%s" "%s"' %
                              (dst_dir, src_dir), shell=True)
    else:
        symlink(src_dir, dst_dir, True)
except:
    print("Error: Unable to symlink GDMP source to mediapipe workspace.")
    sys.exit(-1)
