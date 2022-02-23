package org.godotengine.gdmp;

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
        System.loadLibrary("mediapipe_jni");
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
        signals.add(new SignalInfo("permission_result", String[].class, int[].class));
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
        emitSignal("permission_result", permissions, grantResults);
    }
}
