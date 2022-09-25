#ifndef GDMP_RECT
#define GDMP_RECT

#include "godot_cpp/classes/ref_counted.hpp"

#include "mediapipe/framework/formats/rect.pb.h"

#include "macros.h"

namespace godot {

class Rect : public RefCounted {
		GDMP_PROTO_WRAPPER(Rect, mediapipe::Rect)
		GDMP_RAW_FIELD(x_center)
		GDMP_RAW_FIELD(y_center)
		GDMP_RAW_FIELD(width)
		GDMP_RAW_FIELD(height)
		GDMP_RAW_FIELD(rotation)
		GDMP_RAW_FIELD(rect_id)

	protected:
		static void _bind_methods() {
			GDMP_REGISTER_PROTO(Rect)
			GDMP_REGISTER_FIELD(x_center, Rect)
			GDMP_REGISTER_FIELD(y_center, Rect)
			GDMP_REGISTER_FIELD(width, Rect)
			GDMP_REGISTER_FIELD(height, Rect)
			GDMP_REGISTER_FIELD(rotation, Rect)
			GDMP_REGISTER_FIELD(rect_id, Rect)
		}
};

class NormalizedRect : public RefCounted {
		GDMP_PROTO_WRAPPER(NormalizedRect, mediapipe::NormalizedRect)
		GDMP_RAW_FIELD(x_center)
		GDMP_RAW_FIELD(y_center)
		GDMP_RAW_FIELD(width)
		GDMP_RAW_FIELD(height)
		GDMP_RAW_FIELD(rotation)
		GDMP_RAW_FIELD(rect_id)

	protected:
		static void _bind_methods() {
			GDMP_REGISTER_PROTO(NormalizedRect)
			GDMP_REGISTER_FIELD(x_center, NormalizedRect)
			GDMP_REGISTER_FIELD(y_center, NormalizedRect)
			GDMP_REGISTER_FIELD(width, NormalizedRect)
			GDMP_REGISTER_FIELD(height, NormalizedRect)
			GDMP_REGISTER_FIELD(rotation, NormalizedRect)
			GDMP_REGISTER_FIELD(rect_id, NormalizedRect)
		}
};

} //namespace godot
#endif
