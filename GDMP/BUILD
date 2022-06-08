load("@build_bazel_rules_apple//apple:ios.bzl", "ios_static_framework")
load("//mediapipe/GDMP:calculators.bzl", "CALCULATORS")

alias(
    name = "godot",
    actual = select({
        "//mediapipe:android": ":godot_android",
        "//mediapipe:ios": ":godot_ios",
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

config_setting(
    name = "godot_ios",
    define_values = {
        "GODOT": "1",
    },
    values = {
        "apple_platform_type": "ios",
    },
    visibility = ["//visibility:public"],
)

cc_library(
    name = "GDMP_deps",
    srcs = ["gdlibrary.cc"],
    linkopts = select({
        "//conditions:default": ["-s"],
    }),
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

cc_binary(
    name = "GDMP",
    linkshared = 1,
    visibility = ["//visibility:public"],
    deps = [":GDMP_deps"],
)

ios_static_framework(
    name = "GDMP_ios",
    minimum_os_version = "11.0",
    families = ["iphone", "ipad"],
    deps = [":GDMP_deps"],
)
