package(default_visibility = ["//visibility:public"])

cc_library(
    name = "language_detector",
    srcs = ["language_detector.cc"],
    hdrs = ["language_detector.h"],
    deps = [
        ":text_task",
        "//GDMP/tasks/processors:classifier_options",
        "@godot-cpp//:godot",
        "@mediapipe//mediapipe/tasks/cc/text/language_detector",
    ],
    alwayslink = 1,
)

cc_library(
    name = "text_classifier",
    srcs = ["text_classifier.cc"],
    hdrs = ["text_classifier.h"],
    deps = [
        ":text_task",
        "//GDMP/tasks/containers:classification_result",
        "//GDMP/tasks/processors:classifier_options",
        "@godot-cpp//:godot",
        "@mediapipe//mediapipe/tasks/cc/text/text_classifier",
    ],
    alwayslink = 1,
)

cc_library(
    name = "text_embedder",
    srcs = ["text_embedder.cc"],
    hdrs = ["text_embedder.h"],
    deps = [
        ":text_task",
        "//GDMP/tasks/containers:embedding_result",
        "//GDMP/tasks/processors:embedder_options",
        "@godot-cpp//:godot",
        "@mediapipe//mediapipe/tasks/cc/text/text_embedder",
    ],
    alwayslink = 1,
)

cc_library(
    name = "text_task",
    srcs = ["text_task.cc"],
    hdrs = ["text_task.h"],
    deps = [
        "//GDMP/tasks:task",
        "@godot-cpp//:godot",
    ],
)
