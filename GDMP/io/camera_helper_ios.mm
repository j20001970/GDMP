#include "camera_helper.h"

#import <UIKit/UIKit.h>
#import <AVFoundation/AVFoundation.h>

#include "godot_cpp/classes/time.hpp"

#include "mediapipe/gpu/gpu_buffer.h"

#include "GDMP/framework/packet.h"

@class OutputDelegate;
@interface OutputDelegate : NSObject <AVCaptureVideoDataOutputSampleBufferDelegate>
@property(nonatomic) Ref<MediaPipeGraph> graph;
@property(nonatomic) String stream_name;
@end

@implementation OutputDelegate
- (instancetype)init:(Ref<MediaPipeGraph>)graph
    StreamName:(String)stream_name{
    self = [super init];
    self.graph = graph;
    self.stream_name = stream_name;
  return self;
}
- (void)captureOutput:(AVCaptureOutput*)captureOutput
    didOutputSampleBuffer:(CMSampleBufferRef)sampleBuffer
           fromConnection:(AVCaptureConnection*)connection {
    CVPixelBufferRef imageBuffer = CMSampleBufferGetImageBuffer(sampleBuffer);
    Ref<MediaPipePacket> packet = memnew(MediaPipePacket(mediapipe::MakePacket<mediapipe::GpuBuffer>(imageBuffer)));
    packet->set_timestamp(Time::get_singleton()->get_ticks_usec());
    self.graph->add_packet(self.stream_name, packet);
}
@end

class MediaPipeCameraHelper::Impl {
    private:
        AVCaptureSession* session;
        AVCaptureDeviceInput* videoDeviceInput;
        AVCaptureVideoDataOutput* videoDataOutput;
        dispatch_queue_t delegateQueue;
        OutputDelegate *delegate;

    public:
        Impl() {
            dispatch_queue_attr_t qosAttribute = dispatch_queue_attr_make_with_qos_class(
                DISPATCH_QUEUE_SERIAL, QOS_CLASS_USER_INTERACTIVE, /*relative_priority=*/0);
            delegateQueue = dispatch_queue_create("org.godotengine.gdmp.delegateQueue", qosAttribute);
        }

        ~Impl() {
        }

        bool permission_granted() {
            AVAuthorizationStatus status = [AVCaptureDevice authorizationStatusForMediaType:AVMediaTypeVideo];
            return status == AVAuthorizationStatusAuthorized;
        }

        void request_permission(MediaPipeCameraHelper *camera_helper) {
            [AVCaptureDevice requestAccessForMediaType:AVMediaTypeVideo
                                     completionHandler:^(BOOL granted) {
                camera_helper->emit_signal("permission_result", granted);
            }];
        }

        void set_graph(Ref<MediaPipeGraph> graph, String stream_name) {
            delegate = [[OutputDelegate alloc] init:graph
            StreamName:stream_name];
        }

        void start(int index, Vector2 size) {
            ERR_FAIL_COND(delegate == nil);
            ERR_FAIL_COND(!permission_granted());
            close();
            session = [[AVCaptureSession alloc] init];
            AVCaptureDevicePosition position;
            if(index == 0) {
                position = AVCaptureDevicePositionFront;
            }
            else {
                position = AVCaptureDevicePositionBack;
            }
            AVCaptureDeviceType deviceType = AVCaptureDeviceTypeBuiltInWideAngleCamera;
            AVCaptureDeviceDiscoverySession* deviceDiscoverySession = [AVCaptureDeviceDiscoverySession
                discoverySessionWithDeviceTypes:@[deviceType]
                                      mediaType:AVMediaTypeVideo
                                       position:position];
            AVCaptureDevice* videoDevice =
                [deviceDiscoverySession devices]
                    ? deviceDiscoverySession.devices.firstObject
                    : [AVCaptureDevice defaultDeviceWithMediaType:AVMediaTypeVideo];
            NSError* error = nil;
            videoDeviceInput = [AVCaptureDeviceInput deviceInputWithDevice:videoDevice error:&error];
            [session addInput:videoDeviceInput];
            videoDataOutput = [[AVCaptureVideoDataOutput alloc] init];
            videoDataOutput.alwaysDiscardsLateVideoFrames = YES;
            videoDataOutput.videoSettings = @{(id)kCVPixelBufferPixelFormatTypeKey : @(kCVPixelFormatType_32BGRA)};
            [videoDataOutput setSampleBufferDelegate:delegate queue:delegateQueue];
            [session addOutput:videoDataOutput];
            session.sessionPreset = AVCaptureSessionPreset640x480;
            AVCaptureConnection* connection = [videoDataOutput connectionWithMediaType:AVMediaTypeVideo];
            connection.videoOrientation = (AVCaptureVideoOrientation)UIDevice.currentDevice.orientation;
            if (position == AVCaptureDevicePositionFront) {
                connection.videoMirrored = YES;
            }
            [session startRunning];
        }
    

        void close() {
            if(session != nil) {
                [session stopRunning];
                session = nil;
            }
        }
};

MediaPipeCameraHelper::MediaPipeCameraHelper() {
    impl = std::make_unique<Impl>();
}

MediaPipeCameraHelper::~MediaPipeCameraHelper() = default;

bool MediaPipeCameraHelper::permission_granted() {
    return impl->permission_granted();
}

void MediaPipeCameraHelper::request_permission() {
    impl->request_permission(this);
}

void MediaPipeCameraHelper::set_graph(Ref<MediaPipeGraph> graph, String stream_name) {
    impl->set_graph(graph, stream_name);
}

void MediaPipeCameraHelper::set_mirrored(bool value) {
}

void MediaPipeCameraHelper::start(int index, Vector2 size) {
    impl->start(index, size);
}

void MediaPipeCameraHelper::close() {
    impl->close();
}

#if !MEDIAPIPE_DISABLE_GPU
void MediaPipeCameraHelper::set_use_gpu(bool use_gpu) {
}
#endif
