package(default_visibility = ["//visibility:public"])

cc_library(
    name = "doc_classes",
    srcs = [":doc_classes_generated"],
    deps = [
        "@godot-cpp//:godot",
    ],
    alwayslink = 1,
)

genrule(
    name = "doc_classes_generated",
    srcs = [
        "@godot-cpp//:doc_source_generator.py",
    ] + glob(["*.xml"]),
    outs = ["doc_classes.cc"],
    cmd = """
python3 - << EOF
import sys
from os import path
sys.path.append(path.dirname("$(location @godot-cpp//:doc_source_generator.py)"))
from doc_source_generator import generate_doc_source
files = [f for f in "$(SRCS)".split(" ") if f.endswith(".xml")]
generate_doc_source("$@", files)
EOF
    """,
)
