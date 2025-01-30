tool
extends EditorPlugin

func _enter_tree() -> void:
	add_autoload_singleton("MediaPipe", "res://addons/GDMP/MediaPipe.gd")
	add_autoload_singleton("MediaPipeExternalFiles", "res://addons/GDMP/MediaPipeExternalFiles.gd")

func _exit_tree() -> void:
	remove_autoload_singleton("MediaPipeExternalFiles")
	remove_autoload_singleton("MediaPipe")
