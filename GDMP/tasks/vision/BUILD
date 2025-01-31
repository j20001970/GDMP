cc_library(
    name = "face_detector",
    srcs = ["face_detector.cc"],
    hdrs = ["face_detector.h"],
    visibility = ["//visibility:public"],
    deps = [
        ":vision_task",
        "//GDMP/framework:image",
        "//GDMP/tasks:task",
        "//GDMP/tasks/containers:detection_result",
        "@mediapipe//mediapipe/tasks/cc/vision/face_detector",
        "@godot-cpp//:godot",
    ],
    alwayslink = 1,
)

cc_library(
    name = "face_landmarker",
    srcs = ["face_landmarker.cc"],
    hdrs = ["face_landmarker.h"],
    visibility = ["//visibility:public"],
    deps = [
        ":vision_task",
        "//GDMP/framework:image",
        "//GDMP/tasks:task",
        "//GDMP/tasks/containers:classification_result",
        "//GDMP/tasks/containers:landmark",
        "@mediapipe//mediapipe/tasks/cc/vision/face_landmarker",
        "@godot-cpp//:godot",
    ],
    alwayslink = 1,
)

cc_library(
    name = "face_stylizer",
    srcs = ["face_stylizer.cc"],
    hdrs = ["face_stylizer.h"],
    visibility = ["//visibility:public"],
    deps = [
        ":vision_task",
        "//GDMP/framework:image",
        "//GDMP/tasks:task",
        "@mediapipe//mediapipe/tasks/cc/vision/face_stylizer",
        "@godot-cpp//:godot",
    ],
    alwayslink = 1,
)

cc_library(
    name = "gesture_recognizer",
    srcs = ["gesture_recognizer.cc"],
    hdrs = ["gesture_recognizer.h"],
    visibility = ["//visibility:public"],
    deps = [
        ":vision_task",
        "//GDMP/framework:image",
        "//GDMP/proto",
        "//GDMP/tasks:task",
        "@mediapipe//mediapipe/tasks/cc/vision/gesture_recognizer",
        "@godot-cpp//:godot",
    ],
    alwayslink = 1,
)

cc_library(
    name = "hand_landmarker",
    srcs = ["hand_landmarker.cc"],
    hdrs = ["hand_landmarker.h"],
    visibility = ["//visibility:public"],
    deps = [
        ":vision_task",
        "//GDMP/framework:image",
        "//GDMP/tasks:task",
        "//GDMP/tasks/containers:classification_result",
        "//GDMP/tasks/containers:landmark",
        "@mediapipe//mediapipe/tasks/cc/vision/hand_landmarker",
        "@godot-cpp//:godot",
    ],
    alwayslink = 1,
)

cc_library(
    name = "holistic_landmarker",
    visibility = ["//visibility:public"],
    deps = [
        "@mediapipe//mediapipe/tasks/cc/vision/holistic_landmarker:holistic_landmarker_graph",
    ],
    alwayslink = 1,
)

cc_library(
    name = "image_classifier",
    srcs = ["image_classifier.cc"],
    hdrs = ["image_classifier.h"],
    visibility = ["//visibility:public"],
    deps = [
        ":vision_task",
        "//GDMP/framework:image",
        "//GDMP/tasks:task",
        "//GDMP/tasks/containers:classification_result",
        "@mediapipe//mediapipe/tasks/cc/vision/image_classifier",
        "@godot-cpp//:godot",
    ],
    alwayslink = 1,
)

cc_library(
    name = "image_embedder",
    srcs = ["image_embedder.cc"],
    hdrs = ["image_embedder.h"],
    visibility = ["//visibility:public"],
    deps = [
        ":vision_task",
        "//GDMP/framework:image",
        "//GDMP/tasks:task",
        "//GDMP/tasks/containers:embedding_result",
        "@mediapipe//mediapipe/tasks/cc/vision/image_embedder",
        "@godot-cpp//:godot",
    ],
    alwayslink = 1,
)

cc_library(
    name = "image_segmenter",
    srcs = ["image_segmenter.cc"],
    hdrs = ["image_segmenter.h"],
    visibility = ["//visibility:public"],
    deps = [
        ":vision_task",
        "//GDMP/framework:image",
        "//GDMP/tasks:task",
        "@mediapipe//mediapipe/tasks/cc/vision/image_segmenter",
        "@godot-cpp//:godot",
    ],
    alwayslink = 1,
)

cc_library(
    name = "interactive_segmenter",
    srcs = ["interactive_segmenter.cc"],
    hdrs = ["interactive_segmenter.h"],
    visibility = ["//visibility:public"],
    deps = [
        ":image_segmenter",
        ":vision_task",
        "//GDMP/framework:image",
        "//GDMP/tasks:task",
        "@mediapipe//mediapipe/tasks/cc/vision/interactive_segmenter",
        "@godot-cpp//:godot",
    ],
    alwayslink = 1,
)

cc_library(
    name = "object_detector",
    srcs = ["object_detector.cc"],
    hdrs = ["object_detector.h"],
    visibility = ["//visibility:public"],
    deps = [
        ":vision_task",
        "//GDMP/framework:image",
        "//GDMP/tasks:task",
        "//GDMP/tasks/containers:detection_result",
        "@mediapipe//mediapipe/tasks/cc/vision/object_detector",
        "@godot-cpp//:godot",
    ],
    alwayslink = 1,
)

cc_library(
    name = "pose_landmarker",
    srcs = ["pose_landmarker.cc"],
    hdrs = ["pose_landmarker.h"],
    visibility = ["//visibility:public"],
    deps = [
        ":vision_task",
        "//GDMP/framework:image",
        "//GDMP/tasks:task",
        "//GDMP/tasks/containers:landmark",
        "@mediapipe//mediapipe/tasks/cc/vision/pose_landmarker",
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
        "@mediapipe//mediapipe/tasks/cc/vision/core:image_processing_options",
        "@godot-cpp//:godot",
    ],
)
