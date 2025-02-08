package io.gdmp;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.graphics.SurfaceTexture;
import android.hardware.camera2.CameraAccessException;
import android.hardware.camera2.CameraCaptureSession;
import android.hardware.camera2.CameraCharacteristics;
import android.hardware.camera2.CameraDevice;
import android.hardware.camera2.CameraManager;
import android.hardware.camera2.CaptureRequest;
import android.hardware.camera2.params.StreamConfigurationMap;
import android.os.Handler;
import android.os.HandlerThread;
import android.os.Process;
import android.util.Log;
import android.util.Size;
import android.view.Surface;

import android.opengl.EGL14;
import androidx.annotation.NonNull;

import com.google.mediapipe.components.ExternalTextureConverter;
import com.google.mediapipe.components.TextureFrameConsumer;
import com.google.mediapipe.framework.TextureFrame;
import com.google.mediapipe.glutil.EglManager;

import java.util.Arrays;
import java.util.List;

public class GDMPCameraHelper implements TextureFrameConsumer {
    private static final String TAG = "GDMPCameraHelper";

    private final long nativeCallerPtr;
    private final Activity activity;
    private final EglManager eglManager;
    private final ExternalTextureConverter converter;
    private final HandlerThread cameraThread;
    private final Handler cameraHandler;
    private CameraDevice cameraDevice;
    private CameraCaptureSession captureSession;
    private SurfaceTexture previewTexture;

    public GDMPCameraHelper(long nativeCallerPtr) {
        this.nativeCallerPtr = nativeCallerPtr;
        activity = GDMP.getSingleton().getGodot().getActivity();
        eglManager = new EglManager(EGL14.eglGetCurrentContext());
        converter = new ExternalTextureConverter(eglManager.getContext());
        converter.setConsumer(this);
        converter.setFlipY(true);
        converter.setRotation(activity.getWindowManager().getDefaultDisplay().getRotation());
        cameraThread = new HandlerThread("CameraThread", Process.THREAD_PRIORITY_DEFAULT);
        cameraThread.start();
        cameraHandler = new Handler(cameraThread.getLooper());
    }

    @SuppressLint("MissingPermission")
    public void startCamera(int index, int width, int height) {
        CameraManager cameraManager = (CameraManager) activity.getSystemService(Activity.CAMERA_SERVICE);
        try {
            String[] cameraIds = cameraManager.getCameraIdList();
            Log.e(TAG, "Cameras found: " + Arrays.toString(cameraIds));
            String cameraId = null;

            if (index == 0) {
                // Select front camera
                for (String id : cameraIds) {
                    CameraCharacteristics characteristics = cameraManager.getCameraCharacteristics(id);
                    Integer facing = characteristics.get(CameraCharacteristics.LENS_FACING);
                    if (facing != null && facing == CameraCharacteristics.LENS_FACING_FRONT) {
                        cameraId = id;
                        break;
                    }
                }
            } else if (index == 1) {
                // Select back camera
                for (String id : cameraIds) {
                    CameraCharacteristics characteristics = cameraManager.getCameraCharacteristics(id);
                    Integer facing = characteristics.get(CameraCharacteristics.LENS_FACING);
                    if (facing != null && facing == CameraCharacteristics.LENS_FACING_BACK) {
                        cameraId = id;
                        break;
                    }
                }
            } else {
                // Select other available camera
                if (index < cameraIds.length) {
                    cameraId = cameraIds[index];
                } else {
                    cameraId = cameraIds[0]; // Default to the first camera if index is out of bounds
                }
            }

            CameraCharacteristics characteristics = cameraManager.getCameraCharacteristics(cameraId);
            StreamConfigurationMap map = characteristics.get(CameraCharacteristics.SCALER_STREAM_CONFIGURATION_MAP);
            Size[] sizes = map.getOutputSizes(SurfaceTexture.class);
            Size targetSize = new Size(width, height);
            for (Size size : sizes) {
                if (size.getWidth() == targetSize.getWidth() && size.getHeight() == targetSize.getHeight()) {
                    targetSize = size;
                    break;
                }
            }
            Size finalTargetSize = targetSize;
            cameraManager.openCamera(cameraId, new CameraDevice.StateCallback() {
                @Override
                public void onOpened(@NonNull CameraDevice camera) {
                    cameraDevice = camera;
                    createCaptureSession(finalTargetSize);
                }

                @Override
                public void onDisconnected(@NonNull CameraDevice camera) {
                    camera.close();
                }

                @Override
                public void onError(@NonNull CameraDevice camera, int error) {
                    Log.e(TAG, "Camera error: " + error);
                }
            }, cameraHandler);
        } catch (CameraAccessException e) {
            Log.e(TAG, "Camera access exception: " + e.getMessage());
        }
    }

    private void createCaptureSession(Size targetSize) {
        try {
            SurfaceTexture surfaceTexture = new SurfaceTexture(0);
            surfaceTexture.setDefaultBufferSize(targetSize.getWidth(), targetSize.getHeight());
            previewTexture = surfaceTexture;
            converter.setSurfaceTexture(surfaceTexture, targetSize.getWidth(), targetSize.getHeight());
            Surface surface = new Surface(surfaceTexture);
            cameraDevice.createCaptureSession(List.of(surface), new CameraCaptureSession.StateCallback() {
                @Override
                public void onConfigured(@NonNull CameraCaptureSession session) {
                    captureSession = session;
                    try {
                        CaptureRequest.Builder builder = cameraDevice.createCaptureRequest(CameraDevice.TEMPLATE_PREVIEW);
                        builder.addTarget(surface);
                        captureSession.setRepeatingRequest(builder.build(), null, cameraHandler);
                    } catch (CameraAccessException e) {
                        Log.e(TAG, "Camera access exception: " + e.getMessage());
                    }
                }

                @Override
                public void onConfigureFailed(@NonNull CameraCaptureSession session) {
                    Log.e(TAG, "Capture session configuration failed");
                }
            }, cameraHandler);
        } catch (CameraAccessException e) {
            Log.e(TAG, "Camera access exception: " + e.getMessage());
        }
    }

    public void closeCamera() {
        if (cameraDevice!= null) {
            cameraDevice.close();
        }
        if (captureSession!= null) {
            captureSession.close();
        }
        if (converter!= null) {
            converter.close();
        }
        cameraThread.quitSafely();
    }

    @Override
    public void onNewFrame(TextureFrame frame) {
        nativeOnNewFrame(nativeCallerPtr, frame, frame.getTextureName(), frame.getWidth(), frame.getHeight());
    }

    private native void nativeOnNewFrame(long nativeCallerPtr, TextureFrame frame, int name, int width, int height);
}