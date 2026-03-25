licenses(["notice"])  # MIT

exports_files(["LICENSE"])

exports_files(
    glob(["**"]),
    visibility = ["//visibility:public"],
)

cc_library(
    name = "godot",
    srcs = glob([
        "gen/src/**",
        "src/**",
    ]),
    hdrs = glob([
        "gen/include/**",
        "include/**",
    ]),
    copts = select({
        "@platforms//os:windows": [
            "/utf-8",
        ],
        "//conditions:default": [
            "-fno-exceptions",
        ],
    }),
    defines = select({
        "@platforms//os:windows": [
            "_HAS_EXCEPTIONS=0",
            "TYPED_METHOD_BIND",
            "NOMINMAX",
        ],
        "//conditions:default": [],
    }),
    includes = [
        "gen/include",
        "include",
    ],
    visibility = ["//visibility:public"],
)
