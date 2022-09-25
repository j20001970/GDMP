extends EditorExportPlugin

func _export_begin(features : PackedStringArray, is_debug : bool, path : String, flags : int) -> void:
	var exports : PackedStringArray = find_files("res://", ["binarypb", "pbtxt", "tflite"])
	for file in exports:
		var f : File = File.new()
		var result : int = f.open(file, File.READ)
		if result != OK:
			printerr("GDMP exporter: Failed to read %s: %d" % [file, result])
			continue
		add_file(file, f.get_buffer(f.get_length()), false)

func find_files(path : String, extenstions : PackedStringArray) -> PackedStringArray:
	var files : PackedStringArray = []
	var dir : Directory = Directory.new()
	var result : int = dir.open(path)
	if result != OK:
		printerr("GDMP exporter: Failed to open %s: %d" % [path, result])
	else:
		dir.list_dir_begin()
		var filename : String
		while true:
			filename = dir.get_next()
			if filename.is_empty():
				break
			var next : String = "%s%s" if path.ends_with("/") else "%s/%s"
			next = next % [path, filename]
			if dir.dir_exists(filename) and not next == "res://android":
				files.append_array(find_files(next, extenstions))
			elif filename.get_extension() in extenstions:
				files.append(next)
		dir.list_dir_end()
	return files
