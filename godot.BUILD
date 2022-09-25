licenses(["notice"])  # MIT

exports_files(["LICENSE"])

cc_library(
    name = "godot",
    srcs = glob([
        "src/*.cpp",
        "src/*/*.cpp",
        "gen/src/*/*.cpp",
    ]),
    hdrs = glob([
        "godot-headers/*/*.h",
        "include/godot_cpp/*.hpp",
        "include/godot_cpp/*/*.hpp",
        "gen/include/godot_cpp/*/*.hpp",
    ]),
    includes = [
        "godot-headers",
        "include",
        "gen/include",
    ],
    visibility = ["//visibility:public"],
)
