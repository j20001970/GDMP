#ifndef GDMP_LOCATION_DATA
#define GDMP_LOCATION_DATA

#include <Godot.hpp>
#include <Reference.hpp>

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
			// xmin
			register_method("set_xmin", &BoundingBox::set_xmin);
			register_method("get_xmin", &BoundingBox::get_xmin);
			// ymin
			register_method("set_ymin", &BoundingBox::set_ymin);
			register_method("get_ymin", &BoundingBox::get_ymin);
			// width
			register_method("set_width", &BoundingBox::set_width);
			register_method("get_width", &BoundingBox::get_width);
			// height
			register_method("set_height", &BoundingBox::set_height);
			register_method("get_height", &BoundingBox::get_height);

			register_method("from_bytes", &BoundingBox::from_bytes);
			register_method("to_bytes", &BoundingBox::to_bytes);
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
			// xmin
			register_method("set_xmin", &RelativeBoundingBox::set_xmin);
			register_method("get_xmin", &RelativeBoundingBox::get_xmin);
			// ymin
			register_method("set_ymin", &RelativeBoundingBox::set_ymin);
			register_method("get_ymin", &RelativeBoundingBox::get_ymin);
			// width
			register_method("set_width", &RelativeBoundingBox::set_width);
			register_method("get_width", &RelativeBoundingBox::get_width);
			// height
			register_method("set_height", &RelativeBoundingBox::set_height);
			register_method("get_height", &RelativeBoundingBox::get_height);

			register_method("from_bytes", &RelativeBoundingBox::from_bytes);
			register_method("to_bytes", &RelativeBoundingBox::to_bytes);
		}
};

class BinaryMask : public Reference {
		GDMP_PROTO_WRAPPER(BinaryMask, mediapipe::LocationData::BinaryMask)
		GDMP_RAW_FIELD(width)
		GDMP_RAW_FIELD(height)

	public:
		static void _register_methods() {
			// width
			register_method("set_width", &BinaryMask::set_width);
			register_method("get_width", &BinaryMask::get_width);
			// height
			register_method("set_height", &BinaryMask::set_height);
			register_method("get_height", &BinaryMask::get_height);

			register_method("from_bytes", &BinaryMask::from_bytes);
			register_method("to_bytes", &BinaryMask::to_bytes);
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
			// x
			register_method("set_x", &RelativeKeypoint::set_x);
			register_method("get_x", &RelativeKeypoint::get_x);
			// y
			register_method("set_y", &RelativeKeypoint::set_y);
			register_method("get_y", &RelativeKeypoint::get_y);
			// keypoint_label
			register_method("set_keypoint_label", &RelativeKeypoint::set_keypoint_label);
			register_method("get_keypoint_label", &RelativeKeypoint::get_keypoint_label);
			// score
			register_method("set_score", &RelativeKeypoint::set_score);
			register_method("get_score", &RelativeKeypoint::get_score);

			register_method("from_bytes", &BinaryMask::from_bytes);
			register_method("to_bytes", &BinaryMask::to_bytes);
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
			// format
			register_method("set_format", &LocationData::set_format);
			register_method("get_format", &LocationData::get_format);
			// bounding_box
			register_method("get_bounding_box", &LocationData::get_bounding_box);
			// relative_bounding_box
			register_method("get_relative_bounding_box", &LocationData::get_relative_bounding_box);
			// mask
			register_method("get_mask", &LocationData::get_mask);
			// relative_keypoints
			register_method("relative_keypoints_size", &LocationData::relative_keypoints_size);
			register_method("get_relative_keypoints", &LocationData::get_relative_keypoints);

			register_method("from_bytes", &LocationData::from_bytes);
			register_method("to_bytes", &LocationData::to_bytes);
		}
};

} //namespace godot

#endif
