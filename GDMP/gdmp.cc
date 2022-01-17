#include <memory>

#include <Godot.hpp>
#include <OS.hpp>

#include "mediapipe/framework/calculator_framework.h"
#include "mediapipe/framework/formats/image_frame.h"
#include "mediapipe/framework/formats/image_frame_opencv.h"
#include "mediapipe/framework/port/file_helpers.h"
#include "mediapipe/framework/port/opencv_core_inc.h"
#include "mediapipe/framework/port/opencv_imgproc_inc.h"
#include "mediapipe/framework/port/opencv_video_inc.h"
#include "mediapipe/framework/port/parse_text_proto.h"
#include "mediapipe/framework/port/status.h"
#include "mediapipe/framework/port/status_macros.h"
#include "mediapipe/gpu/gl_calculator_helper.h"
#include "mediapipe/gpu/gpu_buffer.h"
#include "mediapipe/gpu/gpu_shared_data_internal.h"

#include "gdmp.h"

using namespace godot;

void GDMP::_register_methods() {
    register_method("initGraph", &GDMP::init_graph);
    register_method("addProtoCallback", &GDMP::add_proto_callback);
    register_method("addProtoVectorCallback", &GDMP::add_proto_vector_callback);
    register_method("addGpuFrameCallback", &GDMP::add_gpu_frame_callback);
    register_method("startCamera", &GDMP::start_camera);
    register_method("closeCamera", &GDMP::close_camera);
    register_method("loadVideo", &GDMP::load_video);
    register_signal<GDMP>("on_new_proto", "stream_name", GODOT_VARIANT_TYPE_STRING, "proto_bytes", GODOT_VARIANT_TYPE_POOL_BYTE_ARRAY);
    register_signal<GDMP>("on_new_proto_vector", "stream_name", GODOT_VARIANT_TYPE_STRING, "proto_vector", GODOT_VARIANT_TYPE_ARRAY);
    register_signal<GDMP>("on_new_frame", "stream_name", GODOT_VARIANT_TYPE_STRING, "width", GODOT_VARIANT_TYPE_INT, "height", GODOT_VARIANT_TYPE_INT, "data", GODOT_VARIANT_TYPE_POOL_BYTE_ARRAY);
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
        gpu_helper = std::make_unique<mediapipe::GlCalculatorHelper>();
        gpu_helper->InitializeForTest(graph->GetGpuResources().get());
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

void GDMP::add_gpu_frame_callback(String stream_name) {
    absl::Status result = [this, &stream_name]()->absl::Status {
        return graph->ObserveOutputStream(stream_name.alloc_c_string(), [this, stream_name](mediapipe::Packet packet)->absl::Status {
            int width;
            int height;
            PoolByteArray data;
            MP_RETURN_IF_ERROR(gpu_helper->RunInGlContext(
                [this, &packet, &width, &height, &data]() -> ::absl::Status {
                auto& gpu_frame = packet.Get<mediapipe::GpuBuffer>();
                auto texture = gpu_helper->CreateSourceTexture(gpu_frame);
                gpu_helper->BindFramebuffer(texture);
                const auto info =
                    mediapipe::GlTextureInfoForGpuBufferFormat(gpu_frame.format(), 0, gpu_helper->GetGlVersion());
                width = texture.width();
                height = texture.height();
                data.resize(width*height*4);
                glReadPixels(0, 0, texture.width(), texture.height(), info.gl_format,
                            info.gl_type, data.write().ptr());
                glFlush();
                texture.Release();
                return absl::OkStatus();
            }));
            call_deferred("emit_signal", Array::make("on_new_frame", stream_name, width, height, data));
            return absl::OkStatus();
        });
    }();
    if(!result.ok()) {
        Godot::print(result.message().data());
    }
}

void GDMP::start_camera(int index, String stream_name) {
    if(!capture.isOpened()){
        Godot::print("Initialize the camera or load the video.");
        capture.open(index);
        #if (CV_MAJOR_VERSION >= 3) && (CV_MINOR_VERSION >= 2)
        capture.set(cv::CAP_PROP_FRAME_WIDTH, 640);
        capture.set(cv::CAP_PROP_FRAME_HEIGHT, 480);
        capture.set(cv::CAP_PROP_FPS, 30);
        #endif
        absl::Status result = [this]()->absl::Status {
            MP_RETURN_IF_ERROR(graph->StartRun({}));
            return absl::OkStatus();
        }();
        if(capture.isOpened()){
            grab_frames = true;
            camera_thread = std::thread([this, &stream_name]()->void {
                while(grab_frames) {
                    cv::Mat video_frame;
                    capture >> video_frame;
                    cv::flip(video_frame, video_frame, /*flipcode=HORIZONTAL*/ 1);
                    cv::cvtColor(video_frame, video_frame, cv::COLOR_BGR2RGBA);
                    absl::Status result = send_video_frame(video_frame, stream_name.alloc_c_string());
                    if(!result.ok()) {
                        Godot::print(result.message().data());
                    }
                }
                Godot::print("Shutting down.");
                absl::Status result = [this]()->absl::Status {
                    MP_RETURN_IF_ERROR(graph->CloseAllInputStreams());
                    return graph->WaitUntilDone();
                }();
            });
        }
        else{
            Godot::print("fail to open camera");
        }
    }
}

void GDMP::close_camera() {
    grab_frames = false;
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
        absl::Status result = [this]()->absl::Status {
            MP_RETURN_IF_ERROR(graph->StartRun({}));
            return absl::OkStatus();
        }();
        if(capture.isOpened()){
            grab_frames = true;
            camera_thread = std::thread([this](){
                float delay = 1000 / capture.get(cv::CAP_PROP_FPS);
                while(grab_frames) {
                    cv::Mat video_frame;
                    capture >> video_frame;
                    cv::cvtColor(video_frame, video_frame, cv::COLOR_BGR2RGBA);
                    absl::Status result = send_video_frame(video_frame, "input_video");
                    if(!result.ok()) {
                        Godot::print(result.message().data());
                    }
                    OS::get_singleton()->delay_msec(delay);
                }
            });
        }
        else{
            Godot::print("fail to open camera");
        }
    }
}

absl::Status GDMP::send_video_frame(cv::Mat video_frame, String stream_name) {
    auto input_frame = absl::make_unique<mediapipe::ImageFrame>(
        mediapipe::ImageFormat::SRGBA, video_frame.cols, video_frame.rows,
        mediapipe::ImageFrame::kGlDefaultAlignmentBoundary);
    cv::Mat input_frame_mat = mediapipe::formats::MatView(input_frame.get());
    video_frame.copyTo(input_frame_mat);
    // Prepare and add graph input packet.
    size_t frame_timestamp_us = OS::get_singleton()->get_ticks_usec();
    MP_RETURN_IF_ERROR(
        gpu_helper->RunInGlContext([this, &input_frame, &frame_timestamp_us, &stream_name]() -> absl::Status {
        // Convert ImageFrame to GpuBuffer.
        auto texture = gpu_helper->CreateSourceTexture(*input_frame.get());
        auto gpu_frame = texture.GetFrame<mediapipe::GpuBuffer>();
        glFlush();
        texture.Release();
        // Send GPU image packet into the graph.
        MP_RETURN_IF_ERROR(graph->AddPacketToInputStream(
            stream_name.alloc_c_string(), mediapipe::Adopt(gpu_frame.release())
                                .At(mediapipe::Timestamp(frame_timestamp_us))));
        return absl::OkStatus();
        }));
    return absl::OkStatus();
}
