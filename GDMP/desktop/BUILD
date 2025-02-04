cc_binary(
    name = "GDMP",
    linkopts = select({
        "@mediapipe//mediapipe:macos": [],
        "@mediapipe//mediapipe:windows": [],
        "//conditions:default": [
            "-Wl,-R,'$$ORIGIN'",
            "-s",
        ],
    }),
    linkshared = 1,
    visibility = ["//visibility:public"],
    deps = ["//GDMP"],
)
