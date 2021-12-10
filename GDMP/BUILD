load("//mediapipe/GDMP:variables.bzl", "CALCULATORS")

cc_binary(
    name = "gdmp",
    srcs = ["gdlibrary.cc"],
    deps = [
        "//mediapipe/GDMP/proto:landmark",
        "//third_party:godot",
    ] + select({
        "//mediapipe:android": [],
        "//conditions:default": [":gdmp_lib"] + CALCULATORS,
    }),
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
)

cc_library(
    name = "gdmp_lib",
    srcs = [
        "gdmp.cc",
        "gdmp.h",
    ],
    deps = [
        "//mediapipe/framework:calculator_framework",
        "//mediapipe/framework/formats:image_frame",
        "//mediapipe/framework/formats:image_frame_opencv",
        "//mediapipe/framework/formats:landmark_cc_proto",
        "//mediapipe/framework/port:file_helpers",
        "//mediapipe/framework/port:opencv_highgui",
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
    visibility = ["//visibility:public"],
)
