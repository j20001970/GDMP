#include "packet.h"

#include <vector>

#include "../util/image.h"

using namespace godot;

void Packet::_register_methods() {
	register_method("get_image", &Packet::get_image);
	register_method("get_proto", &Packet::get_proto);
	register_method("get_proto_vector", &Packet::get_proto_vector);
	register_method("make", &Packet::make);
	register_method("make_image", &Packet::make_image);
	register_method("make_classification", &Packet::make_classification);
	register_method("make_classification_list", &Packet::make_classification_list);
	register_method("make_classification_list_collection", &Packet::make_classification_list_collection);
	register_method("make_associated_detection", &Packet::make_associated_detection);
	register_method("make_detection", &Packet::make_detection);
	register_method("make_detection_list", &Packet::make_detection_list);
	register_method("make_landmark", &Packet::make_landmark);
	register_method("make_landmark_list", &Packet::make_landmark_list);
	register_method("make_landmark_list_collection", &Packet::make_landmark_list_collection);
	register_method("make_normalized_landmark", &Packet::make_normalized_landmark);
	register_method("make_normalized_landmark_list", &Packet::make_normalized_landmark_list);
	register_method("make_normalized_landmark_list_collection", &Packet::make_normalized_landmark_list_collection);
	register_method("make_bounding_box", &Packet::make_bounding_box);
	register_method("make_relative_bounding_box", &Packet::make_relative_bounding_box);
	register_method("make_binary_mask", &Packet::make_binary_mask);
	register_method("make_relative_keypoint", &Packet::make_relative_keypoint);
	register_method("make_location_data", &Packet::make_location_data);
	register_method("make_interval", &Packet::make_interval);
	register_method("make_rasterization", &Packet::make_rasterization);
	register_method("make_rect", &Packet::make_rect);
	register_method("make_normalized_rect", &Packet::make_normalized_rect);
	register_method("get_timestamp", &Packet::get_timestamp);
	register_method("set_timestamp", &Packet::set_timestamp);
}

Packet *Packet::_new(mediapipe::Packet packet) {
	Packet *p = Packet::_new();
	p->packet = packet;
	return p;
}

void Packet::_init(){};

Ref<Image> Packet::get_image() {
	auto &image_frame = get_packet().Get<mediapipe::ImageFrame>();
	return to_image(image_frame);
}

PoolByteArray Packet::get_proto() {
	PoolByteArray data;
	data.resize(packet.GetProtoMessageLite().ByteSizeLong());
	packet.GetProtoMessageLite().SerializeToArray(data.write().ptr(), data.size());
	return data;
}

Array Packet::get_proto_vector() {
	auto proto_vector = packet.GetVectorOfProtoMessageLitePtrs().value();
	Array data;
	for (int i = 0; i < proto_vector.size(); i++) {
		auto message = proto_vector[i];
		PoolByteArray proto_bytes;
		proto_bytes.resize(message->ByteSizeLong());
		message->SerializeToArray(proto_bytes.write().ptr(), proto_bytes.size());
		data.push_back(proto_bytes);
	}
	return data;
}

void Packet::make(Variant value) {
	switch (value.get_type()) {
		case Variant::Type::BOOL:
			packet = mediapipe::MakePacket<bool>(value);
			break;
		case Variant::Type::INT:
			packet = mediapipe::MakePacket<int>(value);
			break;
		case Variant::Type::REAL:
			packet = mediapipe::MakePacket<float>(value);
			break;
		case Variant::Type::STRING:
			packet = mediapipe::MakePacket<std::string>(String(value).alloc_c_string());
			break;
		default:
			break;
	}
}

void Packet::make_image(Ref<Image> image) {
	mediapipe::ImageFrame image_frame = to_image_frame(image);
	make_image_frame(image_frame);
}

void Packet::make_image_frame(const mediapipe::ImageFrame &image_frame) {
	packet = mediapipe::Adopt(&image_frame);
}

