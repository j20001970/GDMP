cc_library(
    name = "audio_classifier",
    srcs = ["audio_classifier.cc"],
    hdrs = ["audio_classifier.h"],
    visibility = ["//visibility:public"],
    deps = [
        ":audio_task",
        "//GDMP/tasks/containers:classification_result",
        "@mediapipe//mediapipe/tasks/cc/audio/audio_classifier",
        "@godot-cpp//:godot",
    ],
    alwayslink = 1,
)

cc_library(
    name = "audio_embedder",
    srcs = ["audio_embedder.cc"],
    hdrs = ["audio_embedder.h"],
    visibility = ["//visibility:public"],
    deps = [
        ":audio_task",
        "//GDMP/tasks/containers:embedding_result",
        "@mediapipe//mediapipe/tasks/cc/audio/audio_embedder",
        "@godot-cpp//:godot",
    ],
    alwayslink = 1,
)

cc_library(
    name = "audio_task",
    srcs = ["audio_task.cc"],
    hdrs = ["audio_task.h"],
    visibility = ["//visibility:public"],
    deps = [
        "//GDMP/tasks:task",
        "@mediapipe//mediapipe/framework/formats:matrix",
        "@mediapipe//mediapipe/tasks/cc/audio/core:running_mode",
        "@godot-cpp//:godot",
    ],
)
