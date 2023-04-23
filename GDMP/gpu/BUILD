cc_library(
    name = "gpu_resources",
    srcs = ["gpu_resources.cc"],
    hdrs = ["gpu_resources.h"],
    copts = select({
        "//mediapipe/gpu:disable_gpu": ["-DMEDIAPIPE_DISABLE_GPU"],
        "//conditions:default": [],
    }),
    visibility = ["//visibility:public"],
    deps = [
        "@godot-cpp//:godot",
    ] + select({
        "//mediapipe/gpu:disable_gpu": [],
        "//conditions:default": ["//mediapipe/gpu:gpu_shared_data_internal"],
    }),
)
