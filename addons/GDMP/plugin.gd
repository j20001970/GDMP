tool
extends EditorPlugin

var exporter: EditorExportPlugin = preload("editor/exporter.gd").new()

func _enter_tree() -> void:
	add_autoload_singleton("MediaPipe", "res://addons/GDMP/MediaPipe.gd")
	add_export_plugin(exporter)

func _exit_tree() -> void:
	remove_autoload_singleton("MediaPipe")
	remove_export_plugin(exporter)
