cc_library(
    name = "camera_helper",
    srcs = ["camera_helper.cc"],
    hdrs = ["camera_helper.h"],
    visibility = ["//visibility:public"],
    deps = [
        "//mediapipe/GDMP/framework:graph",
        "//third_party:godot",
    ] + select({
        "//mediapipe:android": [":camera_helper_android"],
        "//mediapipe:ios": [":camera_helper_ios"],
        "//conditions:default": [":camera_helper_cv"],
    }),
)

cc_library(
    name = "camera_helper_android",
    srcs = ["camera_helper_android.cc"],
    hdrs = ["camera_helper.h"],
    deps = [
        "//mediapipe/GDMP/framework:graph",
        "//mediapipe/GDMP/framework:packet",
        "//mediapipe/gpu:gl_texture_buffer",
        "//mediapipe/gpu:gpu_buffer",
        "//mediapipe/gpu:gpu_buffer_format",
        "//third_party:godot",
    ],
)

cc_library(
    name = "camera_helper_cv",
    srcs = ["camera_helper_cv.cc"],
    hdrs = ["camera_helper.h"],
    deps = [
        "//mediapipe/GDMP/framework:graph",
        "//mediapipe/GDMP/framework:packet",
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

objc_library(
    name = "camera_helper_ios",
    srcs = ["camera_helper_ios.mm"],
    hdrs = ["camera_helper.h"],
    sdk_frameworks = [
        "Accelerate",
        "AVFoundation",
        "CoreGraphics",
        "CoreVideo",
    ],
    deps = [
        "//mediapipe/GDMP/framework:graph",
        "//mediapipe/GDMP/framework:packet",
    ],
    copts = [
        "-std=c++17",
    ],
)
