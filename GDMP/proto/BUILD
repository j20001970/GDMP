cc_library(
    name = "classification",
    srcs = [
        "classification.h",
        "macros.h",
    ],
    hdrs = ["classification.h"],
    deps = [
        "//mediapipe/framework/formats:classification_cc_proto",
        "//third_party:godot",
    ],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "detection",
    srcs = [
        "detection.h",
        "macros.h",
    ],
    hdrs = ["detection.h"],
    deps = [
        "//mediapipe/framework/formats:detection_cc_proto",
        ":location_data",
        "//third_party:godot",
    ],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "landmark",
    srcs = [
        "landmark.h",
        "macros.h",
    ],
    hdrs = ["landmark.h"],
    deps = [
        "//mediapipe/framework/formats:landmark_cc_proto",
        "//third_party:godot",
    ],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "location_data",
    srcs = [
        "location_data.h",
        "macros.h",
    ],
    hdrs = ["location_data.h"],
    deps = [
        "//mediapipe/framework/formats:location_data_cc_proto",
        ":rasterization",
        "//third_party:godot",
    ],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "rasterization",
    srcs = [
        "rasterization.h",
        "macros.h",
    ],
    hdrs = ["rasterization.h"],
    deps = [
        "//mediapipe/framework/formats/annotation:rasterization_cc_proto",
        "//third_party:godot",
    ],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "rect",
    srcs = [
        "rect.h",
        "macros.h",
    ],
    hdrs = ["rect.h"],
    deps = [
        "//mediapipe/framework/formats:rect_cc_proto",
        "//third_party:godot",
    ],
    visibility = ["//visibility:public"],
)
