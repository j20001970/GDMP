#ifndef GDMP_RASTERIZATION
#define GDMP_RASTERIZATION

#include <Godot.hpp>
#include <Reference.hpp>

#include "mediapipe/framework/formats/annotation/rasterization.pb.h"

#include "macros.h"

namespace godot {

class Interval : public Reference {
		GDMP_PROTO_WRAPPER(Interval, mediapipe::Rasterization::Interval)
		GDMP_RAW_FIELD(y)
		GDMP_RAW_FIELD(left_x)
		GDMP_RAW_FIELD(right_x)

	public:
		static void _register_methods() {
			// y
			register_method("set_y", &Interval::set_y);
			register_method("get_y", &Interval::get_y);
			// left_x
			register_method("set_left_x", &Interval::set_left_x);
			register_method("get_left_x", &Interval::get_left_x);
			// right_x
			register_method("set_right_x", &Interval::set_right_x);
			register_method("get_right_x", &Interval::get_right_x);

			register_method("from_bytes", &Interval::from_bytes);
			register_method("to_bytes", &Interval::to_bytes);
		}
};

class Rasterization : public Reference {
		GDMP_PROTO_WRAPPER(Rasterization, mediapipe::Rasterization)
		GDMP_REPEATED_PROTO_FIELD(interval, Interval)

	public:
		static void _register_methods() {
			// interval
			register_method("interval_size", &Rasterization::interval_size);
			register_method("get_interval", &Rasterization::get_interval);

			register_method("from_bytes", &Rasterization::from_bytes);
			register_method("to_bytes", &Rasterization::to_bytes);
		}
};

} //namespace godot

#endif
