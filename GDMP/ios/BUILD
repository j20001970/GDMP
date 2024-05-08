load("@build_bazel_rules_apple//apple:ios.bzl", "ios_framework")

ios_framework(
    name = "GDMP",
    bundle_id = "io.gdmp",
    bundle_name = "GDMP.ios",
    families = [
        "iphone",
        "ipad",
    ],
    infoplists = ["Info.plist"],
    minimum_os_version = "12.0",
    deps = [
        "//GDMP",
        "@ios_opencv//:OpencvFramework",
    ],
)
