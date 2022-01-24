#ifndef GDMP_H
#define GDMP_H

#include <memory>
#include <thread>

#include <Godot.hpp>
#include <Dictionary.hpp>
#include <Node.hpp>

#include "mediapipe/framework/calculator_framework.h"
#include "mediapipe/framework/formats/image_frame.h"
#include "mediapipe/framework/port/opencv_core_inc.h"
#include "mediapipe/framework/port/opencv_video_inc.h"
#include "mediapipe/framework/port/status.h"
#include "mediapipe/gpu/gl_calculator_helper.h"

namespace godot {
class GDMP : public Node {
    GODOT_CLASS(GDMP, Node)

    public:
        static void _register_methods();

        GDMP();
        ~GDMP();

        void _init(); // our initializer called by Godot

        void init_graph(String graph_path);
        void add_proto_callback(String stream_name);
        void add_proto_vector_callback(String stream_name);
        void add_gpu_frame_callback(String stream_name);

        void start_graph(Dictionary side_packets);

        void start_camera(int index, String stream_name);
        void close_camera();
        absl::Status send_video_frame(std::unique_ptr<mediapipe::ImageFrame> video_frame, String stream_name);

        void load_video(String path);

    private:
        std::unique_ptr<mediapipe::CalculatorGraph> graph;
        std::unique_ptr<mediapipe::GlCalculatorHelper> gpu_helper;
        std::thread camera_thread;
        bool grab_frames;
        String video_stream;
        cv::VideoCapture capture;
};
}

#endif
