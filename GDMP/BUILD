load("@hedron_compile_commands//:refresh_compile_commands.bzl", "refresh_compile_commands")
load("//GDMP:GDMP.bzl", "CALCULATORS", "DATA", "TASKS")

cc_library(
    name = "GDMP",
    srcs = ["gdlibrary.cc"],
    data = DATA,
    visibility = ["//visibility:public"],
    deps = [
        "//GDMP/framework:graph",
        "//GDMP/framework:graph_config",
        "//GDMP/framework:image",
        "//GDMP/framework:packet",
        "//GDMP/gpu:gpu_helper",
        "//GDMP/gpu:gpu_resources",
        "//GDMP/io:camera_helper",
        "//GDMP/proto:proto",
        "//GDMP/tasks:task",
        "//GDMP/tasks/containers:category",
        "//GDMP/tasks/containers:classification_result",
        "//GDMP/tasks/containers:detection_result",
        "//GDMP/tasks/containers:embedding_result",
        "//GDMP/tasks/containers:keypoint",
        "//GDMP/tasks/containers:landmark",
        "//GDMP/util:resource_util",
        "@godot-cpp//:godot",
    ] + CALCULATORS + TASKS,
    alwayslink = 1,
)

refresh_compile_commands(
    name = "refresh_compile_commands",
    targets = {
        "//GDMP:GDMP": """
        --copt -Ibazel-bin
        --copt -Ibazel-bin/external/com_github_gflags_gflags/_virtual_includes/gflags
        --copt -Ibazel-bin/external/com_github_glog_glog/_virtual_includes/default_glog_headers
        --copt -Ibazel-bin/external/com_github_glog_glog_no_gflags/_virtual_includes/default_glog_headers
        """,
        "//GDMP/io:camera_helper_android": """
        --host_crosstool_top=@bazel_tools//tools/cpp:toolchain
        --crosstool_top=//external:android/crosstool
        --cpu=arm64-v8a
        --copt -Ibazel-bin
        --copt -Ibazel-bin/external/com_github_gflags_gflags/_virtual_includes/gflags
        --copt -Ibazel-bin/external/com_github_glog_glog/_virtual_includes/default_glog_headers
        --copt -Ibazel-bin/external/com_github_glog_glog_no_gflags/_virtual_includes/default_glog_headers
        """,
    },
)
