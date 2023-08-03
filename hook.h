#pragma once

#include "PCH.h"

#include "settings.h"

namespace Hook {

	constexpr auto UpdateQueuedTasks = REL::ID(927422);

	constexpr auto Autosave = REL::ID(1280471);
	constexpr auto QuickSave = REL::ID(1055666);

	struct BGSSaveLoadManager__IsSavingAllowed {

		static inline bool thunk([[maybe_unused]] void* BGSSaveLoadManager)
		{
			return false;
		}

		static inline REL::Relocation<decltype(thunk)> fn;
	};

	void Install() 
	{
		static bool installed{};

		if (installed)
			return;

		auto& settings = Settings::Manager::GetSingleton();

		if (settings.IsAutosave()) {

			stl::write_call<BGSSaveLoadManager__IsSavingAllowed, 5>(Autosave, 0x28);
		
			logger::info("Autosave disabled!");
		}

		if (settings.IsQuicksave()) {

			stl::write_call<BGSSaveLoadManager__IsSavingAllowed, 5>(QuickSave, 0x0c);

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
