extends Node

var _gcs_url := "https://storage.googleapis.com"
var _assets_url := _gcs_url.plus_file("mediapipe-assets")
var _models_url := _gcs_url.plus_file("mediapipe-models")

func _on_request_completed(result: int, response_code: int, headers: PoolStringArray, body: PoolByteArray, request: HTTPRequest) -> void:
	request.queue_free()
	remove_child(request)

func _get_file(url: String) -> HTTPRequest:
	var request := HTTPRequest.new()
	add_child(request)
	if request.request(url) == OK:
		request.connect("request_completed", self, "_on_request_completed", [request])
	else:
		remove_child(request)
		request.queue_free()
		request = null
	return request

func get_asset(path: String, generation: int = -1) -> HTTPRequest:
	var url := _assets_url.plus_file(path)
	if generation > 0:
		url = url + "?generation=%d" % generation
	return _get_file(url)

func get_model(path: String) -> HTTPRequest:
	var url := _models_url.plus_file(path)
	return _get_file(url)
