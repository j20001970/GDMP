#include "detection_result.h"

#include "Engine.hpp"

#include "GDMP/tasks/containers/category.h"
#include "GDMP/tasks/containers/keypoint.h"

void MediaPipeDetection::_register_methods() {
	register_method("get_categories", &MediaPipeDetection::get_categories);
	register_method("get_bounding_box", &MediaPipeDetection::get_bounding_box);
	register_method("get_keypoints", &MediaPipeDetection::get_keypoints);
	register_method("has_keypoints", &MediaPipeDetection::has_keypoints);
	register_method("get_proto", &MediaPipeDetection::get_proto);
	register_method("make_vector_proto_packet", &MediaPipeDetection::make_vector_proto_packet);
}

MediaPipeDetection *MediaPipeDetection::_new(const Detection &detection) {
	MediaPipeDetection *d = MediaPipeDetection::_new();
	d->detection = detection;
	return d;
}

mediapipe::Detection MediaPipeDetection::to_proto(const Detection &detection) {
	ProtoType proto;
	proto.mutable_label_id()->Reserve(detection.categories.size());
	proto.mutable_score()->Reserve(detection.categories.size());
	proto.mutable_label()->Reserve(detection.categories.size());
	proto.mutable_display_name()->Reserve(detection.categories.size());
	for (const Category &category : detection.categories) {
		proto.mutable_label_id()->Add(category.index);
		proto.mutable_score()->Add(category.score);
		proto.mutable_label()->Add(category.category_name.value_or(""));
		proto.mutable_display_name()->Add(category.display_name.value_or(""));
	}
	proto.mutable_location_data()->set_format(mediapipe::LocationData::BOUNDING_BOX);
	const Rect &bounding_box = detection.bounding_box;
	proto.mutable_location_data()->mutable_bounding_box()->set_xmin(bounding_box.left);
	proto.mutable_location_data()->mutable_bounding_box()->set_ymin(bounding_box.top);
	proto.mutable_location_data()->mutable_bounding_box()->set_width(bounding_box.right - bounding_box.left);
	proto.mutable_location_data()->mutable_bounding_box()->set_height(bounding_box.bottom - bounding_box.top);
	if (detection.keypoints.has_value()) {
		const auto &keypoints = detection.keypoints.value();
		proto.mutable_location_data()->mutable_relative_keypoints()->Reserve(keypoints.size());
		for (const NormalizedKeypoint &keypoint : keypoints) {
			proto.mutable_location_data()->mutable_relative_keypoints()->Add()->CopyFrom(MediaPipeNormalizedKeypoint::to_proto(keypoint));
		}
	}
	return proto;
}

void MediaPipeDetection::_init() {}

Array MediaPipeDetection::get_categories() const {
	Array array;
	array.resize(detection.categories.size());
	for (int i = 0; i < array.size(); i++) {
		const Category &category = detection.categories[i];
		array[i] = MediaPipeCategory::_new(category);
	}
	return array;
}

Rect2 MediaPipeDetection::get_bounding_box() const {
	const Rect &rect = detection.bounding_box;
	return Rect2(
			rect.left, rect.top,
			rect.right - rect.left,
			rect.bottom - rect.top);
}

Array MediaPipeDetection::get_keypoints() const {
	Array array;
	if (Engine::get_singleton() && Engine::get_singleton()->is_editor_hint()) {
		if (!has_keypoints()) {
			return array;
		}
	}
	ERR_FAIL_COND_V(!has_keypoints(), array);
	const std::vector<NormalizedKeypoint> &keypoints = detection.keypoints.value();
	array.resize(keypoints.size());
	for (int i = 0; i < array.size(); i++) {
		const NormalizedKeypoint &keypoint = keypoints[i];
		array[i] = MediaPipeNormalizedKeypoint::_new(keypoint);
	}
	return array;
}

bool MediaPipeDetection::has_keypoints() const {
	return detection.keypoints.has_value();
}

Ref<MediaPipeProto> MediaPipeDetection::get_proto() {
	return Ref(MediaPipeProto::_new(to_proto(detection)));
}

Ref<MediaPipePacket> MediaPipeDetection::make_vector_proto_packet(Array array) {
	std::vector<ProtoType> vector;
	vector.reserve(array.size());
	for (int i = 0; i < array.size(); i++) {
		Ref<MediaPipeDetection> detection = array[i];
		ERR_BREAK(detection.is_null());
		vector.push_back(to_proto(detection->detection));
	}
	mediapipe::Packet packet = mediapipe::MakePacket<std::vector<ProtoType>>(vector);
	return Ref(MediaPipePacket::_new(packet));
}

void MediaPipeDetectionResult::_register_methods() {
	register_method("get_detections", &MediaPipeDetectionResult::get_detections);
	register_method("get_proto", &MediaPipeDetectionResult::get_proto);
	register_method("make_vector_proto_packet", &MediaPipeDetectionResult::make_vector_proto_packet);
}

MediaPipeDetectionResult *MediaPipeDetectionResult::_new(const DetectionResult &result) {
	MediaPipeDetectionResult *r = MediaPipeDetectionResult::_new();
	r->result = result;
	return r;
}

mediapipe::DetectionList MediaPipeDetectionResult::to_proto(const DetectionResult &result) {
	ProtoType proto;
	proto.mutable_detection()->Reserve(result.detections.size());
	for (const Detection &detection : result.detections) {
		proto.mutable_detection()->Add()->CopyFrom(MediaPipeDetection::to_proto(detection));
	}
	return proto;
}

void MediaPipeDetectionResult::_init() {}

Array MediaPipeDetectionResult::get_detections() const {
	Array array;
	array.resize(result.detections.size());
	for (int i = 0; i < array.size(); i++) {
		const Detection &detection = result.detections[i];
		array[i] = MediaPipeDetection::_new(detection);
	}
	return array;
}

Ref<MediaPipeProto> MediaPipeDetectionResult::get_proto() {
	return Ref(MediaPipeProto::_new(to_proto(result)));
}

Ref<MediaPipePacket> MediaPipeDetectionResult::make_vector_proto_packet(Array array) {
	std::vector<ProtoType> vector;
	vector.reserve(array.size());
	for (int i = 0; i < array.size(); i++) {
		Ref<MediaPipeDetectionResult> result = array[i];
		ERR_BREAK(result.is_null());
		vector.push_back(to_proto(result->result));
	}
	mediapipe::Packet packet = mediapipe::MakePacket<std::vector<ProtoType>>(vector);
	return Ref(MediaPipePacket::_new(packet));
}
