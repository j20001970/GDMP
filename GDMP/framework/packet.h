#ifndef GDMP_PACKET
#define GDMP_PACKET

#include "Array.hpp"
#include "Godot.hpp"
#include "Image.hpp"
#include "PoolArrays.hpp"
#include "Ref.hpp"
#include "Reference.hpp"

#include "mediapipe/framework/formats/image_frame.h"
#include "mediapipe/framework/packet.h"

#include "../proto/classification.h"
#include "../proto/detection.h"
#include "../proto/landmark.h"
#include "../proto/location_data.h"
#include "../proto/rasterization.h"
#include "../proto/rect.h"

namespace godot {

class Packet : public Reference {
		GODOT_CLASS(Packet, Reference)

	public:
		static void _register_methods();
		static Packet *_new(mediapipe::Packet packet);

		void _init();

		// Get mediapipe::ImageFrame from packet and convert to godot::Image
		// For getting GPU frame, refer to GPUHelper::get_gpu_frame
		Ref<Image> get_image();
		// Get serialized proto bytes array from packet.
		PoolByteArray get_proto();
		// Get array of serialized proto bytes from packet,
		// only valid when packet is std::vector of proto type.
		Array get_proto_vector();

		// Make a packet (for bool, int, float and std::string).
		void make(Variant value);
		// Make a mediapipe::ImageFrame packet from godot::Image.
		// For making mediapipe::GpuBuffer packet, refer to GPUHelper::make_packet_from_image
		void make_image(Ref<Image> image);
		// Make a mediapipe::ImageFrame packet.
		void make_image_frame(const mediapipe::ImageFrame &image_frame);
		// Various make proto packet methods.
		// This is a workaround before we can pass GDNative-created packet objects back from GDScript without issues.
		void make_classification(Ref<Classification> value);
		void make_classification_list(Ref<ClassificationList> value);
		void make_classification_list_collection(Ref<ClassificationListCollection> value);
		void make_associated_detection(Ref<AssociatedDetection> value);
		void make_detection(Ref<Detection> value);
		void make_detection_list(Ref<DetectionList> value);
		void make_landmark(Ref<Landmark> value);
		void make_landmark_list(Ref<LandmarkList> value);
		void make_landmark_list_collection(Ref<LandmarkListCollection> value);
		void make_normalized_landmark(Ref<NormalizedLandmark> value);
		void make_normalized_landmark_list(Ref<NormalizedLandmarkList> value);
		void make_normalized_landmark_list_collection(Ref<NormalizedLandmarkListCollection> value);
		void make_bounding_box(Ref<BoundingBox> value);
		void make_relative_bounding_box(Ref<RelativeBoundingBox> value);
		void make_binary_mask(Ref<BinaryMask> value);
		void make_relative_keypoint(Ref<RelativeKeypoint> value);
		void make_location_data(Ref<LocationData> value);
		void make_interval(Ref<Interval> value);
		void make_rasterization(Ref<Rasterization> value);
		void make_rect(Ref<Rect> value);
		void make_normalized_rect(Ref<NormalizedRect> value);

		// Timestamp getter and setter
		int64 get_timestamp();
		void set_timestamp(int64_t timestamp);

		// Get actual mediapipe::Packet
		mediapipe::Packet get_packet();

	private:
		mediapipe::Packet packet;
};

} //namespace godot

#endif
