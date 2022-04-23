cc_library(
    name = "camera_helper",
    srcs = ["camera_helper.cc"] + select({
        "//mediapipe:android": ["camera_helper_android.cc"],
        "//conditions:default": ["camera_helper_cv.cc"],
    }),
    hdrs = ["camera_helper.h"],
    visibility = ["//visibility:public"],
    deps = [
        "//mediapipe/GDMP/framework:graph",
        "//third_party:godot",
    ] + select({
        "//mediapipe:android": [":camera_helper_android"],
        "//conditions:default": [":camera_helper_cv"],
    }),
)

cc_library(
    name = "camera_helper_android",
    deps = [
        "//mediapipe/GDMP/framework:packet",
        "//mediapipe/framework:packet",
        "//mediapipe/gpu:gl_texture_buffer",
        "//mediapipe/gpu:gpu_buffer",
        "//mediapipe/gpu:gpu_buffer_format",
        "//third_party:godot",
    ],
)

cc_library(
    name = "camera_helper_cv",
    deps = [
        "//mediapipe/GDMP/framework:packet",
        "//mediapipe/framework:packet",
        "//mediapipe/framework/formats:image_frame",
        "//mediapipe/framework/formats:image_frame_opencv",
        "//mediapipe/framework/port:opencv_core",
        "//mediapipe/framework/port:opencv_imgproc",
        "//mediapipe/framework/port:opencv_video",
    ] + select({
        "//mediapipe/gpu:disable_gpu": [],
        "//conditions:default": ["//mediapipe/GDMP/framework:gpu_helper"],
    }),
)
