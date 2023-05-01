cc_library(
    name = "classification",
    srcs = [
        "classification.h",
        "macros.h",
    ],
    hdrs = ["classification.h"],
    visibility = ["//visibility:public"],
    deps = [
        "//mediapipe/framework/formats:classification_cc_proto",
        "@godot-cpp//:godot",
    ],
)

cc_library(
    name = "detection",
    srcs = [
        "detection.h",
        "macros.h",
    ],
    hdrs = ["detection.h"],
    visibility = ["//visibility:public"],
    deps = [
        ":location_data",
        "//mediapipe/framework/formats:detection_cc_proto",
        "@godot-cpp//:godot",
    ],
)

cc_library(
    name = "landmark",
    srcs = [
        "landmark.h",
        "macros.h",
    ],
    hdrs = ["landmark.h"],
    visibility = ["//visibility:public"],
    deps = [
        "//mediapipe/framework/formats:landmark_cc_proto",
        "@godot-cpp//:godot",
    ],
)

cc_library(
    name = "location_data",
    srcs = [
        "location_data.h",
        "macros.h",
    ],
    hdrs = ["location_data.h"],
    visibility = ["//visibility:public"],
    deps = [
        ":rasterization",
        "//mediapipe/framework/formats:location_data_cc_proto",
        "@godot-cpp//:godot",
    ],
)

cc_library(
    name = "rasterization",
    srcs = [
        "macros.h",
        "rasterization.h",
    ],
    hdrs = ["rasterization.h"],
    visibility = ["//visibility:public"],
    deps = [
        "//mediapipe/framework/formats/annotation:rasterization_cc_proto",
        "@godot-cpp//:godot",
    ],
)

cc_library(
    name = "rect",
    srcs = [
        "macros.h",
        "rect.h",
    ],
    hdrs = ["rect.h"],
    visibility = ["//visibility:public"],
    deps = [
        "//mediapipe/framework/formats:rect_cc_proto",
        "@godot-cpp//:godot",
    ],
)

cc_library(
    name = "proto",
    srcs = [
        "proto.cc",
        "util.cc"
    ],
    hdrs = [
        "proto.h",
        "util.h",
    ],
    visibility = ["//visibility:public"],
    deps = [
        "@com_google_protobuf//:protobuf",
        "@godot-cpp//:godot",
    ],
)
