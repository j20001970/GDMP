load("@emsdk//emscripten_toolchain:wasm_rules.bzl", "wasm_cc_binary")

config_setting(
    name = "web",
    values = {
        "crosstool_top": "@emsdk//emscripten_toolchain:everything",
    },
    visibility = ["//visibility:public"],
)

cc_binary(
    name = "GDMP",
    linkopts = [
        "-s STANDALONE_WASM=1",
        "--no-entry",
    ],
    deps = ["//GDMP"],
)

wasm_cc_binary(
    name = "GDMP_wasm",
    cc_target = ":GDMP",
)
