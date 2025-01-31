extends Node

var _gcs_url := "https://storage.googleapis.com"
var _assets_url := _gcs_url.path_join("mediapipe-assets")
var _models_url := _gcs_url.path_join("mediapipe-models")

func _on_request_completed(request: HTTPRequest) -> void:
	request.queue_free()
	remove_child(request)

func _get_file(url: String) -> HTTPRequest:
	var request := HTTPRequest.new()
	add_child(request)
	if request.request(url) == OK:
		var callback := _on_request_completed.bind(request).unbind(4)
		request.request_completed.connect(callback)
	else:
		remove_child(request)
		request.queue_free()
		request = null
	return request

func get_asset(path: String, generation: int = -1) -> HTTPRequest:
	var url := _assets_url.path_join(path)
	if generation > 0:
		url = url + "?generation=%d" % generation
	return _get_file(url)

func get_model(path: String) -> HTTPRequest:
	var url := _models_url.path_join(path)
	return _get_file(url)
