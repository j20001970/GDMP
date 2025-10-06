package(default_visibility = ["//visibility:public"])

cc_library(
    name = "image",
    srcs = ["image.cc"],
    hdrs = ["image.h"],
    deps = [
        "@godot-cpp//:godot",
        "@mediapipe//mediapipe/framework/formats:image",
    ],
)

cc_library(
    name = "packet_map",
    srcs = ["packet_map.cc"],
    hdrs = ["packet_map.h"],
    deps = [
        "//GDMP/framework:packet",
        "@godot-cpp//:godot",
        "@mediapipe//mediapipe/tasks/cc/core:task_runner",
    ],
)

cc_library(
    name = "resource_provider",
    srcs = ["resource_provider.cc"],
    hdrs = ["resource_provider.h"],
    deps = [
        "@godot-cpp//:godot",
        "@mediapipe//mediapipe/framework/port:status",
        "@mediapipe//mediapipe/util:resource_util",
        "@mediapipe//mediapipe/util:resource_util_custom",
    ],
)
