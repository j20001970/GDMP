extends EditorExportPlugin

func _supports_platform(platform: EditorExportPlatform) -> bool:
	if platform is EditorExportPlatformAndroid:
		return true
	return false

func _get_android_libraries(platform:EditorExportPlatform, debug: bool) -> PackedStringArray:
	return PackedStringArray(["GDMP/libs/GDMP.android.aar"])

func _get_android_dependencies(platform: EditorExportPlatform, debug: bool) -> PackedStringArray:
	var camerax_version: String = "1.0.0-beta10"
	return PackedStringArray([
		"com.google.flogger:flogger:0.3.1",
		"com.google.flogger:flogger-system-backend:0.3.1",
		"com.google.guava:guava:27.0.1-android",
		"androidx.camera:camera-core:%s" % camerax_version,
		"androidx.camera:camera-camera2:%s" % camerax_version,
		"androidx.camera:camera-lifecycle:%s" % camerax_version,
	])

func _get_name():
	return "GDMP"
