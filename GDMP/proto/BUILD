load("@rules_cc//cc:cc_library.bzl", "cc_library")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "proto",
    srcs = [
        "proto.cc",
        "util.cc",
    ],
    hdrs = [
        "proto.h",
        "util.h",
    ],
    deps = [
        "//GDMP/framework:packet",
        "@godot-cpp//:godot",
        "@mediapipe//mediapipe/framework/port:core_proto",
    ],
)
