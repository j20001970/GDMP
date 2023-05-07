#!/usr/bin/python

import sys
from argparse import ArgumentParser
from os import chdir, environ
from os import makedirs as mkdir
from os import path, remove
from shutil import copyfile as copy
from shutil import which
from subprocess import run

mediapipe_dir = path.join(path.dirname(__file__), 'mediapipe')

targets = {
    'desktop': '//GDMP/desktop:GDMP',
    'android': '//GDMP/android:GDMP',
}


def get_build_args(target):
    target_commands = {
        'desktop': {
            'linux': [
                '--copt', '-DMESA_EGL_NO_X11_HEADERS',
                '--copt', '-DEGL_NO_X11',
                '--copt', '-fPIC',
            ],
            'win32': [
                '--define', 'MEDIAPIPE_DISABLE_GPU=1'
            ],
        },
        'android': [
            '--config=android',
            '--copt', '-fPIC',
            '--cpu=arm64-v8a',
        ],
    }
    bazel_exec = which('bazelisk') or which('bazel')
    if bazel_exec is None:
        print("Error: Cannot find bazel, please check bazel is installed and is in PATH.")
        sys.exit(-1)
    build_args = [bazel_exec, 'build']
    if sys.platform == 'win32':
        python_path = sys.executable.replace('\\', '\\\\')
        build_args.extend(['--action_env', 'PYTHON_BIN_PATH=%s' % python_path])
    build_args.extend(['-c', 'opt'])
    target_args = target_commands[target]
    if target == 'desktop':
        target_args = target_args[sys.platform]
    build_args.extend(target_args)
    build_args.append(targets[target])
    return build_args


def copy_android(src, dst):
    android_project = path.join(path.dirname(__file__), 'android')
    if not path.exists(android_project):
        return
    print("Android project found on %s" % android_project)
    jni_dst = path.join('src', 'main', 'jniLibs')
    jni_dst = path.join(jni_dst, 'arm64-v8a')
    i = input("Copy %s to %s? [Y/n] " % (path.basename(src), jni_dst))
    if len(i) and not i.lower().startswith('y'):
        return
    jni_dst = path.join(android_project, jni_dst)
    mkdir(jni_dst, exist_ok=True)
    copy(src, path.join(jni_dst, path.basename(src)))
    if not path.exists(path.join(jni_dst, 'libopencv_java3.so')):
        jni_dst = path.relpath(jni_dst, android_project)
        print("Please also copy libopencv_java3.so to %s before building aar." % jni_dst)
    i = input("Build aar with Gradle? [Y/n] ")
    if len(i) and not i.lower().startswith('y'):
        return
    gradle_exec = path.join(android_project, 'gradlew')
    chdir(android_project)
    ret = run([gradle_exec, 'assembleRelease']).returncode
    if ret == 0:
        output = path.join(android_project, 'build', 'outputs', 'aar')
        output = path.join(output, 'GDMP-release.aar')
        copy_to_godot(output, dst)
        gdap = path.join(path.dirname(__file__), 'plugins', 'GDMP.gdap')
        copy_to_godot(gdap, dst)


def copy_desktop(src, dst):
    dst = path.join(dst, 'x86_64')
    copy_to_godot(src, dst)


def copy_output(target):
    target_outputs = {
        'android': path.join('bazel-bin', 'GDMP', 'android', 'libGDMP.so'),
        'desktop': {
            'linux': path.join('bazel-bin', 'GDMP', 'desktop', 'libGDMP.so'),
            'win32': path.join('bazel-bin', 'GDMP', 'desktop', 'GDMP.dll'),
        },
    }
    copy_actions = {
        'android': copy_android,
        'desktop': copy_desktop
    }
    godot_output = {
        'android': path.join('android', 'plugins'),
        'desktop': path.join('addons', 'GDMP', 'libs'),
    }
    src = target_outputs[target]
    if target == 'desktop':
        src = src[sys.platform]
    src = path.join(mediapipe_dir, src)
    if not path.exists(src):
        print("Error: Build succeeded but output file does not exist.")
        sys.exit(-1)
    dst = godot_output[target]
    copy_actions[target](src, dst)


def copy_to_godot(src, dst):
    if not 'godot_path' in copy_to_godot.__dict__:
        if not 'GODOT_PROJECT' in environ:
            return
        godot_path = environ['GODOT_PROJECT']
        if not path.exists(path.join(godot_path, 'project.godot')):
            return
        copy_to_godot.godot_path = godot_path
        print("Godot project found on %s" % godot_path)
    godot_path = copy_to_godot.godot_path
    i = input("Copy %s to %s? [Y/n] " % (path.basename(src), dst))
    if len(i) and not i.lower().startswith('y'):
        return
    dst = path.join(godot_path, dst)
    mkdir(dst, exist_ok=True)
    dst = path.join(dst, path.basename(src))
    if path.exists(dst):
        remove(dst)
    copy(src, dst)


def main(args):
    build_args = get_build_args(args.target)
    chdir(mediapipe_dir)
    ret = run(build_args).returncode
    if ret == 0:
        copy_output(args.target)


if __name__ == '__main__':
    parser = ArgumentParser()
    parser.add_argument('target', choices=list(targets), help='build target')
    args = parser.parse_args()
    main(args)
