#ifndef GDMP_RECT
#define GDMP_RECT

#include "Reference.hpp"

#include "mediapipe/framework/formats/rect.pb.h"

#include "macros.h"

using namespace godot;

class Rect : public Reference {
		GDMP_PROTO_WRAPPER(Rect, mediapipe::Rect)
		GDMP_RAW_FIELD(x_center)
		GDMP_RAW_FIELD(y_center)
		GDMP_RAW_FIELD(width)
		GDMP_RAW_FIELD(height)
		GDMP_RAW_FIELD(rotation)
		GDMP_RAW_FIELD(rect_id)

	public:
		static void _register_methods() {
			GDMP_REGISTER_PROTO(Rect)
			GDMP_REGISTER_FIELD(x_center, Rect)
			GDMP_REGISTER_FIELD(y_center, Rect)
			GDMP_REGISTER_FIELD(width, Rect)
			GDMP_REGISTER_FIELD(height, Rect)
			GDMP_REGISTER_FIELD(rotation, Rect)
			GDMP_REGISTER_FIELD(rect_id, Rect)
		}
};

class NormalizedRect : public Reference {
		GDMP_PROTO_WRAPPER(NormalizedRect, mediapipe::NormalizedRect)
		GDMP_RAW_FIELD(x_center)
		GDMP_RAW_FIELD(y_center)
		GDMP_RAW_FIELD(width)
		GDMP_RAW_FIELD(height)
		GDMP_RAW_FIELD(rotation)
		GDMP_RAW_FIELD(rect_id)

	public:
		static void _register_methods() {
			GDMP_REGISTER_PROTO(NormalizedRect)
			GDMP_REGISTER_FIELD(x_center, NormalizedRect)
			GDMP_REGISTER_FIELD(y_center, NormalizedRect)
			GDMP_REGISTER_FIELD(width, NormalizedRect)
			GDMP_REGISTER_FIELD(height, NormalizedRect)
			GDMP_REGISTER_FIELD(rotation, NormalizedRect)
			GDMP_REGISTER_FIELD(rect_id, NormalizedRect)
		}
};

#endif
