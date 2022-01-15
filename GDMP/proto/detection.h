#ifndef GDMP_DETECTION
#define GDMP_DETECTION

#include <Godot.hpp>
#include <Reference.hpp>

#include "mediapipe/framework/formats/detection.pb.h"
#include "location_data.h"
#include "macros.h"

namespace godot {

class AssociatedDetection : public Reference {
    GDMP_PROTO_WRAPPER(AssociatedDetection, mediapipe::Detection::AssociatedDetection)
    GDMP_RAW_FIELD(id)
    GDMP_RAW_FIELD(confidence)

    public:
        static void _register_methods() {
            // id
            register_method("set_id", &AssociatedDetection::set_id);
            register_method("get_id", &AssociatedDetection::get_id);
            // confidence
            register_method("set_confidence", &AssociatedDetection::set_confidence);
            register_method("get_confidence", &AssociatedDetection::get_confidence);

            register_method("from_bytes", &AssociatedDetection::from_bytes);
            register_method("to_bytes", &AssociatedDetection::to_bytes);
        }
};

class Detection : public Reference {
    GDMP_PROTO_WRAPPER(Detection, mediapipe::Detection)
    GDMP_REPEATED_STRING_FIELD(label);
    GDMP_REPEATED_RAW_FIELD(label_id);
    GDMP_REPEATED_RAW_FIELD(score);
    GDMP_PROTO_FIELD(location_data, LocationData)
    GDMP_STRING_FIELD(feature_tag)
    GDMP_STRING_FIELD(track_id)
    GDMP_RAW_FIELD(detection_id)
    GDMP_REPEATED_PROTO_FIELD(associated_detections, AssociatedDetection)
    GDMP_REPEATED_STRING_FIELD(display_name)
    GDMP_RAW_FIELD(timestamp_usec)

    public:
        static void _register_methods() {
            // label
            register_method("label_size", &Detection::label_size);
            register_method("get_label", &Detection::get_label);
            // label_id
            register_method("label_id_size", &Detection::label_id_size);
            register_method("get_label_id", &Detection::get_label_id);
            // score
            register_method("score_size", &Detection::score_size);
            register_method("get_score", &Detection::get_score);
            // location_data
            register_method("get_location_data", &Detection::get_location_data);
            // feature_tag
            register_method("set_feature_tag", &Detection::set_feature_tag);
            register_method("get_feature_tag", &Detection::get_feature_tag);
            // track_id
            register_method("set_track_id", &Detection::set_track_id);
            register_method("get_track_id", &Detection::get_track_id);
            // detection_id
            register_method("set_detection_id", &Detection::set_detection_id);
            register_method("get_detection_id", &Detection::get_detection_id);
            // associated_detections
            register_method("associated_detections_size", &Detection::associated_detections_size);
            register_method("get_associated_detections", &Detection::get_associated_detections);
            // display_name
            register_method("display_name_size", &Detection::display_name_size);
            register_method("get_display_name", &Detection::get_display_name);
            // timestamp_usec
            register_method("set_timestamp_usec", &Detection::set_timestamp_usec);
            register_method("get_timestamp_usec", &Detection::get_timestamp_usec);

            register_method("from_bytes", &Detection::from_bytes);
            register_method("to_bytes", &Detection::to_bytes);
        }
};

class DetectionList : public Reference {
    GDMP_PROTO_WRAPPER(DetectionList, mediapipe::DetectionList)
    GDMP_REPEATED_PROTO_FIELD(detection, Detection)

    public:
        static void _register_methods() {
            // detection
            register_method("detection_size", &DetectionList::detection_size);
            register_method("get_detection", &DetectionList::get_detection);

            register_method("from_bytes", &DetectionList::from_bytes);
            register_method("to_bytes", &DetectionList::to_bytes);
        }
};

}

#endif
