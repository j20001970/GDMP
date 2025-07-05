cc_library(
    name = "language_detector",
    srcs = ["language_detector.cc"],
    hdrs = ["language_detector.h"],
    visibility = ["//visibility:public"],
    deps = [
        ":text_task",
        "@godot-cpp//:godot",
        "@mediapipe//mediapipe/tasks/cc/text/language_detector",
    ],
    alwayslink = 1,
)

cc_library(
    name = "text_classifier",
    srcs = ["text_classifier.cc"],
    hdrs = ["text_classifier.h"],
    visibility = ["//visibility:public"],
    deps = [
        ":text_task",
        "//GDMP/tasks/containers:classification_result",
        "@godot-cpp//:godot",
        "@mediapipe//mediapipe/tasks/cc/text/text_classifier",
    ],
    alwayslink = 1,
)

cc_library(
    name = "text_embedder",
    srcs = ["text_embedder.cc"],
    hdrs = ["text_embedder.h"],
    visibility = ["//visibility:public"],
    deps = [
        ":text_task",
        "//GDMP/tasks/containers:embedding_result",
        "@godot-cpp//:godot",
        "@mediapipe//mediapipe/tasks/cc/text/text_embedder",
    ],
    alwayslink = 1,
)

cc_library(
    name = "text_task",
    srcs = ["text_task.cc"],
    hdrs = ["text_task.h"],
    visibility = ["//visibility:public"],
    deps = [
        "//GDMP/tasks:task",
        "@godot-cpp//:godot",
    ],
)