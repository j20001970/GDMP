package(default_visibility = ["//visibility:public"])

cc_library(
    name = "category",
    srcs = ["category.cc"],
    hdrs = ["category.h"],
    deps = [
        "//GDMP/proto",
        "@godot-cpp//:godot",
        "@mediapipe//mediapipe/tasks/cc/components/containers:category",
    ],
)

cc_library(
    name = "classification_result",
    srcs = ["classification_result.cc"],
    hdrs = ["classification_result.h"],
    deps = [
        ":category",
        "//GDMP/framework:packet",
        "//GDMP/proto",
        "@godot-cpp//:godot",
        "@mediapipe//mediapipe/tasks/cc/components/containers:classification_result",
    ],
)

cc_library(
    name = "detection_result",
    srcs = ["detection_result.cc"],
    hdrs = ["detection_result.h"],
    deps = [
        ":category",
        ":keypoint",
        "//GDMP/framework:packet",
        "//GDMP/proto",
        "@godot-cpp//:godot",
        "@mediapipe//mediapipe/tasks/cc/components/containers:detection_result",
    ],
)

cc_library(
    name = "embedding_result",
    srcs = ["embedding_result.cc"],
    hdrs = ["embedding_result.h"],
    deps = [
        "@godot-cpp//:godot",
        "@mediapipe//mediapipe/tasks/cc/components/containers:embedding_result",
    ],
)

cc_library(
    name = "keypoint",
    srcs = ["keypoint.cc"],
    hdrs = ["keypoint.h"],
    deps = [
        "//GDMP/proto",
        "@godot-cpp//:godot",
        "@mediapipe//mediapipe/framework/formats:location_data_cc_proto",
        "@mediapipe//mediapipe/tasks/cc/components/containers:keypoint",
    ],
)

cc_library(
    name = "landmark",
    srcs = ["landmark.cc"],
    hdrs = ["landmark.h"],
    deps = [
        "//GDMP/framework:packet",
        "//GDMP/proto",
        "@godot-cpp//:godot",
        "@mediapipe//mediapipe/tasks/cc/components/containers:landmark",
    ],
)
