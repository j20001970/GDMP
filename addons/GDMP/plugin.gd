tool
extends EditorPlugin

var exporter : EditorExportPlugin = preload("res://addons/GDMP/editor/exporter.gd").new()

func _enter_tree():
	add_autoload_singleton("MediaPipe", "res://addons/GDMP/MediaPipe.gd")
	add_export_plugin(exporter)

func _exit_tree():
	remove_autoload_singleton("MediaPipe")
	remove_export_plugin(exporter)
