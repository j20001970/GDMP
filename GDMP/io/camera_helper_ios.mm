#include "camera_helper.h"

#import <UIKit/UIKit.h>
#import <AVFoundation/AVFoundation.h>

#include "mediapipe/gpu/gpu_buffer.h"

#include "GDMP/framework/image.h"

@class OutputDelegate;
@interface OutputDelegate : NSObject <AVCaptureVideoDataOutputSampleBufferDelegate>
@property(nonatomic) MediaPipeCameraHelper *camera_helper;
@end

@implementation OutputDelegate
- (instancetype)init:(MediaPipeCameraHelper *)camera_helper {
    self = [super init];
    self.camera_helper = camera_helper;
  return self;
}
- (void)captureOutput:(AVCaptureOutput*)captureOutput
    didOutputSampleBuffer:(CMSampleBufferRef)sampleBuffer
           fromConnection:(AVCaptureConnection*)connection {
    CVPixelBufferRef imageBuffer = CMSampleBufferGetImageBuffer(sampleBuffer);
    Ref<MediaPipeImage> image = MediaPipeImage::_new(imageBuffer);
    self.camera_helper->emit_signal("new_frame", image);
}
@end

class CameraHelperIOS : public CameraHelperImpl {
    private:
        AVCaptureDeviceInput* videoDeviceInput;
        AVCaptureSession* session;
        AVCaptureVideoDataOutput* videoDataOutput;
        OutputDelegate *delegate;
        dispatch_queue_t delegateQueue;

    public:
        CameraHelperIOS(MediaPipeCameraHelper *camera_helper) : CameraHelperImpl(camera_helper) {
            dispatch_queue_attr_t qosAttribute = dispatch_queue_attr_make_with_qos_class(
                DISPATCH_QUEUE_SERIAL, QOS_CLASS_USER_INTERACTIVE, /*relative_priority=*/0);
            delegateQueue = dispatch_queue_create(NULL, qosAttribute);
            delegate = [[OutputDelegate alloc] init:camera_helper];
        }

        ~CameraHelperIOS() = default;

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

        void start(int index, Vector2 size) {
            ERR_FAIL_COND(delegate == nil);
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

void MediaPipeCameraHelper::_init() {
    impl = std::make_unique<CameraHelperIOS>(this);
}
