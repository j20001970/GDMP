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
    name = "graph_config",
    srcs = ["graph_config.cc"],
    hdrs = ["graph_config.h"],
    visibility = ["//visibility:public"],
    deps = [
        "@mediapipe//mediapipe/framework:calculator_cc_proto",
        "@mediapipe//mediapipe/framework/port:parse_text_proto",
        "@godot-cpp//:godot",
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
