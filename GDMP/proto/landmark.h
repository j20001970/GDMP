#ifndef GDMP_LANDMARK
#define GDMP_LANDMARK

#include "Reference.hpp"

#include "mediapipe/framework/formats/landmark.pb.h"

#include "macros.h"

using namespace godot;

class Landmark : public Reference {
		GDMP_PROTO_WRAPPER(Landmark, mediapipe::Landmark)
		GDMP_RAW_FIELD(x)
		GDMP_RAW_FIELD(y)
		GDMP_RAW_FIELD(z)
		GDMP_RAW_FIELD(visibility)
		GDMP_RAW_FIELD(presence)

	public:
		static void _register_methods() {
			GDMP_REGISTER_PROTO(Landmark)
			GDMP_REGISTER_FIELD(x, Landmark)
			GDMP_REGISTER_FIELD(y, Landmark)
			GDMP_REGISTER_FIELD(z, Landmark)
			GDMP_REGISTER_FIELD(visibility, Landmark)
			GDMP_REGISTER_FIELD(presence, Landmark)
		}
};

class LandmarkList : public Reference {
		GDMP_PROTO_WRAPPER(LandmarkList, mediapipe::LandmarkList)
		GDMP_REPEATED_PROTO_FIELD(landmark, Landmark)

	public:
		static void _register_methods() {
			GDMP_REGISTER_PROTO(LandmarkList)
			GDMP_REGISTER_REPEATED_FIELD(landmark, LandmarkList)
		}
};

class LandmarkListCollection : public Reference {
		GDMP_PROTO_WRAPPER(LandmarkListCollection, mediapipe::LandmarkListCollection)
		GDMP_REPEATED_PROTO_FIELD(landmark_list, LandmarkList)

	public:
		static void _register_methods() {
			GDMP_REGISTER_PROTO(LandmarkListCollection)
			GDMP_REGISTER_REPEATED_FIELD(landmark_list, LandmarkListCollection)
		}
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
			GDMP_REGISTER_PROTO(NormalizedLandmark)
			GDMP_REGISTER_FIELD(x, NormalizedLandmark)
			GDMP_REGISTER_FIELD(y, NormalizedLandmark)
			GDMP_REGISTER_FIELD(z, NormalizedLandmark)
			GDMP_REGISTER_FIELD(visibility, NormalizedLandmark)
			GDMP_REGISTER_FIELD(presence, NormalizedLandmark)
		}
};

class NormalizedLandmarkList : public Reference {
		GDMP_PROTO_WRAPPER(NormalizedLandmarkList, mediapipe::NormalizedLandmarkList)
		GDMP_REPEATED_PROTO_FIELD(landmark, NormalizedLandmark)

	public:
		static void _register_methods() {
			GDMP_REGISTER_PROTO(NormalizedLandmarkList)
			GDMP_REGISTER_REPEATED_FIELD(landmark, NormalizedLandmarkList)
		}
};

class NormalizedLandmarkListCollection : public Reference {
		GDMP_PROTO_WRAPPER(NormalizedLandmarkListCollection, mediapipe::NormalizedLandmarkListCollection)
		GDMP_REPEATED_PROTO_FIELD(landmark_list, NormalizedLandmarkList)

	public:
		static void _register_methods() {
			GDMP_REGISTER_PROTO(NormalizedLandmarkListCollection)
			GDMP_REGISTER_REPEATED_FIELD(landmark_list, NormalizedLandmarkListCollection)
		}
};

#endif
