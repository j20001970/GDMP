#include "camera_helper.h"

#include "godot_cpp/classes/engine.hpp"
#include "godot_cpp/classes/time.hpp"
#include "godot_cpp/variant/callable.hpp"
#include "godot_cpp/variant/signal.hpp"

#include "mediapipe/framework/packet.h"
#include "mediapipe/gpu/gl_texture_buffer.h"
#include "mediapipe/gpu/gpu_buffer.h"
#include "mediapipe/gpu/gpu_buffer_format.h"

#include "GDMP/framework/packet.h"

using namespace godot;

class MediaPipeCameraHelper::CameraHelperImpl {
	public:
		CameraHelperImpl(MediaPipeCameraHelper *camera_helper) {
			JNIEnv *env = android_api->godot_android_get_env();
			if (env->IsSameObject(camera_class, NULL)) {
				camera_class = reinterpret_cast<jclass>(
						env->NewGlobalRef(env->FindClass("org/godotengine/gdmp/GDMPCameraHelper")));
			}
			android_plugin = Engine::get_singleton()->get_singleton("GDMP");
			ERR_FAIL_COND(android_plugin == nullptr);
			Signal(android_plugin, "camera_permission_granted").connect(Callable(camera_helper, "emit_signal").bind("permission_result", true));
			Signal(android_plugin, "camera_permission_denied").connect(Callable(camera_helper, "emit_signal").bind("permission_result", false));
		}

		~CameraHelperImpl() {}

		jobject create_camera() {
			jobject camera;
			auto context = graph->get_gpu_resources()->gl_context();
			// Create camera object in GL context so that it can get graph context on Java side.
			context->Run([this, &camera]() -> void {
				JNIEnv *env = android_api->godot_android_get_env();
				const char *sig = "(J)V";
				jmethodID cnstr = env->GetMethodID(camera_class, "<init>", sig);
				camera = env->NewObject(camera_class, cnstr, this);
				camera = env->NewGlobalRef(camera);
			});
			return camera;
		}

		bool permission_granted() {
			PoolStringArray permissions = OS::get_singleton()->get_granted_permissions();
			for (int i = 0; i < permissions.size(); i++) {
				const String &permission = permissions[i];
				if (permission == "android.permission.CAMERA")
					return true;
			}
			return false;
		}

		void request_permission() {
			OS::get_singleton()->request_permission("CAMERA");
		}

		void set_graph(Ref<MediaPipeGraph> graph, String stream_name) {
			this->graph = graph;
			this->stream_name = stream_name;
		}

		void start(int index, Vector2 size) {
			ERR_FAIL_COND(graph.is_null());
			ERR_FAIL_COND(!permission_granted());
			close();
			JNIEnv *env = android_api->godot_android_get_env();
			camera = create_camera();
			const char *sig = "(III)V";
			jint width = size.x;
			jint height = size.y;
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
			Ref<MediaPipePacket> packet = memnew(MediaPipePacket(mediapipe::MakePacket<mediapipe::GpuBuffer>(gpu_frame)));
			size_t timestamp = Time::get_singleton()->get_ticks_usec();
			packet->set_timestamp(timestamp);
			graph->add_packet(stream_name, packet);
		}

	private:
		static jclass camera_class;
		jobject camera = nullptr;
		Object *android_plugin;
		Ref<MediaPipeGraph> graph;
		String stream_name;
};

jclass MediaPipeCameraHelper::CameraHelperImpl::camera_class = nullptr;

extern "C" JNIEXPORT void JNICALL Java_org_godotengine_gdmp_GDMPCameraHelper_nativeOnNewFrame(
		JNIEnv *pEnv, jobject jCaller, jlong cppCaller, jobject frame, jint name, jint width, jint height) {
	auto caller = (MediaPipeCameraHelper::CameraHelperImpl *)(cppCaller);
	caller->on_new_frame(pEnv, frame, name, width, height);
}

MediaPipeCameraHelper::MediaPipeCameraHelper() {
	impl = std::make_unique<CameraHelperImpl>(this);
}

MediaPipeCameraHelper::~MediaPipeCameraHelper() = default;

bool MediaPipeCameraHelper::permission_granted() {
	return impl->permission_granted();
}

void MediaPipeCameraHelper::request_permission() {
	impl->request_permission();
}

void MediaPipeCameraHelper::set_graph(Ref<MediaPipeGraph> graph, String stream_name) {
	impl->set_graph(graph, stream_name);
}

void MediaPipeCameraHelper::set_mirrored(bool value) {
}

void MediaPipeCameraHelper::start(int index, Vector2 size) {
	impl->start(index, size);
}

void MediaPipeCameraHelper::close() {
	impl->close();
}

#if !MEDIAPIPE_DISABLE_GPU
void MediaPipeCameraHelper::set_use_gpu(bool use_gpu) {
}
#endif
