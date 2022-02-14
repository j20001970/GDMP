load("//mediapipe/GDMP:variables.bzl", "CALCULATORS")

cc_binary(
    name = "gdmp",
    srcs = ["gdlibrary.cc"],
    defines = select({
        "//mediapipe:android": ["NO_GDMP"],
        "//conditions:default": [
            "MESA_EGL_NO_X11_HEADERS",
            "EGL_NO_X11",
        ],
    }),
    linkopts = [
        "-s",
    ] + select({
        "//mediapipe:android": [
            "-llog",
            "-lm",
        ],
        "//conditions:default": [],
    }),
    linkshared = 1,
    visibility = ["//visibility:public"],
    deps = [
        "//mediapipe/GDMP/proto:classification",
        "//mediapipe/GDMP/proto:detection",
        "//mediapipe/GDMP/proto:landmark",
        "//mediapipe/GDMP/proto:location_data",
        "//mediapipe/GDMP/proto:rasterization",
        "//mediapipe/GDMP/proto:rect",
        "//third_party:godot",
    ] + select({
        "//mediapipe:android": [],
        "//conditions:default": [":gdmp_lib"] + CALCULATORS,
    }),
)

cc_library(
    name = "gdmp_lib",
    srcs = [
        "gdmp.cc",
        "gdmp.h",
    ],
    visibility = ["//visibility:public"],
    deps = [
        "//mediapipe/framework:calculator_framework",
        "//mediapipe/framework/formats:image_frame",
        "//mediapipe/framework/formats:image_frame_opencv",
        "//mediapipe/framework/port:file_helpers",
        "//mediapipe/framework/port:opencv_core",
        "//mediapipe/framework/port:opencv_imgproc",
        "//mediapipe/framework/port:opencv_video",
        "//mediapipe/framework/port:parse_text_proto",
        "//mediapipe/framework/port:status",
        "//mediapipe/gpu:gl_calculator_helper",
        "//mediapipe/gpu:gpu_buffer",
        "//mediapipe/gpu:gpu_shared_data_internal",
        "//third_party:godot",
    ],
    alwayslink = 1,
)
