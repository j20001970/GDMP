cc_library(
    name = "image",
    hdrs = ["image.h"],
    visibility = ["//visibility:public"],
    deps = [
        "//mediapipe/framework/formats:image_frame",
        "@godot-cpp//:godot",
    ],
)

cc_library(
    name = "resource_util_godot",
    srcs = ["resource_util_godot.cc"],
    visibility = ["//visibility:public"],
    deps = [
        "//mediapipe/framework/port:status",
        "//mediapipe/framework/port:statusor",
        "@com_google_absl//absl/strings",
        "@godot-cpp//:godot",
    ] + select({
        "//mediapipe:android": [
            "//mediapipe/util/android/file/base",
        ],
        "//conditions:default": [],
    }),
)
