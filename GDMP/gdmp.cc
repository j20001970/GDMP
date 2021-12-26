#include "gdmp.h"
#include "mediapipe/framework/deps/status_macros.h"

using namespace godot;

void GDMP::_register_methods() {
    register_method("initGraph", &GDMP::init_graph);
    register_method("addProtoCallback", &GDMP::add_proto_callback);
    register_method("addProtoVectorCallback", &GDMP::add_proto_vector_callback);
    register_method("startCamera", &GDMP::start_camera);
    register_method("closeCamera", &GDMP::close_camera);
    register_method("loadVideo", &GDMP::load_video);
    register_signal<GDMP>((char *)"on_new_proto", "stream_name", GODOT_VARIANT_TYPE_STRING, "proto_bytes", GODOT_VARIANT_TYPE_POOL_BYTE_ARRAY);
    register_signal<GDMP>((char *)"on_new_proto_vector", "stream_name", GODOT_VARIANT_TYPE_STRING, "proto_vector", GODOT_VARIANT_TYPE_ARRAY);
}

GDMP::GDMP() {
    graph = nullptr;
}

GDMP::~GDMP() {
    // add your cleanup here
}

void GDMP::_init() {
    // initialize any variables here
}

void GDMP::init_graph(String graph_path, Dictionary input_side_packets) {
    // setting input side packets is currently no-op
    close_camera();
    absl::Status result = [this, &graph_path]()->absl::Status {
        graph = absl::make_unique<mediapipe::CalculatorGraph>();
        std::string calculator_graph_config_contents;
        MP_RETURN_IF_ERROR(mediapipe::file::GetContents(
            graph_path.alloc_c_string(), &calculator_graph_config_contents));
        LOG(INFO) << "Get calculator graph config contents: "
                    << calculator_graph_config_contents;
        mediapipe::CalculatorGraphConfig config =
            mediapipe::ParseTextProtoOrDie<mediapipe::CalculatorGraphConfig>(calculator_graph_config_contents);
        Godot::print("Initialize the calculator graph.");
        MP_RETURN_IF_ERROR(graph->Initialize(config));
        Godot::print("Initialize the GPU.");
        ASSIGN_OR_RETURN(auto gpu_resources, mediapipe::GpuResources::Create());
        MP_RETURN_IF_ERROR(graph->SetGpuResources(std::move(gpu_resources)));
        return absl::OkStatus();
    }();
    if(!result.ok()) {
        Godot::print(result.message().data());
    }
}

void GDMP::add_proto_callback(String stream_name) {
    absl::Status result = [this, &stream_name]()->absl::Status {
        return graph->ObserveOutputStream(stream_name.alloc_c_string(), [this, stream_name](mediapipe::Packet packet)->absl::Status {
            PoolByteArray data;
            data.resize(packet.GetProtoMessageLite().ByteSizeLong());
            packet.GetProtoMessageLite().SerializeToArray(data.write().ptr(), data.size());
            call_deferred("emit_signal", Array::make("on_new_proto", stream_name, data));
            return absl::OkStatus();
        });
    }();
    if(!result.ok()) {
        Godot::print(result.message().data());
    }
}

void GDMP::add_proto_vector_callback(String stream_name) {
    absl::Status result = [this, &stream_name]()->absl::Status {
        return graph->ObserveOutputStream(stream_name.alloc_c_string(), [this, stream_name](mediapipe::Packet packet)->absl::Status {
            ASSIGN_OR_RETURN(auto proto_vector, packet.GetVectorOfProtoMessageLitePtrs());
            Array data;
            for(int i=0; i<proto_vector.size(); i++) {
                auto message = proto_vector[i];
                PoolByteArray proto_bytes;
                proto_bytes.resize(message->ByteSizeLong());
                message->SerializeToArray(proto_bytes.write().ptr(), proto_bytes.size());
                data.push_back(proto_bytes);
            }
            call_deferred("emit_signal", Array::make("on_new_proto_vector", stream_name, data));
            return absl::OkStatus();
        });
    }();
    if(!result.ok()) {
        Godot::print(result.message().data());
    }
}

