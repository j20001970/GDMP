package(default_visibility = ["//visibility:public"])

cc_library(
    name = "audio_classifier",
    srcs = ["audio_classifier.cc"],
    hdrs = ["audio_classifier.h"],
    deps = [
        ":audio_task",
        "//GDMP/tasks/containers:classification_result",
        "//GDMP/tasks/processors:classifier_options",
        "@godot-cpp//:godot",
        "@mediapipe//mediapipe/tasks/cc/audio/audio_classifier",
    ],
    alwayslink = 1,
)

cc_library(
    name = "audio_embedder",
    srcs = ["audio_embedder.cc"],
    hdrs = ["audio_embedder.h"],
    deps = [
        ":audio_task",
        "//GDMP/tasks/containers:embedding_result",
        "//GDMP/tasks/processors:embedder_options",
        "@godot-cpp//:godot",
        "@mediapipe//mediapipe/tasks/cc/audio/audio_embedder",
    ],
    alwayslink = 1,
)

cc_library(
    name = "audio_task",
    srcs = ["audio_task.cc"],
    hdrs = ["audio_task.h"],
    deps = [
        "//GDMP/tasks:task",
        "@godot-cpp//:godot",
        "@mediapipe//mediapipe/framework/formats:matrix",
        "@mediapipe//mediapipe/tasks/cc/audio/core:running_mode",
    ],
)
