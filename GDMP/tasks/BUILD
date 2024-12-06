cc_library(
    name = "base_options",
    srcs = ["base_options.cc"],
    hdrs = ["base_options.h"],
    visibility = ["//visibility:public"],
    deps = [
        "@mediapipe//mediapipe/tasks/cc/core:base_options",
        "@godot-cpp//:godot",
    ],
)

cc_library(
    name = "task",
    srcs = ["task.cc"],
    hdrs = ["task.h"],
    visibility = ["//visibility:public"],
    deps = [
        ":base_options",
        "@mediapipe//mediapipe/tasks/cc/audio/core:running_mode",
        "@mediapipe//mediapipe/tasks/cc/vision/core:running_mode",
        "@godot-cpp//:godot",
    ],
)
