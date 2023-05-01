package org.godotengine.gdmp;

import android.app.Activity;
import android.graphics.SurfaceTexture;
import android.opengl.EGL14;
import android.opengl.GLES20;
import android.os.Handler;
import android.os.HandlerThread;
import android.os.Process;
import android.util.Log;
import android.util.Size;
import android.view.Surface;

import androidx.camera.core.CameraSelector;
import androidx.camera.core.Preview;
import androidx.camera.lifecycle.ProcessCameraProvider;
import androidx.core.content.ContextCompat;
import androidx.lifecycle.LifecycleOwner;

import com.google.common.util.concurrent.ListenableFuture;
import com.google.mediapipe.components.ExternalTextureConverter;
import com.google.mediapipe.components.TextureFrameConsumer;
import com.google.mediapipe.framework.TextureFrame;
import com.google.mediapipe.glutil.EglManager;

import java.util.concurrent.Executor;
import java.util.concurrent.RejectedExecutionException;

import javax.microedition.khronos.egl.EGLSurface;

public class GDMPCameraHelper implements TextureFrameConsumer {
    private static final String TAG = "GDMPCameraHelper";

    private final long nativeCallerPtr;
    private final Activity activity;
    private final EglManager eglManager;
    private final ExternalTextureConverter converter;
    private final SingleThreadHandlerExecutor renderExecutor =
            new SingleThreadHandlerExecutor("RenderThread", Process.THREAD_PRIORITY_DEFAULT);
    private ProcessCameraProvider cameraProvider;
    private Preview preview;

    public GDMPCameraHelper(long nativeCallerPtr) {
        this.nativeCallerPtr = nativeCallerPtr;
        activity = GDMP.getSingleton().getGodot().getActivity();
        // Get graph GL context by getting current context.
        eglManager = new EglManager(EGL14.eglGetCurrentContext());
        converter = new ExternalTextureConverter(eglManager.getContext());
        converter.setConsumer(this);
        converter.setFlipY(true);
        converter.setRotation(activity.getWindowManager().getDefaultDisplay().getRotation());
    }

    public void startCamera(int index, int width, int height) {
        CameraSelector cameraSelector =
                index == 0
                        ? CameraSelector.DEFAULT_FRONT_CAMERA
                        : CameraSelector.DEFAULT_BACK_CAMERA;
        Size targetSize = new Size(width, height);
        Executor mainThreadExecutor = ContextCompat.getMainExecutor(activity);
        ListenableFuture<ProcessCameraProvider> cameraProviderFuture = ProcessCameraProvider.getInstance(activity);
        cameraProviderFuture.addListener(() -> {
            try {
                cameraProvider = cameraProviderFuture.get();
            } catch (Exception e) {
                if (e instanceof InterruptedException) {
                    Thread.currentThread().interrupt();
                }
                Log.e(TAG, e.getMessage());
                return;
            }
            preview = new Preview.Builder().setTargetResolution(targetSize).build();
            preview.setSurfaceProvider(
                    renderExecutor,
                    request -> {
                        Size frameSize = request.getResolution();
                        int[] textures = new int[1];
                        EGLSurface eglSurface = eglManager.createOffscreenSurface(1, 1);
                        eglManager.makeCurrent(eglSurface, eglSurface);
                        GLES20.glGenTextures(1, textures, 0);
                        SurfaceTexture previewFrameTexture = new SurfaceTexture(textures[0]);
                        previewFrameTexture.setDefaultBufferSize(frameSize.getWidth(), frameSize.getHeight());
                        request.setTransformationInfoListener(
                                renderExecutor,
                                transformationInfo -> {
                                    int frameRotation = transformationInfo.getRotationDegrees();
                                    boolean cameraRotated = frameRotation % 180 == 90;
                                    converter.setSurfaceTexture(
                                            previewFrameTexture,
                                            cameraRotated ? frameSize.getHeight() : frameSize.getWidth(),
                                            cameraRotated ? frameSize.getWidth() : frameSize.getHeight()
                                    );
                                });
                        Surface surface = new Surface(previewFrameTexture);
                        request.provideSurface(
                                surface,
                                renderExecutor,
                                result -> {
                                    surface.release();
                                });
                    }
            );
            cameraProvider.bindToLifecycle((LifecycleOwner) activity, cameraSelector, preview);
        }, mainThreadExecutor);
    }

    public void closeCamera() {
        if (cameraProvider != null && preview != null) {
            cameraProvider.unbind(preview);
        }
        if (converter != null) {
            converter.close();
        }
    }

    @Override
    public void onNewFrame(TextureFrame frame) {
        nativeOnNewFrame(nativeCallerPtr, frame, frame.getTextureName(), frame.getWidth(), frame.getHeight());
    }

    private native void nativeOnNewFrame(long nativeCallerPtr, TextureFrame frame, int name, int width, int height);

    private static final class SingleThreadHandlerExecutor implements Executor {
        private final HandlerThread handlerThread;
        private final Handler handler;

        SingleThreadHandlerExecutor(String threadName, int priority) {
            handlerThread = new HandlerThread(threadName, priority);
            handlerThread.start();
            handler = new Handler(handlerThread.getLooper());
        }

        @Override
        public void execute(Runnable command) {
            if (!handler.post(command)) {
                throw new RejectedExecutionException(handlerThread.getName() + " is shutting down.");
            }
        }

        boolean shutdown() {
            return handlerThread.quitSafely();
        }
    }
}
