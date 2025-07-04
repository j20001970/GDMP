#ifndef GDMP_TASK
#define GDMP_TASK

#include <functional>
#include <map>
#include <memory>
#include <string>

#include "Godot.hpp"
#include "PoolArrays.hpp"
#include "Reference.hpp"
#include "String.hpp"

#include "mediapipe/tasks/cc/core/base_options.h"

using namespace godot;
using namespace mediapipe::tasks::core;

// Common properties of a GDMP task class.
// bool _registered: a internal bool for indicating that if this task has been registered.
// std::map<std::string, std::function<void()>> get_register_map(): a map of task classes that are registered with this task.
// void _register_tasks(): for registering task classes inside the register map
// void _register_task(): for registering this task and associated classes to godot ClassDB.
// register_task(const std::string&, std::function<void()>): for registering other task classes with this task.
#define GDMP_TASK_CLASS                                                              \
private:                                                                             \
	static bool _registered;                                                         \
	static std::map<std::string, std::function<void()>> &get_register_map() {        \
		static std::map<std::string, std::function<void()>> register_map;            \
		return register_map;                                                         \
	}                                                                                \
	static void _register_tasks() {                                                  \
		for (auto func : get_register_map())                                         \
			func.second();                                                           \
	}                                                                                \
	static void _register_task();                                                    \
                                                                                     \
protected:                                                                           \
	static bool register_task(const std::string &name, std::function<void()> func) { \
		auto &register_map = get_register_map();                                     \
		if (auto it = register_map.find(name); it == register_map.end())             \
			register_map.insert(std::make_pair(name, func));                         \
		return true;                                                                 \
	}

// Register TASK_CLASS with WITH_CLASS.
// WITH_CLASS should be the direct base class of TASK_CLASS to ensure the order of registration.
#define GDMP_REGISTER_TASK(TASK_CLASS, WITH_CLASS) \
	bool TASK_CLASS::_registered = WITH_CLASS::register_task(#TASK_CLASS, TASK_CLASS::_register_task);

class MediaPipeTaskBaseOptions : public Reference {
		GODOT_CLASS(MediaPipeTaskBaseOptions, Reference)

	private:
		PoolByteArray model_asset_buffer;
		String model_asset_path;
		int delegate;

	public:
		static void _register_methods();

		enum Delegate {
			DELEGATE_CPU = BaseOptions::Delegate::CPU,
			DELEGATE_GPU = BaseOptions::Delegate::GPU,
			DELEGATE_EDGETPU_NNAPI = BaseOptions::Delegate::EDGETPU_NNAPI,
		};

		void _init();

		PoolByteArray get_model_asset_buffer();
		void set_model_asset_buffer(PoolByteArray buffer);

		String get_model_asset_path();
		void set_model_asset_path(String path);

		int get_delegate();
		void set_delegate(int delegate);

		std::unique_ptr<BaseOptions> get_base_options();
};

class MediaPipeTask : public Reference {
		GDMP_TASK_CLASS

	public:
		static void register_tasks();
};

#endif
