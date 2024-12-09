cc_library(
    name = "image",
    srcs = ["image.cc"],
    hdrs = ["image.h"],
    visibility = ["//visibility:public"],
    deps = [
        ":packet",
        "@mediapipe//mediapipe/framework/formats:image",
        "@godot-cpp//:godot",
    ] + select({
        "@mediapipe//mediapipe/gpu:disable_gpu": [],
        "//conditions:default": ["@mediapipe//mediapipe/gpu:gpu_buffer"],
    }),
)

cc_library(
    name = "graph",
    srcs = ["graph.cc"],
    hdrs = ["graph.h"],
    visibility = ["//visibility:public"],
    deps = [
        ":graph_config",
        ":packet",
        "//GDMP/gpu:gpu_resources",
        "@mediapipe//mediapipe/framework:calculator_framework",
        "@godot-cpp//:godot",
    ] + select({
        "@mediapipe//mediapipe/gpu:disable_gpu": [],
        "//conditions:default": ["@mediapipe//mediapipe/gpu:gpu_shared_data_internal"],
    }),
)

cc_library(
    name = "graph_builder",
    srcs = [
        "graph_builder.cc",
        "graph_node.cc",
    ],
    hdrs = [
        "graph_builder.h",
        "graph_node.h",
    ],
    visibility = ["//visibility:public"],
    deps = [
        ":graph_config",
        "//GDMP/proto",
        "@godot-cpp//:godot",
        "@mediapipe//mediapipe/framework/api2:builder",
    ],
)

cc_library(
    name = "graph_config",
    srcs = ["graph_config.cc"],
    hdrs = ["graph_config.h"],
    visibility = ["//visibility:public"],
    deps = [
        "//GDMP/proto",
        "@godot-cpp//:godot",
        "@mediapipe//mediapipe/framework:calculator_cc_proto",
    ],
)

cc_library(
    name = "packet",
    srcs = ["packet.cc"],
    hdrs = ["packet.h"],
    visibility = ["//visibility:public"],
    deps = [
        "//GDMP/proto",
        "@mediapipe//mediapipe/framework:packet",
        "@godot-cpp//:godot",
    ],
)