void GDMP::start_camera(int index) {
    if(!capture.isOpened()){
        Godot::print("Initialize the camera or load the video.");
        capture.open(index);
        #if (CV_MAJOR_VERSION >= 3) && (CV_MINOR_VERSION >= 2)
        capture.set(cv::CAP_PROP_FRAME_WIDTH, 640);
        capture.set(cv::CAP_PROP_FRAME_HEIGHT, 480);
        capture.set(cv::CAP_PROP_FPS, 30);
        #endif
        if(capture.isOpened()){
            grab_frames = true;
            camera_thread = std::thread([this](){
                while(grab_frames) {
                    cv::Mat video_frame_raw;
                    capture >> video_frame_raw;
                    cv::flip(video_frame_raw, video_frame_raw, /*flipcode=HORIZONTAL*/ 1);
                    mutex.lock();
                    cv::cvtColor(video_frame_raw, video_frame, cv::COLOR_BGR2RGBA);
                    mutex.unlock();
                }
            });
            start_graph();
        }
        else{
            Godot::print("fail to open camera");
        }
    }
}

void GDMP::start_graph() {
    graph_thread = std::thread([this](){
        mediapipe::GlCalculatorHelper gpu_helper;
        gpu_helper.InitializeForTest(graph->GetGpuResources().get());
        absl::Status result = [this, &gpu_helper]()->absl::Status {
            MP_RETURN_IF_ERROR(graph->StartRun({}));

            Godot::print("Start grabbing and processing frames.");
            grab_frames = true;
            while (grab_frames) {
                if (video_frame.empty()) {
                    // Godot::print("Ignore empty frames from camera.");
                    continue;
                }
                mutex.lock();
                auto input_frame = absl::make_unique<mediapipe::ImageFrame>(
                    mediapipe::ImageFormat::SRGBA, video_frame.cols, video_frame.rows,
                    mediapipe::ImageFrame::kGlDefaultAlignmentBoundary);
                cv::Mat input_frame_mat = mediapipe::formats::MatView(input_frame.get());
                video_frame.copyTo(input_frame_mat);
                mutex.unlock();

                // Prepare and add graph input packet.
                size_t frame_timestamp_us =
                    (double)cv::getTickCount() / (double)cv::getTickFrequency() * 1e6;

                MP_RETURN_IF_ERROR(
                gpu_helper.RunInGlContext([this, &input_frame, &frame_timestamp_us, &gpu_helper]() -> absl::Status {
                // Convert ImageFrame to GpuBuffer.
                auto texture = gpu_helper.CreateSourceTexture(*input_frame.get());
                auto gpu_frame = texture.GetFrame<mediapipe::GpuBuffer>();
                glFlush();
                texture.Release();

                // Send GPU image packet into the graph.
                MP_RETURN_IF_ERROR(graph->AddPacketToInputStream(
                    kInputStream, mediapipe::Adopt(gpu_frame.release())
                                        .At(mediapipe::Timestamp(frame_timestamp_us))));
                return absl::OkStatus();
                }));
            }

            Godot::print("Shutting down.");
            MP_RETURN_IF_ERROR(graph->CloseInputStream(kInputStream));
            return graph->WaitUntilDone();
        }();
        if(!result.ok()) {
            Godot::print(result.message().data());
        }
    });
}

void GDMP::close_camera() {
    grab_frames = false;
    if(graph_thread.joinable()) {
        graph_thread.join();
    }
    if(camera_thread.joinable()) {
        camera_thread.join();
    }
    if(capture.isOpened()) {
        capture.release();
    }
}

void GDMP::load_video(String path) {
    if(!capture.isOpened()){
        Godot::print("Initialize the camera or load the video.");
        capture.open(path.alloc_c_string());
        if(capture.isOpened()){
            grab_frames = true;
            camera_thread = std::thread([this](){
                float delay = 1000 / capture.get(cv::CAP_PROP_FPS);
                while(grab_frames) {
                    cv::Mat video_frame_raw;
                    capture >> video_frame_raw;
                    if(video_frame_raw.empty()){
                        return;
                    }
                    mutex.lock();
                    cv::cvtColor(video_frame_raw, video_frame, cv::COLOR_BGR2RGBA);
                    mutex.unlock();
                    OS::get_singleton()->delay_msec(delay);
                }
            });
            start_graph();
        }
        else{
            Godot::print("fail to open camera");
        }
    }
}
