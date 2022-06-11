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
		Object *android_plugin;

		CameraHelperImpl() {
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
			ERR_FAIL_COND(graph == nullptr);
			ERR_FAIL_COND(!permission_granted());
			close();
			JNIEnv *env = android_api->godot_android_get_env();
			camera = create_camera();
			const char *sig = "(III)V";
			// CameraXPreviewHelper transpose target size unconditionally, therefore we transpose it back here.
			jint width = size.y;
			jint height = size.x;
			env->CallVoidMethod(camera, env->GetMethodID(camera_class, "startCamera", sig), index, width, height);
		}

		void close() {
			JNIEnv *env = android_api->godot_android_get_env();
			if (camera && !env->IsSameObject(camera, NULL)) {
				env->CallVoidMethod(camera, env->GetMethodID(camera_class, "closeCamera", "()V"));
				env->DeleteGlobalRef(camera);
				camera = nullptr;
			}
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
		jobject camera = nullptr;
		Graph *graph;
		String stream_name;
};

jclass CameraHelper::CameraHelperImpl::camera_class = nullptr;

extern "C" JNIEXPORT void JNICALL Java_org_godotengine_gdmp_GDMPCameraHelper_nativeOnNewFrame(
		JNIEnv *pEnv, jobject jCaller, jlong cppCaller, jobject frame, jint name, jint width, jint height) {
	auto caller = (CameraHelper::CameraHelperImpl *)(cppCaller);
	caller->on_new_frame(pEnv, frame, name, width, height);
}

CameraHelper::CameraHelper() = default;

CameraHelper::~CameraHelper() = default;

void CameraHelper::_init() {
	impl = std::make_unique<CameraHelperImpl>();
	if (impl->android_plugin) {
		impl->android_plugin->connect(
				"camera_permission_granted", this, "emit_signal", Array::make("permission_granted"));
		impl->android_plugin->connect(
				"camera_permission_denied", this, "emit_signal", Array::make("permission_denied"));
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
