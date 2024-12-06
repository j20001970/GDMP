cc_library(
    name = "category",
    srcs = ["category.cc"],
    hdrs = ["category.h"],
    visibility = ["//visibility:public"],
    deps = [
        "@mediapipe//mediapipe/tasks/cc/components/containers:category",
        "@godot-cpp//:godot",
    ],
)

cc_library(
    name = "classification_result",
    srcs = ["classification_result.cc"],
    hdrs = ["classification_result.h"],
    visibility = ["//visibility:public"],
    deps = [
        ":category",
        "@mediapipe//mediapipe/tasks/cc/components/containers:classification_result",
        "@godot-cpp//:godot",
    ],
)

cc_library(
    name = "detection_result",
    srcs = ["detection_result.cc"],
    hdrs = ["detection_result.h"],
    visibility = ["//visibility:public"],
    deps = [
        ":category",
        ":keypoint",
        "@mediapipe//mediapipe/tasks/cc/components/containers:detection_result",
        "@godot-cpp//:godot",
    ],
)

cc_library(
    name = "embedding_result",
    srcs = ["embedding_result.cc"],
    hdrs = ["embedding_result.h"],
    visibility = ["//visibility:public"],
    deps = [
        "@mediapipe//mediapipe/tasks/cc/components/containers:embedding_result",
        "@godot-cpp//:godot",
    ],
)

cc_library(
    name = "keypoint",
    srcs = ["keypoint.cc"],
    hdrs = ["keypoint.h"],
    visibility = ["//visibility:public"],
    deps = [
        "@mediapipe//mediapipe/tasks/cc/components/containers:keypoint",
        "@godot-cpp//:godot",
    ],
)

cc_library(
    name = "landmark",
    srcs = ["landmark.cc"],
    hdrs = ["landmark.h"],
    visibility = ["//visibility:public"],
    deps = [
        "@mediapipe//mediapipe/tasks/cc/components/containers:landmark",
        "@godot-cpp//:godot",
    ],
)
