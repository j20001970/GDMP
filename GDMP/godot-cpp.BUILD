licenses(["notice"])  # MIT

exports_files(["LICENSE"])

cc_library(
    name = "godot",
    srcs = glob([
        "src/*.cpp",
        "src/*/*.cpp",
        "gen/src/*.cpp",
        "gen/src/*/*.cpp",
    ]),
    hdrs = glob([
        "gdextension/*.h",
        "include/godot_cpp/*.hpp",
        "include/godot_cpp/*/*.hpp",
        "gen/include/godot_cpp/*/*.hpp",
    ]),
    copts = select({
        "@mediapipe//mediapipe:windows": [
            "/utf-8",
        ],
        "//conditions:default": [
            "-fno-exceptions",
        ],
    }),
    defines = select({
        "@mediapipe//mediapipe:windows": [
            "_HAS_EXCEPTIONS=0",
            "TYPED_METHOD_BIND",
            "NOMINMAX",
        ],
        "//conditions:default": [],
    }),
    includes = [
        "gdextension",
        "gen/include",
        "include",
    ],
    visibility = ["//visibility:public"],
)
