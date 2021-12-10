#ifndef NO_GDMP
#include "gdmp.h"
#endif
#include "proto/landmark.h"

extern "C" void GDN_EXPORT godot_gdnative_init(godot_gdnative_init_options *o) {
    godot::Godot::gdnative_init(o);
}

extern "C" void GDN_EXPORT godot_gdnative_singleton() {
    
}

extern "C" void GDN_EXPORT godot_gdnative_terminate(godot_gdnative_terminate_options *o) {
    godot::Godot::gdnative_terminate(o);
}

extern "C" void GDN_EXPORT godot_nativescript_init(void *handle) {
    godot::Godot::nativescript_init(handle);
    #ifndef NO_GDMP
    godot::register_class<godot::GDMP>();
    #endif
    godot::register_class<godot::NormalizedLandmark>();
    godot::register_class<godot::NormalizedLandmarkList>();
}
