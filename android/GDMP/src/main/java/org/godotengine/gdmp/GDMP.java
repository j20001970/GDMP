package org.godotengine.gdmp;

import android.Manifest.permission;
import android.content.pm.PackageManager;

import androidx.annotation.NonNull;
import androidx.collection.ArraySet;

import org.godotengine.godot.Godot;
import org.godotengine.godot.plugin.GodotPlugin;
import org.godotengine.godot.plugin.SignalInfo;

import java.util.Set;

public class GDMP extends GodotPlugin {
    private static String TAG = "GDMP";
    // Godot activity
    private Godot godot;

    static {
        System.loadLibrary("gdmp");
    }

    public GDMP(Godot godot) {
        super(godot);
        this.godot = godot;
    }

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

    @Override
    public void onMainResume() {
        super.onMainResume();
    }

    @Override
    public void onMainPause() {
        super.onMainPause();
    }

    @Override
    public void onMainRequestPermissionsResult(int requestCode, String[] permissions, int[] grantResults) {
        super.onMainRequestPermissionsResult(requestCode, permissions, grantResults);
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
}
