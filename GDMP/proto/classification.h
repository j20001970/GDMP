#ifndef GDMP_CLASSIFICATION
#define GDMP_CLASSIFICATION

#include <Godot.hpp>
#include <Reference.hpp>

#include "mediapipe/framework/formats/classification.pb.h"
#include "macros.h"

namespace godot {

class Classification : public Reference {
    GDMP_PROTO_WRAPPER(Classification, mediapipe::Classification)
    GDMP_RAW_FIELD(index)
    GDMP_RAW_FIELD(score)
    GDMP_STRING_FIELD(label)
    GDMP_STRING_FIELD(display_name)

    public:
        static void _register_methods() {
            // index
            register_method("set_index", &Classification::set_index);
            register_method("get_index", &Classification::get_index);
            // score
            register_method("set_score", &Classification::set_score);
            register_method("get_score", &Classification::get_score);
            // label
            register_method("set_label", &Classification::get_label);
            register_method("get_label", &Classification::get_label);
            // display_name
            register_method("set_display_name", &Classification::set_display_name);
            register_method("get_display_name", &Classification::get_display_name);

            register_method("from_bytes", &Classification::from_bytes);
            register_method("to_bytes", &Classification::to_bytes);
        }
};

class ClassificationList : public Reference {
    GDMP_PROTO_WRAPPER(ClassificationList, mediapipe::ClassificationList)
    GDMP_REPEATED_PROTO_FIELD(classification, Classification)

    public:
        static void _register_methods() {
            //classification
            register_method("classification_size", &ClassificationList::classification_size);
            register_method("get_classification", &ClassificationList::get_classification);

            register_method("from_bytes", &ClassificationList::from_bytes);
            register_method("to_bytes", &ClassificationList::to_bytes);
        }
};

class ClassificationListCollection : public Reference {
    GDMP_PROTO_WRAPPER(ClassificationListCollection, mediapipe::ClassificationListCollection)
    GDMP_REPEATED_PROTO_FIELD(classification_list, ClassificationList)

    public:
        static void _register_methods() {
            //classification
            register_method("classification_list_size", &ClassificationListCollection::classification_list_size);
            register_method("get_classification_list", &ClassificationListCollection::get_classification_list);

            register_method("from_bytes", &ClassificationListCollection::from_bytes);
            register_method("to_bytes", &ClassificationListCollection::to_bytes);
        }
};

}

#endif
