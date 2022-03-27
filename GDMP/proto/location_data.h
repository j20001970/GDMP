#ifndef GDMP_LOCATION_DATA
#define GDMP_LOCATION_DATA

#include "Reference.hpp"

#include "mediapipe/framework/formats/location_data.pb.h"

#include "rasterization.h"

#include "macros.h"

namespace godot {

class BoundingBox : public Reference {
		GDMP_PROTO_WRAPPER(BoundingBox, mediapipe::LocationData::BoundingBox)
		GDMP_RAW_FIELD(xmin)
		GDMP_RAW_FIELD(ymin)
		GDMP_RAW_FIELD(width)
		GDMP_RAW_FIELD(height)

	public:
		static void _register_methods() {
			GDMP_REGISTER_PROTO(BoundingBox)
			GDMP_REGISTER_FIELD(xmin, BoundingBox)
			GDMP_REGISTER_FIELD(ymin, BoundingBox)
			GDMP_REGISTER_FIELD(width, BoundingBox)
			GDMP_REGISTER_FIELD(height, BoundingBox)
		}
};

class RelativeBoundingBox : public Reference {
		GDMP_PROTO_WRAPPER(RelativeBoundingBox, mediapipe::LocationData::RelativeBoundingBox)
		GDMP_RAW_FIELD(xmin)
		GDMP_RAW_FIELD(ymin)
		GDMP_RAW_FIELD(width)
		GDMP_RAW_FIELD(height)

	public:
		static void _register_methods() {
			GDMP_REGISTER_PROTO(RelativeBoundingBox)
			GDMP_REGISTER_FIELD(xmin, RelativeBoundingBox)
			GDMP_REGISTER_FIELD(ymin, RelativeBoundingBox)
			GDMP_REGISTER_FIELD(width, RelativeBoundingBox)
			GDMP_REGISTER_FIELD(height, RelativeBoundingBox)
		}
};

class BinaryMask : public Reference {
		GDMP_PROTO_WRAPPER(BinaryMask, mediapipe::LocationData::BinaryMask)
		GDMP_RAW_FIELD(width)
		GDMP_RAW_FIELD(height)

	public:
		static void _register_methods() {
			GDMP_REGISTER_PROTO(BinaryMask)
			GDMP_REGISTER_FIELD(width, BinaryMask)
			GDMP_REGISTER_FIELD(height, BinaryMask)
		}
};

class RelativeKeypoint : public Reference {
		GDMP_PROTO_WRAPPER(RelativeKeypoint, mediapipe::LocationData::RelativeKeypoint)
		GDMP_RAW_FIELD(x)
		GDMP_RAW_FIELD(y)
		GDMP_STRING_FIELD(keypoint_label)
		GDMP_RAW_FIELD(score)

	public:
		static void _register_methods() {
			GDMP_REGISTER_PROTO(RelativeKeypoint)
			GDMP_REGISTER_FIELD(x, RelativeKeypoint)
			GDMP_REGISTER_FIELD(y, RelativeKeypoint)
			GDMP_REGISTER_FIELD(keypoint_label, RelativeKeypoint)
			GDMP_REGISTER_FIELD(score, RelativeKeypoint)
		}
};

class LocationData : public Reference {
		GDMP_PROTO_WRAPPER(LocationData, mediapipe::LocationData)
		void set_format(Variant value) {
			_data.set_format(static_cast<mediapipe::LocationData_Format>(static_cast<int>(value)));
		}
		Variant get_format() {
			return _data.format();
		}
		GDMP_PROTO_FIELD(bounding_box, BoundingBox)
		GDMP_PROTO_FIELD(relative_bounding_box, RelativeBoundingBox)
		GDMP_PROTO_FIELD(mask, BinaryMask)
		GDMP_REPEATED_PROTO_FIELD(relative_keypoints, RelativeKeypoint)

	public:
		static void _register_methods() {
			GDMP_REGISTER_PROTO(LocationData)
			GDMP_REGISTER_FIELD(format, LocationData);
			GDMP_REGISTER_FIELD(bounding_box, LocationData)
			GDMP_REGISTER_FIELD(relative_bounding_box, LocationData)
			GDMP_REGISTER_FIELD(mask, LocationData)
			GDMP_REGISTER_REPEATED_FIELD(relative_keypoints, LocationData)
		}
};

} //namespace godot

#endif
