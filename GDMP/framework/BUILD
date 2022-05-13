cc_library(
    name = "graph",
    srcs = ["graph.cc"],
    hdrs = ["graph.h"],
    visibility = ["//visibility:public"],
    deps = [
        ":packet",
        "//mediapipe/framework:calculator_graph",
        "//mediapipe/framework:packet",
        "//mediapipe/framework/port:parse_text_proto",
        "//mediapipe/framework/port:status",
        "//mediapipe/util:resource_util",
        "//third_party:godot",
    ] + select({
        "//mediapipe/gpu:disable_gpu": [],
        "//conditions:default": ["//mediapipe/gpu:gpu_shared_data_internal"],
    }),
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
        "//third_party:godot",
    ],
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
        "//third_party:godot",
    ],
)
