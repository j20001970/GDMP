package(default_visibility = ["//visibility:public"])

cc_library(
    name = "face_detector",
    srcs = ["face_detector.cc"],
    hdrs = ["face_detector.h"],
    deps = [
        ":vision_task",
        "//GDMP/framework:image",
        "//GDMP/tasks/containers:detection_result",
        "@godot-cpp//:godot",
        "@mediapipe//mediapipe/tasks/cc/vision/face_detector",
    ],
    alwayslink = 1,
)

cc_library(
    name = "face_landmarker",
    srcs = ["face_landmarker.cc"],
    hdrs = ["face_landmarker.h"],
    deps = [
        ":vision_task",
        "//GDMP/framework:image",
        "//GDMP/tasks/containers:classification_result",
        "//GDMP/tasks/containers:landmark",
        "@godot-cpp//:godot",
        "@mediapipe//mediapipe/tasks/cc/vision/face_landmarker",
    ],
    alwayslink = 1,
)

cc_library(
    name = "face_stylizer",
    srcs = ["face_stylizer.cc"],
    hdrs = ["face_stylizer.h"],
    deps = [
        ":vision_task",
        "//GDMP/framework:image",
        "@godot-cpp//:godot",
        "@mediapipe//mediapipe/tasks/cc/vision/face_stylizer",
    ],
    alwayslink = 1,
)

cc_library(
    name = "gesture_recognizer",
    srcs = ["gesture_recognizer.cc"],
    hdrs = ["gesture_recognizer.h"],
    deps = [
        ":vision_task",
        "//GDMP/framework:image",
        "//GDMP/tasks/containers:classification_result",
        "//GDMP/tasks/containers:landmark",
        "@godot-cpp//:godot",
        "@mediapipe//mediapipe/tasks/cc/vision/gesture_recognizer",
    ],
    alwayslink = 1,
)

cc_library(
    name = "hand_landmarker",
    srcs = ["hand_landmarker.cc"],
    hdrs = ["hand_landmarker.h"],
    deps = [
        ":vision_task",
        "//GDMP/framework:image",
        "//GDMP/tasks/containers:classification_result",
        "//GDMP/tasks/containers:landmark",
        "@godot-cpp//:godot",
        "@mediapipe//mediapipe/tasks/cc/vision/hand_landmarker",
    ],
    alwayslink = 1,
)

cc_library(
    name = "holistic_landmarker",
    deps = [
        "@mediapipe//mediapipe/tasks/cc/vision/holistic_landmarker:holistic_landmarker_graph",
    ],
    alwayslink = 1,
)

cc_library(
    name = "image_classifier",
    srcs = ["image_classifier.cc"],
    hdrs = ["image_classifier.h"],
    deps = [
        ":vision_task",
        "//GDMP/framework:image",
        "//GDMP/tasks/containers:classification_result",
        "//GDMP/tasks/processors:classifier_options",
        "@godot-cpp//:godot",
        "@mediapipe//mediapipe/tasks/cc/vision/image_classifier",
    ],
    alwayslink = 1,
)

cc_library(
    name = "image_embedder",
    srcs = ["image_embedder.cc"],
    hdrs = ["image_embedder.h"],
    deps = [
        ":vision_task",
        "//GDMP/framework:image",
        "//GDMP/tasks/containers:embedding_result",
        "//GDMP/tasks/processors:embedder_options",
        "@godot-cpp//:godot",
        "@mediapipe//mediapipe/tasks/cc/vision/image_embedder",
    ],
    alwayslink = 1,
)

cc_library(
    name = "image_generator",
    srcs = ["image_generator.cc"],
    hdrs = ["image_generator.h"],
    visibility = ["//visibility:public"],
    deps = [
        ":vision_task",
        "//GDMP/framework:image",
        "@mediapipe//mediapipe/tasks/cc/vision/image_generator",
        "@godot-cpp//:godot",
    ],
    alwayslink = 1,
)

cc_library(
    name = "image_segmenter",
    srcs = ["image_segmenter.cc"],
    hdrs = ["image_segmenter.h"],
    deps = [
        ":vision_task",
        "//GDMP/framework:image",
        "@godot-cpp//:godot",
        "@mediapipe//mediapipe/tasks/cc/vision/image_segmenter",
    ],
    alwayslink = 1,
)

cc_library(
    name = "interactive_segmenter",
    srcs = ["interactive_segmenter.cc"],
    hdrs = ["interactive_segmenter.h"],
    deps = [
        ":image_segmenter",
        ":vision_task",
        "//GDMP/framework:image",
        "@godot-cpp//:godot",
        "@mediapipe//mediapipe/tasks/cc/vision/interactive_segmenter",
    ],
    alwayslink = 1,
)

cc_library(
    name = "object_detector",
    srcs = ["object_detector.cc"],
    hdrs = ["object_detector.h"],
    deps = [
        ":vision_task",
        "//GDMP/framework:image",
        "//GDMP/tasks/containers:detection_result",
        "//GDMP/tasks/processors:classifier_options",
        "@godot-cpp//:godot",
        "@mediapipe//mediapipe/tasks/cc/vision/object_detector",
    ],
    alwayslink = 1,
)

cc_library(
    name = "pose_landmarker",
    srcs = ["pose_landmarker.cc"],
    hdrs = ["pose_landmarker.h"],
    deps = [
        ":vision_task",
        "//GDMP/framework:image",
        "//GDMP/tasks/containers:landmark",
        "@godot-cpp//:godot",
        "@mediapipe//mediapipe/tasks/cc/vision/pose_landmarker",
    ],
    alwayslink = 1,
)

cc_library(
    name = "vision_task",
    srcs = ["vision_task.cc"],
    hdrs = ["vision_task.h"],
    deps = [
        "//GDMP/tasks:task",
        "@godot-cpp//:godot",
        "@mediapipe//mediapipe/tasks/cc/vision/core:image_processing_options",
        "@mediapipe//mediapipe/tasks/cc/vision/core:running_mode",
    ],
)
