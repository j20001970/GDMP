@tool
extends EditorPlugin

func _enter_tree() -> void:
	add_autoload_singleton("MediaPipeExternalFiles", "MediaPipeExternalFiles.gd")
	add_autoload_singleton("GDMPAndroid", "GDMPAndroid.gd")

func _exit_tree() -> void:
	remove_autoload_singleton("MediaPipeExternalFiles")
	remove_autoload_singleton("GDMPAndroid")
