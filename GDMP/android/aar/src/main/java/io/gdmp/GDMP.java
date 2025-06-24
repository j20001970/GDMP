package io.gdmp;

import android.Manifest.permission;
import android.content.pm.PackageManager;

import androidx.annotation.NonNull;
import androidx.collection.ArraySet;

import com.google.mediapipe.framework.AndroidAssetUtil;

import org.godotengine.godot.Godot;
import org.godotengine.godot.plugin.GodotPlugin;
import org.godotengine.godot.plugin.SignalInfo;

import java.util.Set;

public class GDMP extends GodotPlugin {
    private static GDMP singleton;

    static {
        System.loadLibrary("GDMP.android");
    }

    private final Godot godot;

    public GDMP(Godot godot) {
        super(godot);
        AndroidAssetUtil.initializeNativeAssetManager(godot.getActivity().getApplicationContext());
        this.godot = godot;
        singleton = this;
    }

    public static GDMP getSingleton() {
        return singleton;
    }

    @Override
    public Godot getGodot() {
        return godot;
    }

    @Override
    public void onMainRequestPermissionsResult(int requestCode, String[] permissions, int[] grantResults) {
        for (int i = 0; i < permissions.length; i++) {
            switch (permissions[i]) {
                case permission.CAMERA:
                    if (grantResults[i] == PackageManager.PERMISSION_GRANTED) {
                        emitSignal("camera_permission_granted");
                    } else {
                        emitSignal("camera_permission_denied");
                    }
                    break;
            }
        }
    }

    @NonNull
    @Override
    public String getPluginName() {
        return "GDMP";
    }

    @NonNull
    @Override
    public Set<SignalInfo> getPluginSignals() {
        Set<SignalInfo> signals = new ArraySet<>();
        signals.add(new SignalInfo("camera_permission_granted"));
        signals.add(new SignalInfo("camera_permission_denied"));
        return signals;
    }

    @NonNull
    @Override
    public Set<String> getPluginGDExtensionLibrariesPaths() {
        ArraySet paths = new ArraySet<String>();
        paths.add("addons/GDMP/GDMP.gdextension");
        return paths;
    }
}
