#pragma once

#include "PCH.h"

#include "settings.h"

namespace Hook {

	constexpr auto QueueSaveLoadTask = REL::ID(1487308);
	constexpr auto UpdateQueuedTasks = REL::ID(927422);

	enum class TaskType : std::uint32_t {

		Nothing,
		Autosave = 1,
		Autosave2 = 2,
		Autoload = 4,
		QuickSave = 8,
		SaveGameOnQuitToMainMenu = 16384,
		SaveGameOnQuitToDesktop = 65536
	};

	struct BGSSaveLoadManager__QueueSaveLoadTask {

		static inline void thunk(void* a_this, TaskType a_type)
		{
			auto& settings = Settings::Manager::GetSingleton();

			switch (a_type) {

			case TaskType::Autosave:
			case TaskType::Autosave2:

				if (settings.IsAutosave())
					a_type = TaskType::Nothing;

				break;

			case TaskType::QuickSave:

				if (settings.IsQuicksave())
					a_type = TaskType::Nothing;

				break;

			case TaskType::Autoload:
				
				if (settings.IsAutoLoad())
					a_type = TaskType::Nothing;

				break;

			case TaskType::SaveGameOnQuitToMainMenu:
			case TaskType::SaveGameOnQuitToDesktop:

				break;

			default:

				logger::info("unknown value : {}", static_cast<std::uint32_t>(a_type));
			
				break;
			}

			a_fn(a_this, a_type);
		}

		static void Install() noexcept
		{
			struct QueueSaveLoadTask_Code : Xbyak::CodeGenerator {

				QueueSaveLoadTask_Code()
				{
					Xbyak::Label retnLabel;

					mov(ptr[rsp + 0x10], rbx);

					jmp(ptr[rip + retnLabel]);

					L(retnLabel);

					dq(QueueSaveLoadTask.address() + 5);
				}
			};

			F4SE::AllocTrampoline(64);

			auto& trampoline = F4SE::GetTrampoline();

			QueueSaveLoadTask_Code code;

			code.ready();

			auto mem = trampoline.allocate(code.getSize());
			std::memcpy(mem, code.getCode(), code.getSize());
			a_fn = (QueueSaveLoadTask_Func)mem;

			trampoline.write_branch<5>(QueueSaveLoadTask.address(), BGSSaveLoadManager__QueueSaveLoadTask::thunk);
		}

		using QueueSaveLoadTask_Func = decltype(&thunk);

		static QueueSaveLoadTask_Func a_fn;
	};

	BGSSaveLoadManager__QueueSaveLoadTask::QueueSaveLoadTask_Func BGSSaveLoadManager__QueueSaveLoadTask::a_fn;

	void Install() 
	{
		static bool installed{};

		if (installed)
			return;

		auto& settings = Settings::Manager::GetSingleton();

		BGSSaveLoadManager__QueueSaveLoadTask::Install();

		if (settings.IsAutoLoad()) {

			logger::info("Autoload disabled!");
		}

		if (settings.IsAutosave()) {
		
			logger::info("Autosave disabled!");
		}

		if (settings.IsQuicksave()) {

			logger::info("Quicksave disabled!");
		}

		std::uint8_t buff[]{ 0x90, 0x90, 0x90, 0x90, 0x90 };

		if (settings.IsSaveGameOnQuitToMainMenu()) {

			stl::write_value(UpdateQueuedTasks, 0x00e9, buff, sizeof buff);
		
			logger::info("Savegame on quit to main menu disabled!");
		}

		if (settings.IsSaveGameOnQuitToDesktop()) {

			stl::write_value(UpdateQueuedTasks, 0x016b, buff, sizeof buff);
		
			logger::info("Savegame on quit to desktop disabled!");
		}

		installed = true;	
	}
}
