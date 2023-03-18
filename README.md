# GDMP
GDMP is a plugin for Godot 3.3+ that allows utilizing MediaPipe graphs in GDScript.

## Getting Started
1. Install [Bazelisk](https://docs.bazel.build/versions/main/install-bazelisk.html) or bazel version that meets MediaPipe requirement.
2. Refer to [Installation](https://google.github.io/mediapipe/getting_started/install.html) for OpenCV and FFmpeg setup.
3. Locate your Godot executable and run `godot --dump-extension-api` to get a copy of extension_api.json for current Godot version,
    then go to `godot-cpp` directory and run:

    ```
    python -c "from binding_generator import *; generate_bindings('/path/to/your/extension_api.json', True)"
    ```
    to generate Godot C++ bindings directly, replacing `/path/to/your/api.json` with the path to your api.json
4. Run `setup.py`, the script will apply necessary changes, setup source code and external dependencies in mediapipe workspace.
5. Place calculator dependencies in `GDMP/calculators.bzl`
6. Copy `addons/GDMP` to your project's directory.

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

## Building for iOS
1. Comment out `macos` and `ios` part of select() in `srcs` and `deps` attributes from `resource_util` in `mediapipe/mediapipe/util/BUILD`, this step is required before the ambiguous match problem is solved.
2. Use [Tulsi](https://tulsi.bazel.build) project located in `GDMP` directory to generate Xcode project.
3. Build `GDMP_ios` static framework target, copy the library from the framework to your project's `addons/GDMP/libs/ios` **and rename it with .a extension**, so that Godot will treat it like static library instead of dynamic when exporting to Xcode.

    (Optional) also copy `opencv2.a` to the project and add it as GDNative library dependencies if OpenCV is used in calculators.
4. After exporting Godot project to Xcode, add necessary frameworks in order to build the app, and go to `Other Linker Flags` in `Build Settings` to force load the static library, for example:

    `-force_load $(PROJECT_DIR)/path/to/your/GDMP_ios.a`

## Building for Linux
1. Run:

    ```
    build.py desktop
    ```
    to build desktop library, generated file will be located in `mediapipe/bazel-bin/mediapipe/GDMP`, copy the library to your project's `addons/GDMP/libs/linux/x86_64`

## Building for Windows
1. Install [MSYS2](https://www.msys2.org) and add `<msys2 path>\usr\bin` to your `PATH` environment variable.
2. Add `PYTHON_BIN_PATH` environment variable that point to your python.exe.
3. Check that MSVC compiler and Windows SDK is installed, and refer to [Bazel documentation](https://docs.bazel.build/versions/main/windows.html#build-c-with-msvc) for setting Bazel related environment variables.
4. Run:

    ```
    build.py desktop
    ```
    to build desktop library, generated file will be located in `mediapipe/bazel-bin/mediapipe/GDMP`, copy the library to your project's `addons/GDMP/libs/windows/x86_64`

    (Optional) also copy `opencv_world3410.dll` to the project and add it as GDNative library dependencies if OpenCV is used in calculators.

## Usage
1. Go to `Project -> Project settings -> Plugins` to enable GDMP.
2. Place files used by MediaPipe in your project's directory, files used by graphs (e.g. TFLite models) need to be placed in your project's directory according to the path provided by the calculator configs.
3. To load a MediaPipe graph:

    ```gdscript
    # load graph config in text or binary format
    var config : MediaPipeGraphConfig = load("res://path/to/your/graph.pbtxt")
    # initialize mediapipe graph with graph config
    var graph : MediaPipeGraph = MediaPipeGraph.new()
    graph.initialize(config)
    ```

4. To add a packet callback to the graph's output stream:

    ```gdscript
    graph.add_packet_callback("stream_name", target_object.target_method)
    ```
5. To use the packet from graph callback(NormalizedLandmarkList for example):

    ```gdscript
    func _on_new_landmarks(stream_name : String, packet) -> void:
        var bytes : PackedByteArray = packet.get_proto()
        var landmarks = NormalizedLandmarkList.new()
        landmarks.from_bytes(bytes)
        for i in range(landmarks.landmark_size()):
            var landmark = landmarks.get_landmark(i)
            print("x:%f, y:%f, z:%f" % [landmark.get_x(), landmark.get_y(), landmark.get_z()])
    ```
6. Start the graph and camera for sending video frames to the graph:

    ```gdscript
    var camera_helper : MediaPipeCameraHelper = MediaPipeCameraHelper.new()
    camera_helper.set_graph(graph, "input_video")
    graph.start()
    if camera_helper.permission_granted():
        camera_helper.start(GDMP.CAMERA_FACING_FRONT, Vector2(640, 480))
    else:
        camera_helper.connect("permission_result", self._on_permission_result)
        camera_helper.request_permission()
    ```
To handle camera permission result callback:

    ```gdscript
    func _on_permission_result(granted : bool):
        if granted:
            print("permission granted")
            camera_helper.start(GDMP.CAMERA_FACING_FRONT, Vector2(640, 480))
        else:
            print("permission denied")
    ```

## Disclaimer
The project is still much work in progress, expect usage API changes in future releases.
