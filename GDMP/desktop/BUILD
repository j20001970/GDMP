load("@rules_cc//cc:cc_binary.bzl", "cc_binary")

cc_binary(
    name = "GDMP",
    linkopts = select({
        "@platforms//os:linux": ["-Wl,-R,'$$ORIGIN'"],
        "//conditions:default": [],
    }),
    linkshared = 1,
    deps = ["//GDMP"],
)
