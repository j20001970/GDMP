load("@emsdk//emscripten_toolchain:wasm_rules.bzl", "wasm_cc_binary")

cc_binary(
    name = "web",
    linkopts = [
        "-sSIDE_MODULE=1",
        "-sSUPPORT_LONGJMP='wasm'",
        "-sWASM_BIGINT",
        "--oformat=wasm",
    ],
    deps = ["//GDMP"],
)

wasm_cc_binary(
    name = "GDMP",
    cc_target = ":web",
    outputs = ["GDMP.wasm"],
)
