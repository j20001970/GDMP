cc_library(
    name = "graph",
    srcs = ["graph.cc"],
    hdrs = ["graph.h"],
    visibility = ["//visibility:public"],
    deps = [
        ":graph_config",
        ":packet",
        "//mediapipe/framework:calculator_graph",
        "//mediapipe/framework:packet",
        "//mediapipe/framework/port:status",
        "//mediapipe/framework/tool:sink",
        "@godot-cpp//:godot",
    ] + select({
        "//mediapipe/gpu:disable_gpu": [],
        "//conditions:default": ["//mediapipe/gpu:gpu_shared_data_internal"],
    }),
)

cc_library(
    name = "graph_config",
    srcs = ["graph_config.cc"],
    hdrs = ["graph_config.h"],
    visibility = ["//visibility:public"],
    deps = [
        "//mediapipe/framework:calculator_cc_proto",
        "//mediapipe/framework/port:parse_text_proto",
        "@godot-cpp//:godot",
    ],
)

cc_library(
    name = "gpu_helper",
    srcs = ["gpu_helper.cc"],
    hdrs = ["gpu_helper.h"],
    visibility = ["//visibility:public"],
    deps = [
        "//mediapipe/GDMP/framework:graph",
        "//mediapipe/GDMP/framework:packet",
        "//mediapipe/GDMP/util:image",
        "//mediapipe/framework/formats:image_frame",
        "//mediapipe/gpu:gl_calculator_helper",
        "//mediapipe/gpu:gpu_buffer",
        "//mediapipe/gpu:gpu_shared_data_internal",
        "@godot-cpp//:godot",
    ] + select({
        "//conditions:default": [],
        "//mediapipe:apple": ["//mediapipe/objc:util"],
    }),
)

cc_library(
    name = "packet",
    srcs = ["packet.cc"],
    hdrs = ["packet.h"],
    visibility = ["//visibility:public"],
    deps = [
        "//mediapipe/GDMP/util:image",
        "//mediapipe/framework:packet",
        "//mediapipe/framework/formats:image_frame",
        "@godot-cpp//:godot",
    ],
)
