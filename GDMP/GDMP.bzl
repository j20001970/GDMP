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
]
