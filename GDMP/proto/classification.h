#ifndef GDMP_CLASSIFICATION
#define GDMP_CLASSIFICATION

#include "godot_cpp/classes/ref_counted.hpp"

#include "mediapipe/framework/formats/classification.pb.h"

#include "macros.h"

namespace godot {

class Classification : public RefCounted {
		GDMP_PROTO_WRAPPER(Classification, mediapipe::Classification)
		GDMP_RAW_FIELD(index)
		GDMP_RAW_FIELD(score)
		GDMP_STRING_FIELD(label)
		GDMP_STRING_FIELD(display_name)

	protected:
		static void _bind_methods() {
			GDMP_REGISTER_PROTO(Classification)
			GDMP_REGISTER_FIELD(index, Classification)
			GDMP_REGISTER_FIELD(score, Classification)
			GDMP_REGISTER_FIELD(label, Classification)
			GDMP_REGISTER_FIELD(display_name, Classification)
		}
};

class ClassificationList : public RefCounted {
		GDMP_PROTO_WRAPPER(ClassificationList, mediapipe::ClassificationList)
		GDMP_REPEATED_PROTO_FIELD(classification, Classification)

	public:
		static void _bind_methods() {
			GDMP_REGISTER_PROTO(ClassificationList)
			GDMP_REGISTER_REPEATED_FIELD(classification, ClassificationList)
		}
};

class ClassificationListCollection : public RefCounted {
		GDMP_PROTO_WRAPPER(ClassificationListCollection, mediapipe::ClassificationListCollection)
		GDMP_REPEATED_PROTO_FIELD(classification_list, ClassificationList)

	public:
		static void _bind_methods() {
			GDMP_REGISTER_PROTO(ClassificationListCollection)
			GDMP_REGISTER_REPEATED_FIELD(classification_list, ClassificationListCollection)
		}
};

} //namespace godot

#endif
