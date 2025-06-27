extends Node

func _enter_tree() -> void:
	var System := JavaClassWrapper.wrap("java.lang.System")
	if System == null:
		return
	System.call("loadLibrary", "GDMP.android")
