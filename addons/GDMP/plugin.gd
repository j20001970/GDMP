@tool
extends EditorPlugin

var android_export_plugin: EditorExportPlugin = preload("editor/android_export_plugin.gd").new()

func _enter_tree() -> void:
	add_export_plugin(android_export_plugin)

func _exit_tree() -> void:
	remove_export_plugin(android_export_plugin)
