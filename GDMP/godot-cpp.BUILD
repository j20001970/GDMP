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
    includes = [
        "godot-headers",
        "include",
        "include/core",
        "include/gen",
    ],
    visibility = ["//visibility:public"],
)
