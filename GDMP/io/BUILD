cc_library(
    name = "camera_helper",
    srcs = select({
        "//conditions:default": ["cv_camera_helper.cc"],
    }),
    hdrs = ["camera_helper.h"],
    visibility = ["//visibility:public"],
    deps = [
        "//mediapipe/GDMP/framework:graph",
        "//third_party:godot",
    ] + select({
        "//conditions:default": [":cv_camera_helper"],
    }),
)

cc_library(
    name = "cv_camera_helper",
    deps = [
        "//mediapipe/GDMP/framework:packet",
        "//mediapipe/framework:packet",
        "//mediapipe/framework/formats:image_frame",
        "//mediapipe/framework/formats:image_frame_opencv",
        "//mediapipe/framework/port:opencv_core",
        "//mediapipe/framework/port:opencv_imgproc",
        "//mediapipe/framework/port:opencv_video",
    ],
)