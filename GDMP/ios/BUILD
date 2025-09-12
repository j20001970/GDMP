load("@build_bazel_rules_apple//apple:apple.bzl", "apple_xcframework")

apple_xcframework(
    name = "GDMP",
    bundle_id = "",
    bundle_name = "GDMP.ios",
    families_required = {
        "ios": [
            "iphone",
            "ipad",
        ],
    },
    infoplists = ["Info.plist"],
    ios = {
        "device": ["arm64"],
        "simulator": ["arm64", "x86_64"],
    },
    minimum_os_versions = {
        "ios": "12.0",
    },
    deps = ["//GDMP"],
)
