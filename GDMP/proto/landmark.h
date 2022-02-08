#ifndef GDMP_LANDMARK
#define GDMP_LANDMARK

#include <Godot.hpp>
#include <Reference.hpp>

#include "mediapipe/framework/formats/landmark.pb.h"

#include "macros.h"

namespace godot {

class Landmark : public Reference {
		GDMP_PROTO_WRAPPER(Landmark, mediapipe::Landmark)
		GDMP_RAW_FIELD(x)
		GDMP_RAW_FIELD(y)
		GDMP_RAW_FIELD(z)
		GDMP_RAW_FIELD(visibility)
		GDMP_RAW_FIELD(presence)

	public:
		static void _register_methods() {
			// x
			register_method("set_x", &Landmark::set_x);
			register_method("get_x", &Landmark::get_x);
			// y
			register_method("set_y", &Landmark::set_y);
			register_method("get_y", &Landmark::get_y);
			// z
			register_method("set_z", &Landmark::set_z);
			register_method("get_z", &Landmark::get_z);
			// visibility
			register_method("set_visibility", &Landmark::set_visibility);
			register_method("get_visibility", &Landmark::get_visibility);
			// presence
			register_method("set_presence", &Landmark::set_presence);
			register_method("get_presence", &Landmark::get_presence);

			register_method("from_bytes", &Landmark::from_bytes);
			register_method("to_bytes", &Landmark::to_bytes);
		}
};

class LandmarkList : public Reference {
		GDMP_PROTO_WRAPPER(LandmarkList, mediapipe::LandmarkList)
		GDMP_REPEATED_PROTO_FIELD(landmark, Landmark)

	public:
		static void _register_methods() {
			// landmark
			register_method("landmark_size", &LandmarkList::landmark_size);
			register_method("get_landmark", &LandmarkList::get_landmark);

			register_method("from_bytes", &LandmarkList::from_bytes);
			register_method("to_bytes", &LandmarkList::to_bytes);
		};
};

class LandmarkListCollection : public Reference {
		GDMP_PROTO_WRAPPER(LandmarkListCollection, mediapipe::LandmarkListCollection)
		GDMP_REPEATED_PROTO_FIELD(landmark_list, LandmarkList)

	public:
		static void _register_methods() {
			// landmark_list
			register_method("landmark_list_size", &LandmarkListCollection::landmark_list_size);
			register_method("get_landmark_list", &LandmarkListCollection::get_landmark_list);

			register_method("from_bytes", &LandmarkListCollection::from_bytes);
			register_method("to_bytes", &LandmarkListCollection::to_bytes);
		};
};

class NormalizedLandmark : public Reference {
		GDMP_PROTO_WRAPPER(NormalizedLandmark, mediapipe::NormalizedLandmark)
		GDMP_RAW_FIELD(x)
		GDMP_RAW_FIELD(y)
		GDMP_RAW_FIELD(z)
		GDMP_RAW_FIELD(visibility)
		GDMP_RAW_FIELD(presence)

	public:
		static void _register_methods() {
			// x
			register_method("set_x", &NormalizedLandmark::set_x);
			register_method("get_x", &NormalizedLandmark::get_x);
			// y
			register_method("set_y", &NormalizedLandmark::set_y);
			register_method("get_y", &NormalizedLandmark::get_y);
			// z
			register_method("set_z", &NormalizedLandmark::set_z);
			register_method("get_z", &NormalizedLandmark::get_z);
			// visibility
			register_method("set_visibility", &NormalizedLandmark::set_visibility);
			register_method("get_visibility", &NormalizedLandmark::get_visibility);
			// presence
			register_method("set_presence", &NormalizedLandmark::set_presence);
			register_method("get_presence", &NormalizedLandmark::get_presence);

			register_method("from_bytes", &NormalizedLandmark::from_bytes);
			register_method("to_bytes", &NormalizedLandmark::to_bytes);
		};
};

class NormalizedLandmarkList : public Reference {
		GDMP_PROTO_WRAPPER(NormalizedLandmarkList, mediapipe::NormalizedLandmarkList)
		GDMP_REPEATED_PROTO_FIELD(landmark, NormalizedLandmark)

	public:
		static void _register_methods() {
			// landmark
			register_method("landmark_size", &NormalizedLandmarkList::landmark_size);
			register_method("get_landmark", &NormalizedLandmarkList::get_landmark);

			register_method("from_bytes", &NormalizedLandmarkList::from_bytes);
			register_method("to_bytes", &NormalizedLandmarkList::to_bytes);
		};
};

class NormalizedLandmarkListCollection : public Reference {
		GDMP_PROTO_WRAPPER(NormalizedLandmarkListCollection, mediapipe::NormalizedLandmarkListCollection)
		GDMP_REPEATED_PROTO_FIELD(landmark_list, NormalizedLandmarkList)

	public:
		static void _register_methods() {
			// landmark_list
			register_method("landmark_list_size", &NormalizedLandmarkListCollection::landmark_list_size);
			register_method("get_landmark_list", &NormalizedLandmarkListCollection::get_landmark_list);

			register_method("from_bytes", &NormalizedLandmarkListCollection::from_bytes);
			register_method("to_bytes", &NormalizedLandmarkListCollection::to_bytes);
		};
};

} //namespace godot

#endif
