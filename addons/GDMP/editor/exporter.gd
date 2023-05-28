extends EditorExportPlugin

func _export_begin(features: PoolStringArray, is_debug: bool, path: String, flags: int) -> void:
	var files := find_files("res://", ["binarypb", "pbtxt", "task", "tflite"])
	for file in files:
		var f := File.new()
		if f.open(file, File.READ) == OK:
			add_file(file, f.get_buffer(f.get_len()), false)
		else:
			printerr("GDMP: Failed to open %s: %d" % [file, f.get_error()])

func _export_file(path: String, type: String, features: PoolStringArray) -> void:
	if path.get_file() == "GDMP.gdnlib":
		if features.has("Android"):
			skip()

func find_files(path: String, extenstions: PoolStringArray) -> PoolStringArray:
	var files: PoolStringArray = []
	var dir := Directory.new()
	var result := dir.open(path)
	if result == OK:
		dir.list_dir_begin(true, true)
		while true:
			var next := dir.get_next()
			if next.empty():
				break
			next = path.plus_file(next)
			if next.begins_with("res://android"):
				continue
			if dir.dir_exists(next):
				files.append_array(find_files(next, extenstions))
			elif next.get_extension() in extenstions:
				files.append(next)
	else:
		printerr("GDMP: Failed to open %s: %d" % [path, result])
	return files
