cc_binary(
    name = "GDMP",
    linkopts = select({
        "@platforms//os:macos": [],
        "@platforms//os:windows": [],
        "//conditions:default": [
            "-Wl,-R,'$$ORIGIN'",
            "-s",
        ],
    }),
    linkshared = 1,
    deps = ["//GDMP"],
)
