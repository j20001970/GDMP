#include "detection_result.h"

#include "godot_cpp/classes/engine.hpp"
#include "godot_cpp/core/class_db.hpp"
#include "godot_cpp/variant/variant.hpp"

void MediaPipeDetection::_bind_methods() {
	ClassDB::bind_static_method(MediaPipeDetection::get_class_static(), D_METHOD("make_vector_proto_packet", "array"), &MediaPipeDetection::make_vector_proto_packet);
	ClassDB::bind_method(D_METHOD("get_categories"), &MediaPipeDetection::get_categories);
	ClassDB::bind_method(D_METHOD("get_bounding_box"), &MediaPipeDetection::get_bounding_box);
	ClassDB::bind_method(D_METHOD("get_keypoints"), &MediaPipeDetection::get_keypoints);
	ClassDB::bind_method(D_METHOD("has_keypoints"), &MediaPipeDetection::has_keypoints);
	ClassDB::bind_method(D_METHOD("get_proto"), &MediaPipeDetection::get_proto);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "categories", PROPERTY_HINT_ARRAY_TYPE, MediaPipeCategory::get_class_static()), "", "get_categories");
	ADD_PROPERTY(PropertyInfo(Variant::RECT2I, "bounding_box"), "", "get_bounding_box");
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "keypoints", PROPERTY_HINT_ARRAY_TYPE, MediaPipeNormalizedKeypoint::get_class_static()), "", "get_keypoints");
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

Ref<MediaPipePacket> MediaPipeDetection::make_vector_proto_packet(TypedArray<MediaPipeDetection> array) {
	std::vector<ProtoType> vector;
	vector.reserve(array.size());
	for (int i = 0; i < array.size(); i++) {
		Ref<MediaPipeDetection> detection = array[i];
		ERR_BREAK(detection.is_null());
		vector.push_back(to_proto(detection->detection));
	}
	mediapipe::Packet packet = mediapipe::MakePacket<std::vector<ProtoType>>(vector);
	return memnew(MediaPipePacket(packet));
}

MediaPipeDetection::MediaPipeDetection() = default;

MediaPipeDetection::MediaPipeDetection(const Detection &detection) {
	this->detection = detection;
}

TypedArray<MediaPipeCategory> MediaPipeDetection::get_categories() const {
	TypedArray<MediaPipeCategory> array;
	array.resize(detection.categories.size());
	for (int i = 0; i < array.size(); i++) {
		const Category &category = detection.categories[i];
		array[i] = memnew(MediaPipeCategory(category));
	}
	return array;
}

Rect2i MediaPipeDetection::get_bounding_box() const {
	const Rect &rect = detection.bounding_box;
	return Rect2i(
			rect.left, rect.top,
			rect.right - rect.left,
			rect.bottom - rect.top);
}

TypedArray<MediaPipeNormalizedKeypoint> MediaPipeDetection::get_keypoints() const {
	TypedArray<MediaPipeNormalizedKeypoint> array;
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
		array[i] = memnew(MediaPipeNormalizedKeypoint(keypoint));
	}
	return array;
}

bool MediaPipeDetection::has_keypoints() const {
	return detection.keypoints.has_value();
}

Ref<MediaPipeProto> MediaPipeDetection::get_proto() {
	return memnew(MediaPipeProto(to_proto(detection)));
}

void MediaPipeDetectionResult::_bind_methods() {
	ClassDB::bind_static_method(MediaPipeDetectionResult::get_class_static(), D_METHOD("make_vector_proto_packet", "array"), &MediaPipeDetectionResult::make_vector_proto_packet);
	ClassDB::bind_method(D_METHOD("get_detections"), &MediaPipeDetectionResult::get_detections);
	ClassDB::bind_method(D_METHOD("get_proto"), &MediaPipeDetectionResult::get_proto);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "detections", PROPERTY_HINT_ARRAY_TYPE, MediaPipeDetection::get_class_static()), "", "get_detections");
}

mediapipe::DetectionList MediaPipeDetectionResult::to_proto(const DetectionResult &result) {
	ProtoType proto;
	proto.mutable_detection()->Reserve(result.detections.size());
	for (const Detection &detection : result.detections) {
		proto.mutable_detection()->Add()->CopyFrom(MediaPipeDetection::to_proto(detection));
	}
	return proto;
}

Ref<MediaPipePacket> MediaPipeDetectionResult::make_vector_proto_packet(TypedArray<MediaPipeDetectionResult> array) {
	std::vector<ProtoType> vector;
	vector.reserve(array.size());
	for (int i = 0; i < array.size(); i++) {
		Ref<MediaPipeDetectionResult> result = array[i];
		ERR_BREAK(result.is_null());
		vector.push_back(to_proto(result->result));
	}
	mediapipe::Packet packet = mediapipe::MakePacket<std::vector<ProtoType>>(vector);
	return memnew(MediaPipePacket(packet));
}

MediaPipeDetectionResult::MediaPipeDetectionResult() = default;

MediaPipeDetectionResult::MediaPipeDetectionResult(const DetectionResult &result) {
	this->result = result;
}

TypedArray<MediaPipeDetection> MediaPipeDetectionResult::get_detections() const {
	TypedArray<MediaPipeDetection> array;
	array.resize(result.detections.size());
	for (int i = 0; i < array.size(); i++) {
		const Detection &detection = result.detections[i];
		array[i] = memnew(MediaPipeDetection(detection));
	}
	return array;
}

Ref<MediaPipeProto> MediaPipeDetectionResult::get_proto() {
	return memnew(MediaPipeProto(to_proto(result)));
}
