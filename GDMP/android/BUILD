cc_library(
    name = "jni",
    srcs = ["jni.cc"],
    hdrs = ["jni.h"],
    visibility = ["//visibility:public"],
    deps = [
        "//mediapipe/java/com/google/mediapipe/framework/jni:jni_util",
        "//mediapipe/util/android:asset_manager_util",
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
        ":jni",
        "//GDMP",
    ],
)
