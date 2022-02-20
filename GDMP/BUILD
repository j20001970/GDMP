load("//mediapipe/GDMP:variables.bzl", "CALCULATORS")

config_setting(
    name = "godot",
    define_values = {
        "GODOT": "1",
    },
    visibility = ["//visibility:public"],
)

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
        "//mediapipe/GDMP/framework:graph",
        "//mediapipe/GDMP/framework:packet",
        "//mediapipe/GDMP/io:camera_helper",
        "//third_party:godot",
    ] + select({
        "//mediapipe/gpu:disable_gpu": [],
        "//conditions:default": ["//mediapipe/GDMP/framework:gpu_helper"],
    }),
    alwayslink = 1,
)
