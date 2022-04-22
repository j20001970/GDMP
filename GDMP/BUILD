load("//mediapipe/GDMP:calculators.bzl", "CALCULATORS")

alias(
    name = "godot",
    actual = select({
        ":godot_android": ":godot_android",
        "//conditions:default": "godot_desktop",
    }),
    visibility = ["//visibility:public"],
)

config_setting(
    name = "godot_android",
    define_values = {
        "GODOT": "1",
    },
    values = {"crosstool_top": "//external:android/crosstool"},
    visibility = ["//visibility:public"],
)

config_setting(
    name = "godot_desktop",
    define_values = {
        "GODOT": "1",
    },
    visibility = ["//visibility:public"],
)

cc_binary(
    name = "gdmp",
    srcs = [
        "gdlibrary.cc",
    ],
    defines = select({
        "//mediapipe:android": [],
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
        "//mediapipe/GDMP/framework:graph",
        "//mediapipe/GDMP/framework:packet",
        "//mediapipe/GDMP/io:camera_helper",
        "//mediapipe/GDMP/proto:classification",
        "//mediapipe/GDMP/proto:detection",
        "//mediapipe/GDMP/proto:landmark",
        "//mediapipe/GDMP/proto:location_data",
        "//mediapipe/GDMP/proto:rasterization",
        "//mediapipe/GDMP/proto:rect",
        "//third_party:godot",
    ] + select({
        "//mediapipe/gpu:disable_gpu": [],
        "//conditions:default": ["//mediapipe/GDMP/framework:gpu_helper"],
    }) + CALCULATORS,
)
