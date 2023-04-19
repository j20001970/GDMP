#ifndef GDMP_RASTERIZATION
#define GDMP_RASTERIZATION

#include "Reference.hpp"

#include "mediapipe/framework/formats/annotation/rasterization.pb.h"

#include "macros.h"

using namespace godot;

class Interval : public Reference {
		GDMP_PROTO_WRAPPER(Interval, mediapipe::Rasterization::Interval)
		GDMP_RAW_FIELD(y)
		GDMP_RAW_FIELD(left_x)
		GDMP_RAW_FIELD(right_x)

	public:
		static void _register_methods() {
			GDMP_REGISTER_PROTO(Interval)
			GDMP_REGISTER_FIELD(y, Interval)
			GDMP_REGISTER_FIELD(left_x, Interval)
			GDMP_REGISTER_FIELD(right_x, Interval)
		}
};

class Rasterization : public Reference {
		GDMP_PROTO_WRAPPER(Rasterization, mediapipe::Rasterization)
		GDMP_REPEATED_PROTO_FIELD(interval, Interval)

	public:
		static void _register_methods() {
			GDMP_REGISTER_PROTO(Rasterization)
			GDMP_REGISTER_REPEATED_FIELD(interval, Rasterization)
		}
};

#endif
