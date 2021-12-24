#ifndef GDMP_H
#define GDMP_H

#include <cstdlib>
#include <mutex>
#include <thread>

#include "mediapipe/framework/calculator_framework.h"
#include "mediapipe/framework/calculator_graph.h"
#include "mediapipe/framework/formats/image_frame.h"
#include "mediapipe/framework/formats/image_frame_opencv.h"
#include "mediapipe/framework/formats/landmark.pb.h"
#include "mediapipe/framework/port/file_helpers.h"
#include "mediapipe/framework/port/opencv_highgui_inc.h"
#include "mediapipe/framework/port/opencv_imgproc_inc.h"
#include "mediapipe/framework/port/opencv_video_inc.h"
#include "mediapipe/framework/port/parse_text_proto.h"
#include "mediapipe/framework/port/status.h"
#include "mediapipe/gpu/gl_calculator_helper.h"
#include "mediapipe/gpu/gpu_buffer.h"
#include "mediapipe/gpu/gpu_shared_data_internal.h"

#include <Godot.hpp>
#include <Dictionary.hpp>
#include <Node.hpp>
#include <OS.hpp>

namespace godot {
constexpr char kInputStream[] = "input_video";
constexpr char kOutputStream[] = "output_video";
constexpr char kWindowName[] = "MediaPipe";

class GDMP : public Node {
    GODOT_CLASS(GDMP, Node)

    private:
        std::unique_ptr<mediapipe::CalculatorGraph> graph;
        std::thread camera_thread;
        std::thread graph_thread;
        String graph_path;
        bool grab_frames;
        std::mutex mutex;
        cv::VideoCapture capture;
        cv::Mat video_frame;
        cv::Mat output_video;
        Dictionary proto_packets;
        Dictionary proto_vector_packets;

    public:
        static void _register_methods();

        GDMP();
        ~GDMP();

        void _init(); // our initializer called by Godot

        void _process(float delta);

        void init_graph(String graph_path, Dictionary input_side_packets);
        void add_proto_callback(String stream_name);
        void add_proto_vector_callback(String stream_name);

        void start_camera(int index);
        void start_graph();
        void close_camera();

        void load_video(String path);
};
}

#endif
