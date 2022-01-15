#ifndef GDMP_RECT
#define GDMP_RECT

#include <Godot.hpp>
#include <Reference.hpp>

#include "mediapipe/framework/formats/rect.pb.h"
#include "macros.h"

namespace godot{

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
            // x_center
            register_method("set_x_center", &Rect::set_x_center);
            register_method("get_x_center", &Rect::get_x_center);
            // y_center
            register_method("set_y_center", &Rect::set_y_center);
            register_method("get_y_center", &Rect::get_y_center);
            // width
            register_method("set_width", &Rect::set_width);
            register_method("get_width", &Rect::get_width);
            // height
            register_method("set_height", &Rect::set_height);
            register_method("get_height", &Rect::get_height);
            // rotation
            register_method("set_rotation", &Rect::set_rotation);
            register_method("get_rotation", &Rect::get_rotation);
            // rect_id
            register_method("set_rect_id", &Rect::set_rect_id);
            register_method("get_rect_id", &Rect::get_rect_id);

            register_method("from_bytes", &Rect::from_bytes);
            register_method("to_bytes", &Rect::to_bytes);
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
            // x_center
            register_method("set_x_center", &NormalizedRect::set_x_center);
            register_method("get_x_center", &NormalizedRect::get_x_center);
            // y_center
            register_method("set_y_center", &NormalizedRect::set_y_center);
            register_method("get_y_center", &NormalizedRect::get_y_center);
            // width
            register_method("set_width", &NormalizedRect::set_width);
            register_method("get_width", &NormalizedRect::get_width);
            // height
            register_method("set_height", &NormalizedRect::set_height);
            register_method("get_height", &NormalizedRect::get_height);
            // rotation
            register_method("set_rotation", &NormalizedRect::set_rotation);
            register_method("get_rotation", &NormalizedRect::get_rotation);
            // rect_id
            register_method("set_rect_id", &NormalizedRect::set_rect_id);
            register_method("get_rect_id", &NormalizedRect::get_rect_id);

            register_method("from_bytes", &NormalizedRect::from_bytes);
            register_method("to_bytes", &NormalizedRect::to_bytes);
        }
};

}
#endif
