load("@emsdk//emscripten_toolchain:wasm_rules.bzl", "wasm_cc_binary")

cc_binary(
    name = "web",
    deps = ["//GDMP"],
)

wasm_cc_binary(
    name = "GDMP",
    cc_target = ":web",
    outputs = ["GDMP.wasm"],
)
