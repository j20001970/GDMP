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
6. (Optional) Set `GODOT_PROJECT` environment variable that points to your Godot project, the build script will try to automate copy process if it is detected.

## Building for Android
1. Install Android SDK and NDK, then setting `ANDROID_HOME` and `ANDROID_NDK_HOME` environment variable that point to your install location.

    You will also need to uncomment `android_sdk_repository` and `android_ndk_repository` that setup script added eariler at the end of `mediapipe/WORKSPACE`

    Refer to [MediaPipe docs](https://developers.google.com/mediapipe/framework/getting_started/android#prerequisite) for more details.
2. Run:

    ```
    build.py android
    ```

    to build android library, then copy `mediapipe/bazel-bin/GDMP/android/libGDMP.so` and `libopencv_java3.so` to `android/src/main/jniLibs/arm64-v8a`
3. Build GDMP aar with Android Studio or Gradle using the `android` project, then copy the built `release` variant aar located in `android/build/outputs/aar` to your Godot project's `addons/GDMP/libs`

## Building for iOS
**Warning**: Building instructions for iOS is very outdated.
If you encountered any problems following the instructions below, try building with Bazel directly by running `bazel(or bazelisk) build --config=ios-arm64 //GDMP/ios:GDMP` on `mediapipe` directory.

1. Use [Tulsi](https://tulsi.bazel.build) project to generate Xcode project.
2. Build `GDMP` static framework target, copy the library from the framework to your Godot project's `addons/GDMP/libs/ios` **and rename it with .a extension**, so that Godot will treat it like static library instead of dynamic when exporting to Xcode.

    (Optional) also copy `opencv2.a` to the project and add it as GDNative library dependencies if OpenCV is used in calculators.
3. After exporting Godot project to Xcode, add necessary frameworks in order to build the app, and go to `Other Linker Flags` in `Build Settings` to force load the static library, for example:

    `-force_load $(PROJECT_DIR)/path/to/your/GDMP.a`

## Building for Linux
1. Install OpenCV and FFmpeg, then modify `mediapipe/third_party/opencv_linux.BUILD` to make OpenCV visible to Bazel.
2. Run:

    ```
    build.py desktop
    ```

    to build desktop library, then copy `mediapipe/bazel-bin/GDMP/desktop/libGDMP.so` to your Godot project's `addons/GDMP/libs/x86_64`

## Building for Windows
1. Install MSVC compiler and Windows SDK, then setting `BAZEL_VC` environment variable pointing to your VC installation.

    Refer to [Bazel documentation](https://bazel.build/configure/windows#build_cpp) for more details.
2. Bash is required for building MediaPipe, make sure `bash` is in `PATH` or setting `BAZEL_SH` environment variable pointing to it.

    Bash can be installed from [Git for Windows](https://gitforwindows.org) or [MSYS2](https://www.msys2.org)
3. Install OpenCV and configure `mediapipe` workspace:
    - Modify `mediapipe/WORKSPACE` for `path` under `windows_opencv` if OpenCV is not installed on `C:\opencv`
    - Modify `OPENCV_VERSION` in `mediapipe/third_party/opencv_windows.BUILD` if OpenCV version is not `3.4.10`

    Refer to [MediaPipe documentation](https://developers.google.com/mediapipe/framework/getting_started/install#installing_on_windows) for more details.
4. Run:

    ```
    build.py desktop
    ```

    to build desktop library, then copy `mediapipe/bazel-bin/GDMP/desktop/GDMP.dll` to your Godot project's `addons/GDMP/libs/x86_64`

    (Optional) also copy `opencv_world3410.dll` to the project and add it as GDNative library dependencies if OpenCV is used in calculators.
