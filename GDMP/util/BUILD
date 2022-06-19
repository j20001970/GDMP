cc_library(
    name = "image",
    hdrs = ["image.h"],
    visibility = ["//visibility:public"],
    deps = [
        "//mediapipe/framework/formats:image_frame",
        "//third_party:godot",
    ],
)

cc_library(
    name = "resource_util_godot",
    srcs = ["resource_util_godot.cc"],
    visibility = ["//visibility:public"],
    deps = [
        "//mediapipe/framework/port:status",
        "//mediapipe/framework/port:statusor",
        "//third_party:godot",
        "@com_google_absl//absl/strings",
    ] + select({
        "//mediapipe:android": [
            "//mediapipe/util/android/file/base",
        ],
        "//conditions:default": [],
    }),
)
