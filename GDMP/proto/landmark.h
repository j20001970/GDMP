#ifndef GDMP_LANDMARK
#define GDMP_LANDMARK

#include <Godot.hpp>
#include <Reference.hpp>
#include <Variant.hpp>

#include "mediapipe/framework/formats/landmark.pb.h"
#include "macros.h"

namespace godot {

class NormalizedLandmark : public Reference {
    GODOT_CLASS(NormalizedLandmark, Reference)
    GDMP_PROTO_WRAPPER(NormalizedLandmark, mediapipe::NormalizedLandmark)

    public:
        static void _register_methods() {
            register_method("from_bytes", &NormalizedLandmark::from_bytes);
            register_method("get_x", &NormalizedLandmark::get_x);
            register_method("get_y", &NormalizedLandmark::get_y);
            register_method("get_z", &NormalizedLandmark::get_z);
            register_method("get_visibility", &NormalizedLandmark::get_visibility);
            register_method("get_presence", &NormalizedLandmark::get_presence);
        };

        void _init() {

        };

        void _init(mediapipe::NormalizedLandmark data) {
            _data = data;
        }

        Variant get_x() {
            if(_data.has_x()) {
                return _data.x();
            }
            else {
                return Variant();
            }
        }

        Variant get_y() {
            if(_data.has_y()) {
                return _data.y();
            }
            else {
                return Variant();
            }
        }

        Variant get_z() {
            if(_data.has_z()) {
                return _data.z();
            }
            else {
                return Variant();
            }
        }

        Variant get_visibility() {
            if(_data.has_visibility()) {
                return _data.visibility();
            }
            else {
                return Variant();
            }
        }

        Variant get_presence() {
            if(_data.has_presence()) {
                return _data.presence();
            }
            else {
                return Variant();
            }
        }
};

class NormalizedLandmarkList : public Reference {
    GODOT_CLASS(NormalizedLandmarkList, Reference)
    GDMP_PROTO_WRAPPER(NormalizedLandmarkList, mediapipe::NormalizedLandmarkList)

    public:
        static void _register_methods() {
            register_method("from_bytes", &NormalizedLandmarkList::from_bytes);
            register_method("landmark_size", &NormalizedLandmarkList::landmark_size);
            register_method("get_landmark", &NormalizedLandmarkList::get_landmark);
        };

        void _init() {

        };

        int landmark_size() {
            return _data.landmark_size();
        }

        Ref<NormalizedLandmark> get_landmark(int index) {
            NormalizedLandmark *landmark = NormalizedLandmark()._new();
            landmark->_init(_data.landmark(index));
            return Ref<NormalizedLandmark>(landmark);
        }

};

}

#endif
