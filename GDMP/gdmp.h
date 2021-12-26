#ifndef GDMP_H
#define GDMP_H

#include <mutex>
#include <thread>

#include <Godot.hpp>
#include <Node.hpp>

#include "mediapipe/framework/calculator_framework.h"
#include "mediapipe/framework/port/opencv_core_inc.h"
#include "mediapipe/framework/port/opencv_video_inc.h"

namespace godot {
constexpr char kInputStream[] = "input_video";

class GDMP : public Node {
    GODOT_CLASS(GDMP, Node)

    private:
        std::unique_ptr<mediapipe::CalculatorGraph> graph;
        std::thread camera_thread;
        std::thread graph_thread;
        bool grab_frames;
        std::mutex mutex;
        cv::VideoCapture capture;
        cv::Mat video_frame;
        cv::Mat output_video;

    public:
        static void _register_methods();

        GDMP();
        ~GDMP();

        void _init(); // our initializer called by Godot

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
