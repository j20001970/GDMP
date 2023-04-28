load("@build_bazel_rules_apple//apple:ios.bzl", "ios_static_framework")

ios_static_framework(
    name = "GDMP",
    families = [
        "iphone",
        "ipad",
    ],
    minimum_os_version = "11.0",
    deps = ["//mediapipe/GDMP"],
)
