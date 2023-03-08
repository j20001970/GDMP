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
        "gdextension/gdextension_interface.h",
        "include/godot_cpp/*.hpp",
        "include/godot_cpp/*/*.hpp",
        "gen/include/godot_cpp/*/*.hpp",
    ]),
    includes = [
        "gdextension",
        "include",
        "gen/include",
    ],
    visibility = ["//visibility:public"],
)
