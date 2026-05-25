load("@rules_cc//cc:cc_binary.bzl", "cc_binary")
load("@rules_cc//cc:cc_library.bzl", "cc_library")

cc_library(
    name = "asset_util",
    srcs = ["asset_util.cc"],
    hdrs = ["asset_util.h"],
    deps = [
        "@mediapipe//mediapipe/util/android:asset_manager_util",
    ],
    alwayslink = 1,
)

cc_binary(
    name = "GDMP",
    linkshared = 1,
    deps = [
        "//GDMP",
    ],
)
