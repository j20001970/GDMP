cc_library(
    name = "asset_util",
    srcs = ["asset_util.cc"],
    hdrs = ["asset_util.h"],
    deps = [
        "@mediapipe//mediapipe/util/android:asset_manager_util",
    ],
    alwayslink = 1,
)

cc_library(
    name = "jni",
    srcs = ["jni.cc"],
    hdrs = ["jni.h"],
    deps = [
        ":asset_util",
        "@godot-cpp//:godot",
    ],
    alwayslink = 1,
)

cc_binary(
    name = "GDMP",
    linkopts = select({
        "//conditions:default": ["-s"],
    }),
    linkshared = 1,
    deps = [
        ":jni",
        "//GDMP",
    ],
)
