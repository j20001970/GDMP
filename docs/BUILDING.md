# Building GDMP

## Getting Started
1. Clone the repository, remember to **initialize the submodules**.

    `git submodule update --init --recursive [--depth=1]`
2. Install [Bazelisk](https://bazel.build/install/bazelisk) or Bazel version that meets MediaPipe requirement.
3. Run `setup.py` to generate the [godot-cpp](https://github.com/godotengine/godot-cpp) bindings as well as apply/setup source code to the `mediapipe` workspace. Run `setup.py --help` to view the various options.

Example:
```
./setup.py --godot-binary path/to/godot/binary
```

4. Copy `addons` to your Godot project's root directory.
5. (Optional) Add calculator dependencies in `GDMP/GDMP.bzl` if you have additional calculators to compile into GDMP.

## General Build Instructions
`build.py` is a wrapper script that invoke Bazel build commands in `mediapipe` workspace to build GDMP libraries.

Common usage:

```
./build.py [target] --type [debug/release] --output [output-dir]
```
Run `build.py --help` to see all options.

## Building for Android
**Windows users**: Cross-compiling Android library on Windows is currently not supported, please set up WSL or Linux VM before proceed to the instructions below.

1. Install Android SDK and NDK, then setting `ANDROID_HOME` and `ANDROID_NDK_HOME` environment variable that point to your install location.

    You will also need to uncomment `android_sdk_repository` and `android_ndk_repository` that setup script added eariler at the end of `mediapipe/WORKSPACE`

    Refer to [MediaPipe docs](https://developers.google.com/mediapipe/framework/getting_started/android#prerequisite) for more details.
2. Build Android libraries. For example, run:

    ```
    build.py android --arch arm64-v8a
    ```

    to build library for `arm64-v8a` architecture, you can pass comma-separated list to build multiple architectures.
    If `--arch` is omitted, building native libraries will be skipped, and procceed to build Android AAR directly.
    You can use `--android-skip-aar` to skip building Android AAR.

## Building for iOS
1. Install [Xcode](https://developer.apple.com/xcode), then switch to installed Xcode by using `xcode-select`
2. Run `build.py ios` to build iOS library

    Bazel should build GDMP for your Mac's CPU architecture by default, if you would like to build multiple architectures, append `--arch x86_64,arm64` to the build command to build both x86_64 and arm64 architectures for example.

## Building for Linux
**Flatpak environment**: If you intend to distribute the library, you might want to use an environment isolated from host before you proceed. You can use `scripts/flatpak_env.sh` to enable Flatpak environment.

1. Install OpenCV and FFmpeg, then modify `mediapipe/third_party/opencv_linux.BUILD` to make OpenCV visible to Bazel.
2. Run `build.py desktop` to build desktop library.

## Building for Windows
1. Install MSVC compiler and Windows SDK, then setting `BAZEL_VC` environment variable pointing to your VC installation.

    Refer to [Bazel documentation](https://bazel.build/configure/windows#build_cpp) for more details.
2. Bash is required for building MediaPipe, make sure `bash` is in `PATH` or setting `BAZEL_SH` environment variable pointing to it.

    Bash can be installed from [Git for Windows](https://gitforwindows.org) or [MSYS2](https://www.msys2.org)
3. Install OpenCV and configure `mediapipe` workspace:
    - Modify `mediapipe/WORKSPACE` for `path` under `windows_opencv` if OpenCV is not installed on `C:\opencv`
    - Modify `OPENCV_VERSION` in `mediapipe/third_party/opencv_windows.BUILD` if OpenCV version is not `3.4.10`

    Refer to [MediaPipe documentation](https://developers.google.com/mediapipe/framework/getting_started/install#installing_on_windows) for more details.
4. Run `build.py desktop` to build desktop library.
