package(default_visibility = ["//visibility:public"])

cc_library(
    name = "gpu_helper",
    srcs = ["gpu_helper.cc"],
    hdrs = ["gpu_helper.h"],
    deps = [
        ":gpu_resources",
        "//GDMP/framework:image",
        "//GDMP/framework:packet",
        "@godot-cpp//:godot",
    ] + select({
        "@mediapipe//mediapipe/gpu:disable_gpu": [],
        "//conditions:default": [
            "@mediapipe//mediapipe/gpu:gl_calculator_helper",
            "@mediapipe//mediapipe/gpu:gpu_buffer",
            "@mediapipe//mediapipe/gpu:gpu_shared_data_internal",
        ],
    }),
)

cc_library(
    name = "gpu_resources",
    srcs = ["gpu_resources.cc"],
    hdrs = ["gpu_resources.h"],
    deps = [
        "@godot-cpp//:godot",
        "@mediapipe//mediapipe/gpu:gpu_shared_data_internal",
    ],
)
