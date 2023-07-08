load("//mediapipe/framework/tool:mediapipe_files.bzl", "mediapipe_files")

mediapipe_files(
    srcs = [
        "coco_efficientdet_lite0_v1_1.0_quant_2021_09_06.tflite",
        "coco_ssd_mobilenet_v1_1.0_quant_2018_06_29.tflite",
        "coco_ssd_mobilenet_v1_1.0_quant_2018_06_29_with_dummy_score_calibration.tflite",
        "deeplabv3.tflite",
        "efficientdet_lite0_fp16_no_nms.tflite",
        "face_detection_full_range.tflite",
        "face_detection_full_range_sparse.tflite",
        "face_detection_short_range.tflite",
        "face_landmarker.task",
        "face_landmarker_v2.task",
        "face_landmarker_v2_with_blendshapes.task",
        "gesture_recognizer.task",
        "hair_segmentation.tflite",
        "hand_landmarker.task",
        "mobilenet_v1_0.25_224_quant.tflite",
        "mobilenet_v1_0.25_224_quant_with_dummy_score_calibration.tflite",
        "mobilenet_v2_1.0_224.tflite",
        "mobilenet_v3_small_100_224_embedder.tflite",
        "pose_landmarker.task",
        "selfie_segm_128_128_3.tflite",
        "selfie_segm_144_256_3.tflite",
        "selfie_segmentation.tflite",
        "selfie_segmentation_landscape.tflite",
    ],
)

filegroup(
    name = "face_detector_models",
    srcs = [
        "face_detection_full_range.tflite",
        "face_detection_full_range_sparse.tflite",
        "face_detection_short_range.tflite",
    ],
)

filegroup(
    name = "face_landmarker_models",
    srcs = [
        "face_landmarker.task",
        "face_landmarker_v2.task",
        "face_landmarker_v2_with_blendshapes.task",
    ],
)

filegroup(
    name = "gesture_recognizer_models",
    srcs = [
        "gesture_recognizer.task",
    ],
)

filegroup(
    name = "hand_landmarker_models",
    srcs = [
        "hand_landmarker.task",
    ],
)

filegroup(
    name = "image_classifier_models",
    srcs = [
        "mobilenet_v1_0.25_224_quant.tflite",
        "mobilenet_v1_0.25_224_quant_with_dummy_score_calibration.tflite",
        "mobilenet_v2_1.0_224.tflite",
    ],
)

filegroup(
    name = "image_embedder_models",
    srcs = [
        "mobilenet_v3_small_100_224_embedder.tflite",
    ],
)

filegroup(
    name = "image_segmenter_models",
    srcs = [
        "deeplabv3.tflite",
        "hair_segmentation.tflite",
        "selfie_segm_128_128_3.tflite",
        "selfie_segm_144_256_3.tflite",
        "selfie_segmentation.tflite",
        "selfie_segmentation_landscape.tflite",
    ],
)

filegroup(
    name = "object_detector_models",
    srcs = [
        "coco_efficientdet_lite0_v1_1.0_quant_2021_09_06.tflite",
        "coco_ssd_mobilenet_v1_1.0_quant_2018_06_29.tflite",
        "coco_ssd_mobilenet_v1_1.0_quant_2018_06_29_with_dummy_score_calibration.tflite",
        "efficientdet_lite0_fp16_no_nms.tflite",
    ],
)

filegroup(
    name = "pose_landmarker_models",
    srcs = [
        "pose_landmarker.task",
    ],
)

cc_library(
    name = "face_detector",
    srcs = ["face_detector.cc"],
    hdrs = ["face_detector.h"],
    data = [":face_detector_models"],
    visibility = ["//visibility:public"],
    deps = [
        ":vision_task",
        "//GDMP/framework:image",
        "//GDMP/tasks:task",
        "//GDMP/tasks/containers:detection_result",
        "//mediapipe/tasks/cc/vision/face_detector",
        "@godot-cpp//:godot",
    ],
    alwayslink = 1,
)

cc_library(
    name = "face_landmarker",
    srcs = ["face_landmarker.cc"],
    hdrs = ["face_landmarker.h"],
    data = [":face_landmarker_models"],
    visibility = ["//visibility:public"],
    deps = [
        ":vision_task",
        "//GDMP/framework:image",
        "//GDMP/tasks:task",
        "//GDMP/tasks/containers:classification_result",
        "//GDMP/tasks/containers:landmark",
        "//mediapipe/tasks/cc/vision/face_landmarker",
        "@godot-cpp//:godot",
    ],
    alwayslink = 1,
)

