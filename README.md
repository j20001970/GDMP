# GDMP
GDMP is a plugin for Godot 3.3+ that allows utilizing MediaPipe graphs in GDScript.

## Getting Started
1. Install [Bazelisk](https://docs.bazel.build/versions/main/install-bazelisk.html) or bazel version that meets MediaPipe requirement.
2. Refer to [Installation](https://google.github.io/mediapipe/getting_started/install.html) for OpenCV and FFmpeg setup.
3. Apply `mediapipe_setup.diff` to `mediapipe` directory.
4. Copy `addons/GDMP` to your project's directory.

## Building for Android
1. Refer to [Prerequisite](https://google.github.io/mediapipe/getting_started/android.html#prerequisite) section for Java and Android SDK & NDK setup.
2. Place the calculator dependencies in `GDMP/variables.bzl` and assets in `GDMP/mediapipe_aar/java/com/google/mediapipe/BUILD`
3. Run:

    ```
    build.py aar
    ```
    to build mediapipe_aar, generated file will be located in

    `mediapipe/bazel-bin/mediapipe/GDMP/mediapipe_aar/java/com/google/mediapipe/mediapipe_aar.aar`
4. Copy or link godot-lib and mediapipe_aar AAR to `android/GDMP/libs` directory.
5. Run:

    ```
    build.py android
    ```
    to build android library, and copy them to `android/GDMP/src/main/jniLibs/{ABI}` depending on your target ABIs.
6. Build GDMP AAR, copy generated file to your project's `android/plugins` directory, along with mediapipe_aar from step 3.
7. Copy `plugins/GDMP.gdap` to your project's `android/plugins` directory.

    Note that Android export needs to exclude `res://addons/GDMP/GDMP.gdnlib` which is intended for desktop usage, otherwise it will fail to load GDNative library that provided by GDMP aar.

    You can go to `Project -> Export...`, select your Android preset, and in `Resources` tab fill `addons/GDMP/*.gdnlib` in filter to exclude files from project.

## Building for Linux
1. Refer to [Building the C++ bindings](https://docs.godotengine.org/en/stable/tutorials/plugins/gdnative/gdnative-cpp-example.html#building-the-c-bindings) section for building Godot C++ bindings in `godot-cpp` directory.
2. Place the calculator dependencies in `GDMP/variables.bzl`
3. Run:

    ```
    build.py desktop
    ```
    to build desktop library, generated file will be located in

    `mediapipe/bazel-bin/mediapipe/GDMP/libgdmp.so`
4. Copy libgdmp.so to your project's `addons/GDMP` directory.

## Usage
1. Go to `Project -> Project settings -> Plugins` to enable GDMP.
2. To load a MediaPipe graph:

    ```gdscript
    match OS.get_name():
        "Android":
            GDMP.init_graph("name_of_your_graph.binarypb")
        "X11":
            GDMP.init_graph("path/to/your/graph.pbtxt")
    ```
    Note that for desktop Linux, files used by MediaPipe need to be placed externally (e.g. place the models in your project's directory according to the calculator's model_path, and export graphs and models outside of .pck file)
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
