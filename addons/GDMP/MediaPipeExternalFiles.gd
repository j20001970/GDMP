@tool
extends Node

var external_url := "https://storage.googleapis.com/mediapipe-assets"

func _on_request_completed(request: HTTPRequest) -> void:
	request.queue_free()
	remove_child(request)

func get_file(filename: String, generation: int = -1) -> HTTPRequest:
	var request := HTTPRequest.new()
	add_child(request)
	var url := external_url.path_join(filename)
	if generation > 0:
		url = url + "?generation=%d" % generation
	if request.request(url) == OK:
		var callback := _on_request_completed.bind(request).unbind(4)
		request.request_completed.connect(callback)
	else:
		remove_child(request)
		request.queue_free()
		request = null
	return request
