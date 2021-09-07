tool
extends EditorPlugin

func _enter_tree():
	add_autoload_singleton("GDMP", "res://addons/GDMP/GDMP.gd")

func _exit_tree():
	remove_autoload_singleton("GDMP")
