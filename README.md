# GDMP
GDMP is a plugin for Godot 3.3+ that allows utilizing MediaPipe graphs in GDScript.

## Getting Started
1. Install [Bazelisk](https://docs.bazel.build/versions/main/install-bazelisk.html) or bazel version that meets MediaPipe requirement.
2. Refer to [Installation](https://google.github.io/mediapipe/getting_started/install.html) for OpenCV and FFmpeg setup.
3. Locate your Godot executable and run `godot --gdnative-generate-json-api api.json` to get a copy of api.json for current Godot version,
    then go to `godot-cpp` directory and run:

    ```
    python -c "from binding_generator import *; generate_bindings('/path/to/your/api.json', True)"
    ```
    to generate Godot C++ bindings directly, replacing `/path/to/your/api.json` with the path to your api.json
4. Apply `mediapipe_setup.diff` to `mediapipe` directory, then copy `godot.BUILD` to `mediapipe/third_party`.
5. Run `setup.py`, the script will try to symlink source code and external dependencies to mediapipe workspace, if symlink is not available on the host it will just copy the code directory instead.
6. Copy `addons/GDMP` to your project's directory.

## Building for Android
1. Refer to [Prerequisite](https://google.github.io/mediapipe/getting_started/android.html#prerequisite) section for Java and Android SDK & NDK setup.
2. Place the calculator dependencies in `GDMP/variables.bzl`
3. Copy or symlink godot-lib to `android/GDMP/libs` directory as dependency, and rename the file to `godot-lib.release.aar`.

    godot-lib can be obtained from [godotengine.org](https://godotengine.org/download) or from your project's `android/build/libs/release` if Android build template is installed.

4. Build GDMP AAR using Android Studio or gradlew, copy the release variant AAR located in `android/GDMP/build/outputs/aar` to your project's `android/plugins` directory.
5. Run:

    ```
    build.py android
    ```
    to build android library, and copy them to your project's `addons/GDMP/libs/android/{ABI}` depending on your target ABIs.

    (Optional) also copy `libopencv_java3.so` to the project and add it as GDNative library dependencies if OpenCV is used in calculators.

6. Copy `plugins/GDMP.gdap` to your project's `android/plugins` directory.

    Files used by MediaPipe graphs (e.g. TFLite models) need to be placed in your project's directory according to the path provided by the calculator configs.

## Building for Linux
1. Place the calculator dependencies in `GDMP/variables.bzl`
2. Run:

    ```
    build.py desktop
    ```
    to build desktop library, generated file will be located in

    `mediapipe/bazel-bin/mediapipe/GDMP/libgdmp.so`
3. Copy libgdmp.so to your project's `addons/GDMP` directory.

    Files used by MediaPipe graphs (e.g. TFLite models) need to be placed in your project's directory according to the path provided by the calculator configs.

## Usage
1. Go to `Project -> Project settings -> Plugins` to enable GDMP.
2. To load a MediaPipe graph:

    ```gdscript
    GDMP.init_graph("res://path/to/your/graph.pbtxt")
    ```

3. To add a proto callback to the graph's output stream:

    ```gdscript
    GDMP.add_proto_callback("stream_name", target_object, "target_method")
    ```
4. To use the data from proto callback(NormalizedLandmarkList for example):

    ```gdscript
    func _on_new_landmarks(data : PoolByteArray) -> void:
        var landmarks = GDMP.NormalizedLandmarkList.new()
        landmarks.from_bytes(data)
        for i in range(landmarks.landmark_size()):
            var landmark = landmarks.get_landmark(i)
            print("x:%f, y:%f, z:%f" % [landmark.get_x(), landmark.get_y(), landmark.get_z()])
    ```
5. Start the graph and camera for sending video frames to the graph:

    ```gdscript
    GDMP.start_graph()
    GDMP.start_camera()
    ```
    The default is front camera for Android and index 0 for desktop Linux, you can pass the `CAMERA_FACING_FRONT`, `CAMERA_FACING_BACK` enum or `index` integer to specify the camera to use.

## Disclaimer
- The project is still much work in progress, expect usage API changes in future releases.
-  Contributions are welcome, if you are interested in bugfixes, adding features or support for other platforms (for now we lack support for Windows and iOS specifically)
