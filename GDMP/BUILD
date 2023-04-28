load("@hedron_compile_commands//:refresh_compile_commands.bzl", "refresh_compile_commands")
load("@build_bazel_rules_apple//apple:ios.bzl", "ios_static_framework")
load("//mediapipe/GDMP:calculators.bzl", "CALCULATORS", "DATA")

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

refresh_compile_commands(
    name = "refresh_compile_commands",
    targets = {
        "//mediapipe/GDMP/io:camera_helper_android": """
        --define GODOT=1
        --host_crosstool_top=@bazel_tools//tools/cpp:toolchain
        --crosstool_top=//external:android/crosstool
        --cpu=arm64-v8a
        --copt -Ibazel-bin
        --copt -Ibazel-bin/external/com_github_gflags_gflags/_virtual_includes/gflags
        --copt -Ibazel-bin/external/com_github_glog_glog/_virtual_includes/default_glog_headers
        """,
        "//mediapipe/GDMP:GDMP": """
        --define GODOT=1
        --copt -Ibazel-bin
        --copt -Ibazel-bin/external/com_github_gflags_gflags/_virtual_includes/gflags
        --copt -Ibazel-bin/external/com_github_glog_glog/_virtual_includes/default_glog_headers
        """,
    },
)

cc_library(
    name = "GDMP_deps",
    srcs = ["gdlibrary.cc"],
    data = DATA,
    visibility = ["//visibility:public"],
    deps = [
        "//mediapipe/GDMP/framework:graph",
        "//mediapipe/GDMP/framework:graph_config",
        "//mediapipe/GDMP/framework:packet",
        "//mediapipe/GDMP/gpu:gpu_helper",
        "//mediapipe/GDMP/gpu:gpu_resources",
        "//mediapipe/GDMP/io:camera_helper",
        "//mediapipe/GDMP/proto:classification",
        "//mediapipe/GDMP/proto:detection",
        "//mediapipe/GDMP/proto:landmark",
        "//mediapipe/GDMP/proto:location_data",
        "//mediapipe/GDMP/proto:rasterization",
        "//mediapipe/GDMP/proto:rect",
        "@godot-cpp//:godot",
    ] + CALCULATORS,
    alwayslink = 1,
)

cc_binary(
    name = "GDMP",
    linkopts = select({
        "//conditions:default": ["-s"],
    }),
    linkshared = 1,
    visibility = ["//visibility:public"],
    deps = [":GDMP_deps"],
)

ios_static_framework(
    name = "GDMP_ios",
    families = [
        "iphone",
        "ipad",
    ],
    minimum_os_version = "11.0",
    deps = [":GDMP_deps"],
)
