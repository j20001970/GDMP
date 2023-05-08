#include "camera_helper.h"

#include "godot_cpp/classes/engine.hpp"
#include "godot_cpp/classes/os.hpp"
#include "godot_cpp/classes/time.hpp"
#include "godot_cpp/variant/callable.hpp"
#include "godot_cpp/variant/signal.hpp"

#include "mediapipe/framework/packet.h"
#include "mediapipe/gpu/gl_texture_buffer.h"
#include "mediapipe/gpu/gpu_buffer.h"
#include "mediapipe/gpu/gpu_buffer_format.h"

#include "GDMP/android/jni.h"
#include "GDMP/framework/packet.h"

class MediaPipeCameraHelper::Impl {
	private:
		static jclass camera_class;

		MediaPipeCameraHelper *camera_helper;
		Object *android_plugin;
		jobject camera = nullptr;
		std::shared_ptr<mediapipe::GpuResources> gpu_resources;

		bool get_env(JNIEnv **env) {
			jint res = jvm->GetEnv((void **)env, JNI_VERSION_1_6);
			if (res == JNI_EDETACHED) {
				res = jvm->AttachCurrentThread(env, NULL);
				ERR_FAIL_COND_V(res != JNI_OK, false);
				return true;
			}
			return false;
		}

	public:
		Impl(MediaPipeCameraHelper *camera_helper) {
			JNIEnv *env;
			get_env(&env);
			if (env->IsSameObject(camera_class, NULL)) {
				camera_class = reinterpret_cast<jclass>(
						env->NewGlobalRef(env->FindClass("org/godotengine/gdmp/GDMPCameraHelper")));
			}
			android_plugin = Engine::get_singleton()->get_singleton("GDMP");
			ERR_FAIL_COND(android_plugin == nullptr);
			Signal(android_plugin, "camera_permission_granted").connect(Callable((Object *)camera_helper, "emit_signal").bindv(Array::make("permission_result", true)));
			Signal(android_plugin, "camera_permission_denied").connect(Callable((Object *)camera_helper, "emit_signal").bindv(Array::make("permission_result", false)));
			this->camera_helper = camera_helper;
		}

		~Impl() {}

		jobject create_camera() {
			jobject camera;
			auto context = gpu_resources->gl_context();
			// Create camera object in GL context so that it can get GL context on Java side.
			context->Run([this, &camera]() -> void {
				JNIEnv *env;
				bool attach = get_env(&env);
				const char *sig = "(J)V";
				jmethodID cnstr = env->GetMethodID(camera_class, "<init>", sig);
				camera = env->NewObject(camera_class, cnstr, this);
				camera = env->NewGlobalRef(camera);
				if (attach)
					jvm->DetachCurrentThread();
			});
			return camera;
		}

		bool permission_granted() {
			PackedStringArray permissions = OS::get_singleton()->get_granted_permissions();
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

		void start(int index, Vector2 size) {
			ERR_FAIL_COND(!permission_granted());
			ERR_FAIL_COND(gpu_resources == nullptr);
			close();
			JNIEnv *env;
			get_env(&env);
			camera = create_camera();
			const char *sig = "(III)V";
			jint width = size.x;
			jint height = size.y;
			env->CallVoidMethod(camera, env->GetMethodID(camera_class, "startCamera", sig), index, width, height);
		}

		void close() {
			JNIEnv *env;
			get_env(&env);
			if (camera && !env->IsSameObject(camera, NULL)) {
				env->CallVoidMethod(camera, env->GetMethodID(camera_class, "closeCamera", "()V"));
				env->DeleteGlobalRef(camera);
				camera = nullptr;
			}
		}

		void set_gpu_resources(Ref<MediaPipeGPUResources> gpu_resources) {
			if (gpu_resources.is_null()) {
				this->gpu_resources = nullptr;
				return;
			}
			this->gpu_resources = gpu_resources->get_gpu_resources();
		}

		void on_new_frame(JNIEnv *env, jobject frame, int name, int width, int height) {
			auto gl_context = gpu_resources->gl_context();
			mediapipe::GlTextureBuffer::DeletionCallback callback;
			if (frame) {
				jobject java_frame = env->NewGlobalRef(frame);
				jclass frame_class = env->GetObjectClass(java_frame);
				jmethodID release_method = env->GetMethodID(frame_class, "release", "()V");
				env->DeleteLocalRef(frame_class);
				callback = [this, java_frame, release_method](mediapipe::GlSyncToken release_token) {
					JNIEnv *env;
					bool attach = get_env(&env);
					long token = reinterpret_cast<long>(new mediapipe::GlSyncToken(std::move(release_token)));
					delete reinterpret_cast<mediapipe::GlSyncToken *>(token);
					env->CallVoidMethod(java_frame, release_method);
					env->DeleteGlobalRef(java_frame);
					if (attach)
						jvm->DetachCurrentThread();
				};
			}
			mediapipe::GpuBuffer gpu_frame = mediapipe::GpuBuffer(mediapipe::GlTextureBuffer::Wrap(
					GL_TEXTURE_2D, name, width, height, mediapipe::GpuBufferFormat::kBGRA32, gl_context, callback));
			Ref<MediaPipePacket> packet = memnew(MediaPipePacket(mediapipe::MakePacket<mediapipe::GpuBuffer>(gpu_frame)));
			size_t timestamp = Time::get_singleton()->get_ticks_usec();
			packet->set_timestamp(timestamp);
			camera_helper->emit_signal("new_frame", packet);
		}
};

jclass MediaPipeCameraHelper::Impl::camera_class = nullptr;

extern "C" JNIEXPORT void JNICALL Java_org_godotengine_gdmp_GDMPCameraHelper_nativeOnNewFrame(
		JNIEnv *pEnv, jobject jCaller, jlong cppCaller, jobject frame, jint name, jint width, jint height) {
	auto caller = (MediaPipeCameraHelper::Impl *)(cppCaller);
	caller->on_new_frame(pEnv, frame, name, width, height);
}

MediaPipeCameraHelper::MediaPipeCameraHelper() {
	impl = std::make_unique<Impl>(this);
}

MediaPipeCameraHelper::~MediaPipeCameraHelper() = default;

bool MediaPipeCameraHelper::permission_granted() {
	return impl->permission_granted();
}

void MediaPipeCameraHelper::request_permission() {
	impl->request_permission();
}

void MediaPipeCameraHelper::set_mirrored(bool value) {}

void MediaPipeCameraHelper::start(int index, Vector2 size) {
	impl->start(index, size);
}

void MediaPipeCameraHelper::close() {
	impl->close();
}

void MediaPipeCameraHelper::set_gpu_resources(Ref<MediaPipeGPUResources> gpu_resources) {
	impl->set_gpu_resources(gpu_resources);
}
