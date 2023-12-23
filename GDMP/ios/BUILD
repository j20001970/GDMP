load("@build_bazel_rules_apple//apple:ios.bzl", "ios_framework")

config_setting(
    name = "debug",
    values = {"compilation_mode": "dbg"},
)

config_setting(
    name = "release",
    values = {"compilation_mode": "opt"},
)

ios_framework(
    name = "GDMP",
    bundle_id = "io.gdmp",
    bundle_name = select({
        ":debug": "GDMP.ios.debug",
        ":release": "GDMP.ios.release",
    }),
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
