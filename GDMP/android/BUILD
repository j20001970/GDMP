cc_library(
    name = "asset_util",
    srcs = ["asset_util.cc"],
    hdrs = ["asset_util.h"],
    visibility = ["//visibility:public"],
    deps = [
        "@mediapipe//mediapipe/util/android:asset_manager_util",
    ],
    alwayslink = 1,
)

cc_binary(
    name = "GDMP",
    linkopts = select({
        "//conditions:default": ["-s"],
    }),
    linkshared = 1,
    visibility = ["//visibility:public"],
    deps = [
        "//GDMP",
    ],
)
