tool
extends EditorPlugin

func _enter_tree() -> void:
	add_autoload_singleton("MediaPipe", "res://addons/GDMP/MediaPipe.gd")

func _exit_tree() -> void:
	remove_autoload_singleton("MediaPipe")
