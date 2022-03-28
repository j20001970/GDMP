#include "camera_helper.h"

#include "Engine.hpp"
#include "OS.hpp"

#include "mediapipe/framework/packet.h"
#include "mediapipe/gpu/gl_texture_buffer.h"
#include "mediapipe/gpu/gpu_buffer.h"
#include "mediapipe/gpu/gpu_buffer_format.h"

#include "../framework/packet.h"

using namespace godot;

class CameraHelper::CameraHelperImpl {
	public:
		bool started;
		Object *android_plugin;

		CameraHelperImpl() {
			JNIEnv *env = android_api->godot_android_get_env();
			jobject activity = android_api->godot_android_get_activity();
			if (env->IsSameObject(camera_class, NULL)) {
				camera_class = reinterpret_cast<jclass>(
						env->NewGlobalRef(env->FindClass("org/godotengine/gdmp/GDMPCameraHelper")));
			}
			if (env->IsSameObject(camera_facing_class, NULL)) {
				camera_facing_class = reinterpret_cast<jclass>(
						env->NewGlobalRef(env->FindClass("com/google/mediapipe/components/CameraHelper$CameraFacing")));
			}
			if (env->IsSameObject(size_class, NULL)) {
				size_class = reinterpret_cast<jclass>(
						env->NewGlobalRef(env->FindClass("android/util/Size")));
			}
			android_plugin = Engine::get_singleton()->get_singleton("GDMP");
		}

		~CameraHelperImpl() {
		}

		void set_graph(Graph *graph, String stream_name) {
			this->graph = graph;
			this->stream_name = stream_name;
		}

		void start(int index) {
			camera_facing = index;
			start();
		}

		void start() {
			if (graph == nullptr) {
				Godot::print("Graph is not set");
				return;
			}
			started = true;
			JNIEnv *env = android_api->godot_android_get_env();
			jobject activity = android_api->godot_android_get_activity();
			jclass permission_class = env->FindClass("com/google/mediapipe/components/PermissionHelper");
			jmethodID check_method = env->GetStaticMethodID(
					permission_class, "cameraPermissionsGranted", "(Landroid/app/Activity;)Z");
			jboolean permission_granted = env->CallStaticBooleanMethod(permission_class, check_method, activity);
			if (permission_granted) {
				const char *camera_facing_field_sig =
						"Lcom/google/mediapipe/components/CameraHelper$CameraFacing;";
				jfieldID camera_facing_field;
				switch (camera_facing) {
					case 0:
						camera_facing_field = env->GetStaticFieldID(camera_facing_class, "FRONT", camera_facing_field_sig);
						break;
					case 1:
						camera_facing_field = env->GetStaticFieldID(camera_facing_class, "BACK", camera_facing_field_sig);
						break;
					default:
						Godot::print(String("unexpected camera facing {0}").format(Array::make(camera_facing)));
						return;
				}
				const char *camera_cnstr_sig =
						"(JLandroid/app/Activity;Lcom/google/mediapipe/glutil/EglManager;)V";
				jmethodID camera_cnstr = env->GetMethodID(camera_class, "<init>", camera_cnstr_sig);
				camera = env->NewObject(camera_class, camera_cnstr, this, activity, graph->egl_manager);
				camera = env->NewGlobalRef(camera);
				jobject camera_facing = env->GetStaticObjectField(camera_facing_class, camera_facing_field);
				jobject size = env->NewObject(size_class, env->GetMethodID(size_class, "<init>", "(II)V"), 640, 480);
				const char *start_camera_sig =
						"(Lcom/google/mediapipe/components/CameraHelper$CameraFacing;Landroid/util/Size;)V";
				env->CallVoidMethod(camera, env->GetMethodID(camera_class, "startCamera", start_camera_sig), camera_facing, size);
				env->DeleteLocalRef(camera_facing);
				env->DeleteLocalRef(size);
			} else {
				jmethodID request_method = env->GetStaticMethodID(
						permission_class, "checkAndRequestCameraPermissions", "(Landroid/app/Activity;)V");
				env->CallStaticVoidMethod(permission_class, request_method, activity);
			}
		}

		void close() {
			JNIEnv *env = android_api->godot_android_get_env();
			if (camera && !env->IsSameObject(camera, NULL)) {
				env->CallVoidMethod(camera, env->GetMethodID(camera_class, "closeCamera", "()V"));
			}
			started = false;
		}

		void on_new_frame(JNIEnv *env, jobject frame, int name, int width, int height) {
			auto gl_context = graph->get_gpu_resources()->gl_context();
			mediapipe::GlTextureBuffer::DeletionCallback callback;
			if (frame) {
				jobject java_frame = env->NewGlobalRef(frame);
				jmethodID release_method = env->GetMethodID(
						camera_class, "releaseFrame", "(Lcom/google/mediapipe/framework/TextureReleaseCallback;J)V");
				callback = [this, java_frame, release_method](mediapipe::GlSyncToken release_token) {
					JNIEnv *env = android_api->godot_android_get_env();
					jlong token = reinterpret_cast<jlong>(new mediapipe::GlSyncToken(std::move(release_token)));
					env->CallVoidMethod(camera, release_method, java_frame, token);
					env->DeleteGlobalRef(java_frame);
				};
			}
			mediapipe::GpuBuffer gpu_frame = mediapipe::GpuBuffer(mediapipe::GlTextureBuffer::Wrap(
					GL_TEXTURE_2D, name, width, height, mediapipe::GpuBufferFormat::kBGRA32, gl_context, callback));
			Ref<Packet> packet = Packet::_new(mediapipe::MakePacket<mediapipe::GpuBuffer>(gpu_frame));
			size_t timestamp = OS::get_singleton()->get_ticks_usec();
			packet->set_timestamp(timestamp);
			graph->add_packet(stream_name, packet);
		}

	private:
		static jclass camera_class;
		static jclass camera_facing_class;
		static jclass size_class;
		int camera_facing;
		jobject camera = nullptr;
		Graph *graph;
		String stream_name;
};

jclass CameraHelper::CameraHelperImpl::camera_class = nullptr;
jclass CameraHelper::CameraHelperImpl::camera_facing_class = nullptr;
jclass CameraHelper::CameraHelperImpl::size_class = nullptr;

extern "C" {
JNIEXPORT void JNICALL Java_org_godotengine_gdmp_GDMPCameraHelper_nativeOnNewFrame(JNIEnv *pEnv, jobject jCaller, jlong cppCaller, jobject frame, jint name, jint width, jint height) {
	auto caller = (CameraHelper::CameraHelperImpl *)(cppCaller);
	caller->on_new_frame(pEnv, frame, name, width, height);
}
}

CameraHelper::CameraHelper() = default;

CameraHelper::~CameraHelper() = default;

void CameraHelper::_init() {
	impl = std::make_unique<CameraHelperImpl>();
	if (impl->android_plugin) {
		impl->android_plugin->connect("permission_result", this, "_on_permission_result");
	}
}

void CameraHelper::_on_permission_result(PoolStringArray permissions, PoolIntArray results) {
	for (int i = 0; i < permissions.size(); i++) {
		String permission = permissions[i];
		if (permission == "android.permission.CAMERA" && results[i] == 0 && impl->started) {
			impl->start();
		}
	}
}

void CameraHelper::set_graph(Graph *graph, String stream_name) {
	impl->set_graph(graph, stream_name);
}

void CameraHelper::start(int index) {
	impl->start(index);
}

void CameraHelper::close() {
	impl->close();
}

#if !MEDIAPIPE_DISABLE_GPU
void CameraHelper::set_use_gpu(bool use_gpu) {
}
#endif
