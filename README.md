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
6. Place calculator dependencies in `GDMP/calculators.bzl`
7. Copy `addons/GDMP` to your project's directory.

## Building for Android
1. Refer to [Prerequisite](https://google.github.io/mediapipe/getting_started/android.html#prerequisite) section for Java and Android SDK & NDK setup.
2. Copy or symlink godot-lib to `android/GDMP/libs` directory as dependency, and rename the file to `godot-lib.release.aar`.

    godot-lib can be obtained from [godotengine.org](https://godotengine.org/download) or from your project's `android/build/libs/release` if Android build template is installed.

3. Build GDMP AAR using Android Studio or gradlew, copy the release variant AAR located in `android/GDMP/build/outputs/aar` to your project's `android/plugins` directory.
4. Copy `plugins/GDMP.gdap` to your project's `android/plugins` directory.
5. Run:

    ```
    build.py android
    ```
    to build android library, generated file will be located in `mediapipe/bazel-bin/mediapipe/GDMP`, copy the library to your project's `addons/GDMP/libs/android/{ABI}` depending on your target ABIs.

    (Optional) also copy `libopencv_java3.so` to the project and add it as GDNative library dependencies if OpenCV is used in calculators.

## Building for Linux
1. Run:

    ```
    build.py desktop
    ```
    to build desktop library, generated file will be located in `mediapipe/bazel-bin/mediapipe/GDMP`, copy the library to your project's `addons/GDMP/libs/linux/x86_64`.

## Usage
1. Go to `Project -> Project settings -> Plugins` to enable GDMP.
2. Place files used by MediaPipe in your project's directory, files used by graphs (e.g. TFLite models) need to be placed in your project's directory according to the path provided by the calculator configs.
3. To load a MediaPipe graph:

    ```gdscript
    var graph : GDMP.Graph = GDMP.Graph.new()
    # to load text graph
    graph.initialize("res://path/to/your/graph.pbtxt", true)
    # to load binary graph
    graph.initialize("res://path/to/your/graph.binarypb", false)
    ```

4. To add a packet callback to the graph's output stream:

    ```gdscript
    graph.add_packet_callback("stream_name", target_object, "target_method")
    ```
5. To use the packet from graph callback(NormalizedLandmarkList for example):

    ```gdscript
    func _on_new_landmarks(stream_name : String, packet) -> void:
        var bytes : PoolByteArray = packet.get_proto()
        var landmarks = GDMP.NormalizedLandmarkList.new()
        landmarks.from_bytes(bytes)
        for i in range(landmarks.landmark_size()):
            var landmark = landmarks.get_landmark(i)
            print("x:%f, y:%f, z:%f" % [landmark.get_x(), landmark.get_y(), landmark.get_z()])
    ```
6. Start the graph and camera for sending video frames to the graph:

    ```gdscript
    var camera_helper : GDMP.CameraHelper = GDMP.CameraHelper.new()
    camera_helper.set_graph(graph, "input_video")
    graph.start()
    if camera_helper.permission_granted():
        camera_helper.start(GDMP.CAMERA_FACING_FRONT, Vector2(640, 480))
    ```

## Disclaimer
- The project is still much work in progress, expect usage API changes in future releases.
-  Contributions are welcome, if you are interested in bugfixes, adding features or support for other platforms (for now we lack support for Windows and iOS specifically)
