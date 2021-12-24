package org.godotengine.gdmp;

import android.annotation.SuppressLint;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Matrix;
import android.graphics.SurfaceTexture;
import android.util.Log;
import android.util.Size;

import androidx.annotation.NonNull;
import androidx.camera.core.ImageAnalysis;
import androidx.camera.core.internal.utils.ImageUtil;
import androidx.collection.ArraySet;

import com.google.mediapipe.components.CameraHelper;
import com.google.mediapipe.components.ExternalTextureConverter;
import com.google.mediapipe.components.FrameProcessor;
import com.google.mediapipe.components.PermissionHelper;
import com.google.mediapipe.framework.AndroidAssetUtil;
import com.google.mediapipe.framework.AndroidPacketCreator;
import com.google.mediapipe.framework.Packet;
import com.google.mediapipe.framework.PacketGetter;
import com.google.mediapipe.glutil.EglManager;

import org.godotengine.godot.Dictionary;
import org.godotengine.godot.Godot;
import org.godotengine.godot.GodotLib;
import org.godotengine.godot.plugin.GodotPlugin;
import org.godotengine.godot.plugin.SignalInfo;
import org.godotengine.godot.plugin.UsedByGodot;

import java.nio.ByteBuffer;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;

public class GDMP extends GodotPlugin {
    // Godot activity
    public Godot godot;
    // The godot object instance id that get assigned from init() method
    private int instance_id;

    private static String TAG = "GDMP";
    // Whether camera has started
    private boolean cameraStarted = false;
    // Selected camera facing
    private CameraHelper.CameraFacing cameraFacing;
    // {@link SurfaceTexture} where the camera-preview frames can be accessed.
    private SurfaceTexture previewFrameTexture;
    // Creates and manages an {@link EGLContext}.
    private EglManager eglManager;
    // Sends camera-preview frames into a MediaPipe graph for processing
    private FrameProcessor processor;
    // Converts the GL_TEXTURE_EXTERNAL_OES texture from Android camera into a regular texture to be
    // consumed by {@link FrameProcessor} and the underlying MediaPipe graph.
    private ExternalTextureConverter converter;
    // Handles camera access via the {@link CameraX} Jetpack support library.
    private GDMPCameraHelper cameraHelper;

    static {
        // Load all native libraries needed by the app.
        System.loadLibrary("mediapipe_jni");
        System.loadLibrary("opencv_java3");
    }

    public GDMP(Godot godot) {
        super(godot);
        this.godot = godot;

        // Initialize asset manager so that MediaPipe native libraries can access the app assets, e.g.,
        // binary graphs.
        AndroidAssetUtil.initializeNativeAssetManager(this.godot.getActivity());
        eglManager = new EglManager(null);
    }

    @Override
    public String getPluginName() {
        return "GDMP";
    }

    @NonNull
    @Override
    public Set<SignalInfo> getPluginSignals() {
        Set<SignalInfo> signals = new ArraySet<>();
        signals.add(new SignalInfo("on_new_proto", String.class, byte[].class));
        signals.add(new SignalInfo("on_new_proto_vector", String.class, Object[].class));
        return signals;
    }

    @NonNull
    @Override
    protected Set<String> getPluginGDNativeLibrariesPaths() {
        return new HashSet<>(Arrays.asList("addons/GDMP/GDMP.gdnlib"));
    }

    @Override
    public void onMainResume() {
        super.onMainResume();
        if (cameraStarted && PermissionHelper.cameraPermissionsGranted(this.godot.getActivity())) {
            converter = new ExternalTextureConverter(eglManager.getContext());
            converter.setFlipY(true);
            converter.setConsumer(processor);
            startCamera();
        }
    }

    @Override
    public void onMainPause() {
        super.onMainPause();
        if (converter != null) {
            converter.close();
        }
    }

    @Override
    public void onMainRequestPermissionsResult(int requestCode, String[] permissions, int[] grantResults) {
        super.onMainRequestPermissionsResult(requestCode, permissions, grantResults);
    }

    @UsedByGodot
    public void init(final int id) {
        instance_id = id;
    }

    @UsedByGodot
    public void initGraph(String graph_path, Dictionary input_side_packet) {
        closeCamera();
        processor = new FrameProcessor(
                this.godot.getContext(),
                eglManager.getNativeContext(),
                graph_path,
                "input_video",
                "output_video"
        );
        AndroidPacketCreator packetCreator = processor.getPacketCreator();
        Map<String, Packet> sidePackets = new HashMap<>();
        for (String key : input_side_packet.get_keys()) {
            Object value = input_side_packet.get(key);
            Class<? extends Object> cls = value.getClass();
            if (cls == Integer.class) {
                sidePackets.put(key, packetCreator.createInt32((int) value));
            }
        }
        processor.setInputSidePackets(sidePackets);
        converter = new ExternalTextureConverter(eglManager.getContext());
        converter.setFlipY(true);
        converter.setConsumer(processor);
    }

    @UsedByGodot
    private void addProtoCallback(String streamName) {
        if (processor != null) {
            processor.addPacketCallback(
                    streamName,
                    packet -> {
                        byte[] protoBytes = PacketGetter.getProtoBytes(packet);
                        emitSignal("on_new_proto", new Object[]{streamName, protoBytes});
                    }
            );
        }
    }

    @UsedByGodot
    private void addProtoVectorCallback(String streamName) {
        if (processor != null) {
            processor.addPacketCallback(
                    streamName,
                    packet -> {
                        byte[][] protoVector = PacketGetter.getProtoBytesVector(packet);
                        List<byte[]> protoBytesList = new ArrayList<>();
                        for (byte[] protoBytes : protoVector) {
                            protoBytesList.add(protoBytes);
                        }
                        emitSignal("on_new_proto_vector", new Object[]{streamName, protoBytesList.toArray()});
                    }
            );
        }
    }

    @UsedByGodot
    private void startCamera(int cameraFacing) {
        if (cameraFacing > -1 && CameraHelper.CameraFacing.values().length > cameraFacing) {
            this.cameraFacing = CameraHelper.CameraFacing.values()[cameraFacing];
            startCamera();
        }
    }

    private void startCamera() {
        // Set cameraStarted to true unconditionally, so that onMainResume
        // can check if camera need to open.
        cameraStarted = true;
        // Actually start camera only when permission is granted.
        PermissionHelper.checkAndRequestCameraPermissions(godot.getActivity());
        if (PermissionHelper.cameraPermissionsGranted(godot.getActivity())) {
            cameraHelper = new GDMPCameraHelper();
            cameraHelper.setOnCameraStartedListener(
                    surfaceTexture -> {
                        previewFrameTexture = surfaceTexture;
                        Size displaySize = cameraHelper.getFrameSize();
                        boolean isCameraRotated = cameraHelper.isCameraRotated();
                        converter.setSurfaceTextureAndAttachToGLContext(
                                previewFrameTexture,
                                isCameraRotated ? displaySize.getHeight() : displaySize.getWidth(),
                                isCameraRotated ? displaySize.getWidth() : displaySize.getHeight());
                    }
            );
            runOnUiThread(
                    () -> {
                        cameraHelper.startCamera(godot.getActivity(), cameraFacing, previewFrameTexture, new Size(640, 480));
                    }
            );
        }
    }

    @UsedByGodot
    public void closeCamera() {
        cameraStarted = false;
        if (cameraHelper != null) {
            cameraHelper.closeCamera(godot.getActivity());
        }
        if (converter != null) {
            converter.close();
        }
    }
}
