cc_library(
    name = "asset_util",
    srcs = ["asset_util.cc"],
    visibility = ["//visibility:public"],
    deps = [
        "@mediapipe//mediapipe/java/com/google/mediapipe/framework/jni:jni_util",
        "@mediapipe//mediapipe/util/android:asset_manager_util",
    ],
    alwayslink = 1,
)

cc_library(
    name = "jni",
    srcs = ["jni.cc"],
    hdrs = ["jni.h"],
    visibility = ["//visibility:public"],
    deps = ["@godot-cpp//:godot"],
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
        ":asset_util",
        ":jni",
        "//GDMP",
    ],
)
