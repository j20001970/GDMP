#ifndef GDMP_RASTERIZATION
#define GDMP_RASTERIZATION

#include "godot_cpp/classes/ref_counted.hpp"

#include "mediapipe/framework/formats/annotation/rasterization.pb.h"

#include "macros.h"

namespace godot {

class Interval : public RefCounted {
		GDMP_PROTO_WRAPPER(Interval, mediapipe::Rasterization::Interval)
		GDMP_RAW_FIELD(y)
		GDMP_RAW_FIELD(left_x)
		GDMP_RAW_FIELD(right_x)

	protected:
		static void _bind_methods() {
			GDMP_REGISTER_PROTO(Interval)
			GDMP_REGISTER_FIELD(y, Interval)
			GDMP_REGISTER_FIELD(left_x, Interval)
			GDMP_REGISTER_FIELD(right_x, Interval)
		}
};

class Rasterization : public RefCounted {
		GDMP_PROTO_WRAPPER(Rasterization, mediapipe::Rasterization)
		GDMP_REPEATED_PROTO_FIELD(interval, Interval)

	protected:
		static void _bind_methods() {
			GDMP_REGISTER_PROTO(Rasterization)
			GDMP_REGISTER_REPEATED_FIELD(interval, Rasterization)
		}
};

} //namespace godot

#endif
