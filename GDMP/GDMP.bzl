"""GDMP configuration"""

CALCULATORS = [
    # Put additional calculator dependencies here.
]

DATA = [
    # Put additional data used by tasks or calculator dependencies here.
]

# List of enabled tasks.
TASKS = [
    "//GDMP/tasks/vision:face_detector",
    "//GDMP/tasks/vision:face_landmarker",
    "//GDMP/tasks/vision:face_stylizer",
    "//GDMP/tasks/vision:gesture_recognizer",
    "//GDMP/tasks/vision:hand_landmarker",
    "//GDMP/tasks/vision:holistic_landmarker",
    "//GDMP/tasks/vision:image_classifier",
    "//GDMP/tasks/vision:image_embedder",
    "//GDMP/tasks/vision:image_segmenter",
    "//GDMP/tasks/vision:interactive_segmenter",
    "//GDMP/tasks/vision:object_detector",
    "//GDMP/tasks/vision:pose_landmarker",
]
