#include "camera_helper.h"

#import <UIKit/UIKit.h>
#import <AVFoundation/AVFoundation.h>

#include "OS.hpp"

#include "mediapipe/gpu/gpu_buffer.h"

#include "mediapipe/GDMP/framework/packet.h"

using namespace godot;

@class OutputDelegate;
@interface OutputDelegate : NSObject <AVCaptureVideoDataOutputSampleBufferDelegate>
@property(nonatomic) Ref<Graph> graph;
@property(nonatomic) String stream_name;
@end

@implementation OutputDelegate
- (instancetype)init:(Ref<Graph>)graph
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
    Ref<Packet> packet = Packet::_new(mediapipe::MakePacket<mediapipe::GpuBuffer>(imageBuffer));
    packet->set_timestamp(OS::get_singleton()->get_ticks_usec());
    self.graph->add_packet(self.stream_name, packet);
}
@end

class CameraHelper::CameraHelperImpl {
    public:
        CameraHelperImpl() {
            dispatch_queue_attr_t qosAttribute = dispatch_queue_attr_make_with_qos_class(
                DISPATCH_QUEUE_SERIAL, QOS_CLASS_USER_INTERACTIVE, /*relative_priority=*/0);
            delegateQueue = dispatch_queue_create("org.godotengine.gdmp.delegateQueue", qosAttribute);
        }

        ~CameraHelperImpl() {
        }

        bool permission_granted() {
            AVAuthorizationStatus status = [AVCaptureDevice authorizationStatusForMediaType:AVMediaTypeVideo];
            return status == AVAuthorizationStatusAuthorized;
        }

        void request_permission(CameraHelper *camera_helper) {
            [AVCaptureDevice requestAccessForMediaType:AVMediaTypeVideo
                                     completionHandler:^(BOOL granted) {
                camera_helper->emit_signal("permission_result", granted);
            }];
        }

        void set_graph(Ref<Graph> graph, String stream_name) {
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

    private:
        AVCaptureSession* session;
        AVCaptureDeviceInput* videoDeviceInput;
        AVCaptureVideoDataOutput* videoDataOutput;
        dispatch_queue_t delegateQueue;
        OutputDelegate *delegate;
};

CameraHelper::CameraHelper() = default;

CameraHelper::~CameraHelper() = default;

void CameraHelper::_init() {
    impl = std::make_unique<CameraHelperImpl>();
}

bool CameraHelper::permission_granted() {
    return impl->permission_granted();
}

void CameraHelper::request_permission() {
    impl->request_permission(this);
}

void CameraHelper::set_graph(Ref<Graph> graph, String stream_name) {
    impl->set_graph(graph, stream_name);
}

void CameraHelper::set_mirrored(bool value) {
}

void CameraHelper::start(int index, Vector2 size) {
    impl->start(index, size);
}

void CameraHelper::close() {
    impl->close();
}

#if !MEDIAPIPE_DISABLE_GPU
void CameraHelper::set_use_gpu(bool use_gpu) {
}
#endif
