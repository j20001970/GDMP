package(default_visibility = ["//visibility:public"])

cc_library(
    name = "image",
    srcs = ["image.cc"],
    hdrs = ["image.h"],
    deps = [
        ":packet",
        "//GDMP/util:image",
        "@godot-cpp//:godot",
        "@mediapipe//mediapipe/framework/formats:image",
    ] + select({
        "@mediapipe//mediapipe/gpu:disable_gpu": [],
        "//conditions:default": ["@mediapipe//mediapipe/gpu:gpu_buffer"],
    }),
)

cc_library(
    name = "graph",
    srcs = ["graph.cc"],
    hdrs = ["graph.h"],
    deps = [
        ":graph_config",
        ":packet",
        "//GDMP/gpu:gpu_resources",
        "//GDMP/util:packet_map",
        "@godot-cpp//:godot",
        "@mediapipe//mediapipe/framework:calculator_framework",
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
        "node_port.cc",
    ],
    hdrs = [
        "graph_builder.h",
        "graph_node.h",
    ],
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
    deps = [
        "//GDMP/util:image",
        "@godot-cpp//:godot",
        "@mediapipe//mediapipe/framework:packet",
    ],
)
