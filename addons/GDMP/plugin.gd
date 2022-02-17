tool
extends EditorPlugin

var exporter : EditorExportPlugin = preload("res://addons/GDMP/editor/exporter.gd").new()

func _enter_tree():
	add_autoload_singleton("GDMP", "res://addons/GDMP/GDMP.gd")
	add_export_plugin(exporter)

func _exit_tree():
	remove_autoload_singleton("GDMP")
	remove_export_plugin(exporter)
