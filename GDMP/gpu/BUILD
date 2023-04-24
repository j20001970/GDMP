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

cc_library(
    name = "gpu_helper",
    srcs = ["gpu_helper.cc"],
    hdrs = ["gpu_helper.h"],
    visibility = ["//visibility:public"],
    deps = [
        "//mediapipe/GDMP/framework:packet",
        "//mediapipe/GDMP/gpu:gpu_resources",
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
