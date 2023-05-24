load("//mediapipe/framework/tool:mediapipe_files.bzl", "mediapipe_files")

mediapipe_files(
    srcs = [
        "face_detection_full_range.tflite",
        "face_detection_full_range_sparse.tflite",
        "face_detection_short_range.tflite",
        "face_landmarker.task",
        "face_landmarker_v2.task",
        "face_landmarker_v2_with_blendshapes.task",
    ],
)

filegroup(
    name = "face_detector_models",
    srcs = [
        "face_detection_full_range.tflite",
        "face_detection_full_range_sparse.tflite",
        "face_detection_short_range.tflite",
    ],
)

filegroup(
    name = "face_landmarker_models",
    srcs = [
        "face_landmarker.task",
        "face_landmarker_v2.task",
        "face_landmarker_v2_with_blendshapes.task",
    ],
)

cc_library(
    name = "face_detector",
    srcs = ["face_detector.cc"],
    hdrs = ["face_detector.h"],
    data = [":face_detector_models"],
    visibility = ["//visibility:public"],
    deps = [
        ":vision_task",
        "//GDMP/framework:image",
        "//GDMP/tasks:task",
        "//GDMP/tasks/containers:detection_result",
        "//mediapipe/tasks/cc/vision/face_detector",
        "@godot-cpp//:godot",
    ],
    alwayslink = 1,
)

cc_library(
    name = "face_landmarker",
    srcs = ["face_landmarker.cc"],
    hdrs = ["face_landmarker.h"],
    data = [":face_landmarker_models"],
    visibility = ["//visibility:public"],
    deps = [
        ":vision_task",
        "//GDMP/framework:image",
        "//GDMP/tasks:task",
        "//GDMP/tasks/containers:classification_result",
        "//GDMP/tasks/containers:landmark",
        "//mediapipe/tasks/cc/vision/face_landmarker",
        "@godot-cpp//:godot",
    ],
    alwayslink = 1,
)

cc_library(
    name = "vision_task",
    srcs = ["vision_task.cc"],
    hdrs = ["vision_task.h"],
    visibility = ["//visibility:public"],
    deps = [
        "//mediapipe/tasks/cc/vision/core:image_processing_options",
        "@godot-cpp//:godot",
    ],
)
