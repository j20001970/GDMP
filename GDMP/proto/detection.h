#ifndef GDMP_DETECTION
#define GDMP_DETECTION

#include "godot_cpp/classes/ref_counted.hpp"

#include "mediapipe/framework/formats/detection.pb.h"

#include "location_data.h"

#include "macros.h"

using namespace godot;

class AssociatedDetection : public RefCounted {
		GDMP_PROTO_WRAPPER(AssociatedDetection, mediapipe::Detection::AssociatedDetection)
		GDMP_RAW_FIELD(id)
		GDMP_RAW_FIELD(confidence)

	protected:
		static void _bind_methods() {
			GDMP_REGISTER_PROTO(AssociatedDetection)
			GDMP_REGISTER_FIELD(id, AssociatedDetection)
			GDMP_REGISTER_FIELD(confidence, AssociatedDetection)
		}
};

class Detection : public RefCounted {
		GDMP_PROTO_WRAPPER(Detection, mediapipe::Detection)
		GDMP_REPEATED_STRING_FIELD(label);
		GDMP_REPEATED_RAW_FIELD(label_id);
		GDMP_REPEATED_RAW_FIELD(score);
		GDMP_PROTO_FIELD(location_data, LocationData)
		GDMP_STRING_FIELD(feature_tag)
		GDMP_STRING_FIELD(track_id)
		GDMP_RAW_FIELD(detection_id)
		GDMP_REPEATED_PROTO_FIELD(associated_detections, AssociatedDetection)
		GDMP_REPEATED_STRING_FIELD(display_name)
		GDMP_RAW_FIELD(timestamp_usec)

	protected:
		static void _bind_methods() {
			GDMP_REGISTER_PROTO(Detection)
			GDMP_REGISTER_REPEATED_FIELD(label, Detection)
			GDMP_REGISTER_REPEATED_FIELD(label_id, Detection)
			GDMP_REGISTER_REPEATED_FIELD(score, Detection)
			GDMP_REGISTER_FIELD(location_data, Detection)
			GDMP_REGISTER_FIELD(feature_tag, Detection)
			GDMP_REGISTER_FIELD(track_id, Detection)
			GDMP_REGISTER_FIELD(detection_id, Detection)
			GDMP_REGISTER_REPEATED_FIELD(associated_detections, Detection)
			GDMP_REGISTER_REPEATED_FIELD(display_name, Detection)
			GDMP_REGISTER_FIELD(timestamp_usec, Detection)
		}
};

class DetectionList : public RefCounted {
		GDMP_PROTO_WRAPPER(DetectionList, mediapipe::DetectionList)
		GDMP_REPEATED_PROTO_FIELD(detection, Detection)

	protected:
		static void _bind_methods() {
			GDMP_REGISTER_PROTO(DetectionList)
			GDMP_REGISTER_REPEATED_FIELD(detection, DetectionList)
		}
};

#endif
