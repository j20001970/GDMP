licenses(["notice"])  # MIT

exports_files(["LICENSE"])

cc_library(
    name = "godot",
    srcs = glob([
        "src/core/*.cpp",
        "src/gen/*.cpp",
    ]),
    hdrs = glob([
        "godot-headers/*.h",
        "godot-headers/*/*.h",
        "include/*/*.hpp",
    ]),
    copts = [
        "-fPIC",
        "-std=c++14",
        "-Wwrite-strings",
        "-O3",
    ],
    includes = [
        "godot-headers",
        "include",
        "include/core",
        "include/gen",
    ],
    linkstatic = 1,
    visibility = ["//visibility:public"],
)
