cc_library(
    name = "packet_map",
    srcs = ["packet_map.cc"],
    hdrs = ["packet_map.h"],
    visibility = ["//visibility:public"],
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
    visibility = ["//visibility:public"],
    deps = [
        "@mediapipe//mediapipe/framework/port:status",
        "@mediapipe//mediapipe/util:resource_util",
        "@mediapipe//mediapipe/util:resource_util_custom",
        "@godot-cpp//:godot",
    ],
)
