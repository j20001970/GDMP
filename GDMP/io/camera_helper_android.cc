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
			started = false;
			JNIEnv *env = android_api->godot_android_get_env();
			if (env->IsSameObject(camera_class, NULL)) {
				camera_class = reinterpret_cast<jclass>(
						env->NewGlobalRef(env->FindClass("org/godotengine/gdmp/GDMPCameraHelper")));
			}
			android_plugin = Engine::get_singleton()->get_singleton("GDMP");
		}

		~CameraHelperImpl() {
		}

		jobject create_camera() {
			jobject camera;
			auto context = graph->get_gpu_resources()->gl_context();
			// Create camera object in GL context so that it can get graph context on Java side.
			context->Run([this, &camera]() -> void {
				JNIEnv *env = android_api->godot_android_get_env();
				jobject activity = android_api->godot_android_get_activity();
				const char *sig = "(JLandroid/app/Activity;)V";
				jmethodID cnstr = env->GetMethodID(camera_class, "<init>", sig);
				camera = env->NewObject(camera_class, cnstr, this, activity);
				camera = env->NewGlobalRef(camera);
			});
			return camera;
		}

		jobject get_camera_facing(int index) {
			JNIEnv *env = android_api->godot_android_get_env();
			jclass cls = env->FindClass("com/google/mediapipe/components/CameraHelper$CameraFacing");
			const char *sig =
					"Lcom/google/mediapipe/components/CameraHelper$CameraFacing;";
			jfieldID fieldID;
			switch (index) {
				case 0:
					fieldID = env->GetStaticFieldID(cls, "FRONT", sig);
					break;
				case 1:
					fieldID = env->GetStaticFieldID(cls, "BACK", sig);
					break;
				default:
					Godot::print(String("unexpected camera facing {0}").format(Array::make(index)));
					break;
			}
			jobject camera_facing = env->GetStaticObjectField(cls, fieldID);
			env->DeleteLocalRef(cls);
			return camera_facing;
		}

		bool permission_granted() {
			JNIEnv *env = android_api->godot_android_get_env();
			jobject activity = android_api->godot_android_get_activity();
			jclass cls = env->FindClass("com/google/mediapipe/components/PermissionHelper");
			jmethodID method = env->GetStaticMethodID(
					cls, "cameraPermissionsGranted", "(Landroid/app/Activity;)Z");
			return env->CallStaticBooleanMethod(cls, method, activity);
		}

		void request_permission() {
			JNIEnv *env = android_api->godot_android_get_env();
			jobject activity = android_api->godot_android_get_activity();
			jclass cls = env->FindClass("com/google/mediapipe/components/PermissionHelper");
			jmethodID method = env->GetStaticMethodID(
					cls, "checkAndRequestCameraPermissions", "(Landroid/app/Activity;)V");
			env->CallStaticVoidMethod(cls, method, activity);
		}

		void set_graph(Graph *graph, String stream_name) {
			this->graph = graph;
			this->stream_name = stream_name;
		}

		void start(int index, Vector2 size) {
			camera_facing = index;
			camera_size = size;
			start();
		}

		void start() {
			if (graph == nullptr) {
				Godot::print("Graph is not set");
				return;
			}
			if (started) {
				return;
			}
			started = true;
			JNIEnv *env = android_api->godot_android_get_env();
			if (permission_granted()) {
				camera = create_camera();
				jobject camera_facing = get_camera_facing(this->camera_facing);
				const char *sig =
						"(Lcom/google/mediapipe/components/CameraHelper$CameraFacing;II)V";
				// CameraXPreviewHelper transpose target size unconditionally, therefore we transpose it back here.
				jint width = camera_size.y;
				jint height = camera_size.x;
				env->CallVoidMethod(
						camera, env->GetMethodID(camera_class, "startCamera", sig), camera_facing, width, height);
				env->DeleteLocalRef(camera_facing);
			} else {
				request_permission();
			}
		}

		void close() {
			JNIEnv *env = android_api->godot_android_get_env();
			if (camera && !env->IsSameObject(camera, NULL)) {
				env->CallVoidMethod(camera, env->GetMethodID(camera_class, "closeCamera", "()V"));
				env->DeleteGlobalRef(camera);
			}
			started = false;
		}

		void on_new_frame(JNIEnv *env, jobject frame, int name, int width, int height) {
			auto gl_context = graph->get_gpu_resources()->gl_context();
			mediapipe::GlTextureBuffer::DeletionCallback callback;
			if (frame) {
				jobject java_frame = env->NewGlobalRef(frame);
				jclass frame_class = env->GetObjectClass(java_frame);
				jmethodID release_method = env->GetMethodID(frame_class, "release", "()V");
				env->DeleteLocalRef(frame_class);
				callback = [this, java_frame, release_method](mediapipe::GlSyncToken release_token) {
					JNIEnv *env = android_api->godot_android_get_env();
					long token = reinterpret_cast<long>(new mediapipe::GlSyncToken(std::move(release_token)));
					delete reinterpret_cast<mediapipe::GlSyncToken *>(token);
					env->CallVoidMethod(java_frame, release_method);
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
		int camera_facing;
		Vector2 camera_size;
		jobject camera = nullptr;
		Graph *graph;
		String stream_name;
};

jclass CameraHelper::CameraHelperImpl::camera_class = nullptr;

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
		if (permission == "android.permission.CAMERA") {
			if (results[i] == 0) {
				emit_signal("permission_granted");
			} else {
				emit_signal("permission_denied");
			}
			break;
		}
	}
}

bool CameraHelper::permission_granted() {
	return impl->permission_granted();
}

void CameraHelper::request_permission() {
	impl->request_permission();
}

void CameraHelper::set_graph(Graph *graph, String stream_name) {
	impl->set_graph(graph, stream_name);
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
