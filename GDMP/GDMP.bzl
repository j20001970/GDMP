"""GDMP configuration"""

CALCULATORS = [
    "@mediapipe//mediapipe/calculators/util:annotation_overlay_calculator",
    "@mediapipe//mediapipe/calculators/util:detections_to_render_data_calculator",
    "@mediapipe//mediapipe/calculators/util:landmarks_to_render_data_calculator",
    "@mediapipe//mediapipe/calculators/util:labels_to_render_data_calculator",
    "@mediapipe//mediapipe/calculators/util:rect_to_render_data_calculator",
]

# List of enabled tasks.
TASKS = [
    "//GDMP/tasks/audio:audio_classifier",
    "//GDMP/tasks/audio:audio_embedder",
    "//GDMP/tasks/text:language_detector",
    "//GDMP/tasks/text:text_classifier",
    "//GDMP/tasks/text:text_embedder",
    "//GDMP/tasks/vision:face_detector",
    "//GDMP/tasks/vision:face_landmarker",
    "//GDMP/tasks/vision:face_stylizer",
    "//GDMP/tasks/vision:gesture_recognizer",
    "//GDMP/tasks/vision:hand_landmarker",
    "//GDMP/tasks/vision:holistic_landmarker",
    "//GDMP/tasks/vision:image_classifier",
    "//GDMP/tasks/vision:image_embedder",
    "//GDMP/tasks/vision:object_detector",
    "//GDMP/tasks/vision:pose_landmarker",
] + select({
    "@platforms//os:emscripten": [],
    "//conditions:default": [
        "//GDMP/tasks/vision:image_segmenter",
        "//GDMP/tasks/vision:interactive_segmenter",
    ],
})