void Packet::make_classification(Ref<Classification> value) {
	packet = mediapipe::MakePacket<mediapipe::Classification>(value->data());
}

void Packet::make_classification_list(Ref<ClassificationList> value) {
	packet = mediapipe::MakePacket<mediapipe::ClassificationList>(value->data());
}

void Packet::make_classification_list_collection(Ref<ClassificationListCollection> value) {
	packet = mediapipe::MakePacket<mediapipe::ClassificationListCollection>(value->data());
}

void Packet::make_associated_detection(Ref<AssociatedDetection> value) {
	packet = mediapipe::MakePacket<mediapipe::Detection::AssociatedDetection>(value->data());
}

void Packet::make_detection(Ref<Detection> value) {
	packet = mediapipe::MakePacket<mediapipe::Detection>(value->data());
}

void Packet::make_detection_list(Ref<DetectionList> value) {
	packet = mediapipe::MakePacket<mediapipe::DetectionList>(value->data());
}

void Packet::make_landmark(Ref<Landmark> value) {
	packet = mediapipe::MakePacket<mediapipe::Landmark>(value->data());
}

void Packet::make_landmark_list(Ref<LandmarkList> value) {
	packet = mediapipe::MakePacket<mediapipe::LandmarkList>(value->data());
}

void Packet::make_landmark_list_collection(Ref<LandmarkListCollection> value) {
	packet = mediapipe::MakePacket<mediapipe::LandmarkListCollection>(value->data());
}

void Packet::make_normalized_landmark(Ref<NormalizedLandmark> value) {
	packet = mediapipe::MakePacket<mediapipe::NormalizedLandmark>(value->data());
}

void Packet::make_normalized_landmark_list(Ref<NormalizedLandmarkList> value) {
	packet = mediapipe::MakePacket<mediapipe::NormalizedLandmarkList>(value->data());
}

void Packet::make_normalized_landmark_list_collection(Ref<NormalizedLandmarkListCollection> value) {
	packet = mediapipe::MakePacket<mediapipe::NormalizedLandmarkListCollection>(value->data());
}

void Packet::make_bounding_box(Ref<BoundingBox> value) {
	packet = mediapipe::MakePacket<mediapipe::LocationData::BoundingBox>(value->data());
}

void Packet::make_relative_bounding_box(Ref<RelativeBoundingBox> value) {
	packet = mediapipe::MakePacket<mediapipe::LocationData::RelativeBoundingBox>(value->data());
}

void Packet::make_binary_mask(Ref<BinaryMask> value) {
	packet = mediapipe::MakePacket<mediapipe::LocationData::BinaryMask>(value->data());
}

void Packet::make_relative_keypoint(Ref<RelativeKeypoint> value) {
	packet = mediapipe::MakePacket<mediapipe::LocationData::RelativeKeypoint>(value->data());
}

void Packet::make_location_data(Ref<LocationData> value) {
	packet = mediapipe::MakePacket<mediapipe::LocationData>(value->data());
}

void Packet::make_interval(Ref<Interval> value) {
	packet = mediapipe::MakePacket<mediapipe::Rasterization::Interval>(value->data());
}

void Packet::make_rasterization(Ref<Rasterization> value) {
	packet = mediapipe::MakePacket<mediapipe::Rasterization>(value->data());
}

void Packet::make_rect(Ref<Rect> value) {
	packet = mediapipe::MakePacket<mediapipe::Rect>(value->data());
}

void Packet::make_normalized_rect(Ref<NormalizedRect> value) {
	packet = mediapipe::MakePacket<mediapipe::NormalizedRect>(value->data());
}

int64_t Packet::get_timestamp() {
	return packet.Timestamp().Microseconds();
}

void Packet::set_timestamp(int64_t timestamp) {
	packet = packet.At(mediapipe::Timestamp(timestamp));
}

mediapipe::Packet Packet::get_packet() {
	return packet;
}
