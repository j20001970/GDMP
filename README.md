# GDMP
GDMP is a plugin for Godot 3.3+ that allows utilizing MediaPipe graphs in GDScript.

## Getting Started
1. Install [Bazelisk](https://docs.bazel.build/versions/main/install-bazelisk.html) or bazel version that meets MediaPipe requirement.
2. Refer to [Installation](https://google.github.io/mediapipe/getting_started/install.html) for OpenCV and FFmpeg setup.
3. Apply `mediapipe_setup.diff` to `mediapipe` directory.
4. (Optional) Apply `absl_update.diff` to `mediapipe` directory if you are using GCC 11 or higher.
5. Copy `addons/GDMP` to your project's directory.

## Building for Android
1. Refer to [Prerequisite](https://google.github.io/mediapipe/getting_started/android.html#prerequisite) section for Java and Android SDK & NDK setup.
2. Place the calculator dependencies and assets in `mediapipe_aar/java/com/google/mediapipe/BUILD`
3. Run:

    ```
    build.py aar
    ```
    to build mediapipe_aar, generated file will be located in

    `mediapipe/bazel-bin/mediapipe/mediapipe_aar/java/com/google/mediapipe/mediapipe_aar.aar`
4. Copy or link godot-lib and mediapipe_aar AAR to `android/GDMP/libs` directory.
5. Build GDMP AAR, copy generated file to your project's `android/plugins` directory, along with mediapipe_aar from step 3.
6. Copy `plugins/GDMP.gdap` to your project's `android/plugins` directory.

## Building for Linux
1. Refer to [Building the C++ bindings](https://docs.godotengine.org/en/stable/tutorials/plugins/gdnative/gdnative-cpp-example.html#building-the-c-bindings) section for building Godot C++ bindings in `godot-cpp` directory.
2. Place the calculator dependencies in `desktop/BUILD`
3. Run:

    ```
    build.py desktop
    ```
    to build desktop library, generated file will be located in

    `mediapipe/bazel-bin/mediapipe/desktop/libgdmp.so`
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
3. To add a callback to the graph's output stream:

    ```gdscript
    GDMP.add_packet_callback("stream_name", is_packet_vector, target_object, "target_method")
    ```
4. To start the camera for sending video frames to the graph:

    ```gdscript
    GDMP.start_camera()
    ```
    The default is front camera for Android and index 0 for desktop Linux, you can pass the `CAMERA_FACING_FRONT`, `CAMERA_FACING_BACK` enum or `index` integer to specify the camera to use.

## Disclaimer
The project is still much work in progress, as there are many limitations and hardcoded behaviors that need to be ironed out, we released it in the hope that someone could find the project useful, even if it's far from perfect. Contributions are welcome, if you are interested in bugfixes, adding features or support for other platforms (for now we lack support for Windows and iOS specifically)