cc_library(
    name = "gesture_recognizer",
    srcs = ["gesture_recognizer.cc"],
    hdrs = ["gesture_recognizer.h"],
    data = [":gesture_recognizer_models"],
    visibility = ["//visibility:public"],
    deps = [
        ":vision_task",
        "//GDMP/framework:image",
        "//GDMP/proto",
        "//GDMP/tasks:task",
        "//mediapipe/tasks/cc/vision/gesture_recognizer",
        "@godot-cpp//:godot",
    ],
    alwayslink = 1,
)

cc_library(
    name = "hand_landmarker",
    srcs = ["hand_landmarker.cc"],
    hdrs = ["hand_landmarker.h"],
    data = [":hand_landmarker_models"],
    visibility = ["//visibility:public"],
    deps = [
        ":vision_task",
        "//GDMP/framework:image",
        "//GDMP/tasks:task",
        "//GDMP/tasks/containers:classification_result",
        "//GDMP/tasks/containers:landmark",
        "//mediapipe/tasks/cc/vision/hand_landmarker",
        "@godot-cpp//:godot",
    ],
    alwayslink = 1,
)

cc_library(
    name = "image_classifier",
    srcs = ["image_classifier.cc"],
    hdrs = ["image_classifier.h"],
    data = [":image_classifier_models"],
    visibility = ["//visibility:public"],
    deps = [
        ":vision_task",
        "//GDMP/framework:image",
        "//GDMP/tasks:task",
        "//GDMP/tasks/containers:classification_result",
        "//mediapipe/tasks/cc/vision/image_classifier",
        "@godot-cpp//:godot",
    ],
    alwayslink = 1,
)

cc_library(
    name = "image_embedder",
    srcs = ["image_embedder.cc"],
    hdrs = ["image_embedder.h"],
    data = [":image_embedder_models"],
    visibility = ["//visibility:public"],
    deps = [
        ":vision_task",
        "//GDMP/framework:image",
        "//GDMP/tasks:task",
        "//GDMP/tasks/containers:embedding_result",
        "//mediapipe/tasks/cc/vision/image_embedder",
        "@godot-cpp//:godot",
    ],
    alwayslink = 1,
)

cc_library(
    name = "image_segmenter",
    srcs = ["image_segmenter.cc"],
    hdrs = ["image_segmenter.h"],
    data = [":image_segmenter_models"],
    visibility = ["//visibility:public"],
    deps = [
        ":vision_task",
        "//GDMP/framework:image",
        "//GDMP/tasks:task",
        "//mediapipe/tasks/cc/vision/image_segmenter",
        "@godot-cpp//:godot",
    ],
    alwayslink = 1,
)

cc_library(
    name = "object_detector",
    srcs = ["object_detector.cc"],
    hdrs = ["object_detector.h"],
    data = [":object_detector_models"],
    visibility = ["//visibility:public"],
    deps = [
        ":vision_task",
        "//GDMP/framework:image",
        "//GDMP/tasks:task",
        "//GDMP/tasks/containers:detection_result",
        "//mediapipe/tasks/cc/vision/object_detector",
        "@godot-cpp//:godot",
    ],
    alwayslink = 1,
)

cc_library(
    name = "pose_landmarker",
    srcs = ["pose_landmarker.cc"],
    hdrs = ["pose_landmarker.h"],
    data = [":pose_landmarker_models"],
    visibility = ["//visibility:public"],
    deps = [
        ":vision_task",
        "//GDMP/framework:image",
        "//GDMP/tasks:task",
        "//GDMP/tasks/containers:landmark",
        "//mediapipe/tasks/cc/vision/pose_landmarker",
        "@godot-cpp//:godot",
    ],
    alwayslink = 1,
)

cc_library(
    name = "vision_task",
    srcs = ["vision_task.cc"],
    hdrs = ["vision_task.h"],
    visibility = ["//visibility:public"],
    deps = [
        "//mediapipe/tasks/cc/vision/core:image_processing_options",
        "@godot-cpp//:godot",
    ],
)
