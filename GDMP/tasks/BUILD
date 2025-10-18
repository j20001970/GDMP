package(default_visibility = ["//visibility:public"])

cc_library(
    name = "task",
    srcs = ["task.cc"],
    hdrs = ["task.h"],
    deps = [
        "@godot-cpp//:godot",
        "@mediapipe//mediapipe/tasks/cc/core:base_options",
    ],
)

cc_library(
    name = "task_runner",
    srcs = ["task_runner.cc"],
    hdrs = ["task_runner.h"],
    deps = [
        "//GDMP/framework:graph_config",
        "//GDMP/framework:packet",
        "//GDMP/gpu:gpu_resources",
        "//GDMP/util:packet_map",
        "@godot-cpp//:godot",
        "@mediapipe//mediapipe/tasks/cc/core:base_options",
        "@mediapipe//mediapipe/tasks/cc/core:task_runner",
    ],
)
