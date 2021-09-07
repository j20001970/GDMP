#include "gdmp.h"

using namespace godot;

void GDMP::_register_methods() {
    register_method("_process", &GDMP::_process);
    register_method("initGraph", &GDMP::init_graph);
    register_method("addPacketCallback", &GDMP::add_packet_callback);
    register_method("startCamera", &GDMP::start_camera);
    register_method("closeCamera", &GDMP::close_camera);
    register_method("loadVideo", &GDMP::load_video);
    register_signal<GDMP>((char *)"new_frame", "data", GODOT_VARIANT_TYPE_POOL_REAL_ARRAY, "width", GODOT_VARIANT_TYPE_INT, "height", GODOT_VARIANT_TYPE_INT);
    register_signal<GDMP>((char *)"new_packet", "stream_name", GODOT_VARIANT_TYPE_STRING, "is_vector", GODOT_VARIANT_TYPE_BOOL, "data", GODOT_VARIANT_TYPE_POOL_REAL_ARRAY);
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

void GDMP::_process(float delta) {
    mutex.lock();
    PoolStringArray streams = PoolStringArray(packet_data.keys());
    for(int i=0; i<streams.size(); i++) {
        String stream_name = streams[i];
        bool is_vector = static_cast<Dictionary>(packet_data[stream_name])["is_vector"];
        PoolRealArray data = static_cast<Dictionary>(packet_data[stream_name])["data"];
        emit_signal("new_packet", stream_name, is_vector, data);
        packet_data.erase(streams[i]);
    }
    if(!video_frame.empty()){
        PoolByteArray data = PoolByteArray();
        int buf_size = video_frame.cols*video_frame.rows*video_frame.channels();
        data.resize(buf_size);
        memcpy(data.write().ptr(), video_frame.ptr<uint8_t>(), buf_size);
        emit_signal("new_frame", data, video_frame.cols, video_frame.rows);
    }
    if(!output_video.empty()) {
        cv::imshow(kWindowName, output_video);
        cv::waitKey(1);
    }
    mutex.unlock();
}

void GDMP::init_graph(String graph_path, Dictionary input_side_packets) {
    // setting input side packets is currently no-op
    close_camera();
    packet_data.clear();
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

void GDMP::add_packet_callback(String stream_name, bool is_vector) {
    absl::Status result = [this, &stream_name, &is_vector]()->absl::Status {
        return graph->ObserveOutputStream(stream_name.alloc_c_string(), [this, stream_name, is_vector](mediapipe::Packet packet)->absl::Status{
            mutex.lock();
            if(is_vector) {
                auto& landmarks_list = packet.Get<std::vector<mediapipe::NormalizedLandmarkList>>();
                Dictionary godot_data;
                godot_data["is_vector"] = is_vector;
                PoolRealArray data;
                data.push_back(landmarks_list.size());
                for(int i=0; i<landmarks_list.size(); i++) {
                    auto& landmarks = landmarks_list[i];
                    data.push_back(landmarks.landmark_size());
                    for(int j=0; j<landmarks.landmark_size(); j++) {
                        auto& landmark = landmarks.landmark(j);
                        data.push_back(landmark.x());
                        data.push_back(landmark.y());
                        data.push_back(landmark.z());
                    }
                }
                godot_data["data"] = data;
                packet_data[stream_name] = godot_data;
            }
            else {
                auto& landmarks = packet.Get<::mediapipe::NormalizedLandmarkList>();
                Dictionary godot_data;
                godot_data["is_vector"] = is_vector;
                PoolRealArray data;
                data.push_back(landmarks.landmark_size());
                for(int i=0; i<landmarks.landmark_size(); i++){
                    auto& landmark = landmarks.landmark(i);
                    data.push_back(landmark.x());
                    data.push_back(landmark.y());
                    data.push_back(landmark.z());
                }
                godot_data["data"] = data;
                packet_data[stream_name] = godot_data;
            }
            mutex.unlock();
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
            absl::Status output_video_status = 
                graph->ObserveOutputStream(kOutputStream, [this, &gpu_helper](mediapipe::Packet packet)->absl::Status{
                    std::unique_ptr<mediapipe::ImageFrame> output_frame;
                        MP_RETURN_IF_ERROR(gpu_helper.RunInGlContext(
                        [this, &gpu_helper, &packet, &output_frame]() -> ::absl::Status {
                        auto& gpu_frame = packet.Get<mediapipe::GpuBuffer>();
                        auto texture = gpu_helper.CreateSourceTexture(gpu_frame);
                        output_frame = absl::make_unique<mediapipe::ImageFrame>(
                            mediapipe::ImageFormatForGpuBufferFormat(gpu_frame.format()),
                            gpu_frame.width(), gpu_frame.height(),
                            mediapipe::ImageFrame::kGlDefaultAlignmentBoundary);
                        gpu_helper.BindFramebuffer(texture);
                        const auto info =
                            mediapipe::GlTextureInfoForGpuBufferFormat(gpu_frame.format(), 0);
                        glReadPixels(0, 0, texture.width(), texture.height(), info.gl_format,
                                    info.gl_type, output_frame->MutablePixelData());
                        glFlush();
                        texture.Release();
                        return absl::OkStatus();
                    }));
                    // Convert back to opencv for display or saving.
                    cv::Mat output_frame_mat = mediapipe::formats::MatView(output_frame.get());
                    if (output_frame_mat.channels() == 4)
                        cv::cvtColor(output_frame_mat, output_video, cv::COLOR_RGBA2BGR);
                    else
                        cv::cvtColor(output_frame_mat, output_video, cv::COLOR_RGB2BGR);
                    return absl::OkStatus();
                    });
            if(!output_video_status.ok()) {
                Godot::print(output_video_status.message().data());
            }
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
