#pragma once

#include "PCH.h"

namespace Settings {

	constexpr auto IniFile = "Data\\F4SE\\Plugins\\DisableExitSave.ini";

	class Manager {

	public:

		[[nodiscard]] static Manager& GetSingleton() noexcept 
		{
			static Manager instance;
			return instance;
		}

		void Read() noexcept;

		[[nodiscard]] const auto& IsAutosave() const noexcept { return bDisableAutosave; }
		[[nodiscard]] const auto& IsQuicksave() const noexcept { return bDisableQuicksave; }
		[[nodiscard]] const auto& IsAutoLoad() const noexcept { return bDisableAutoload; }
		[[nodiscard]] const auto& IsSaveGameOnQuitToMainMenu() const noexcept { return bSaveGameOnQuitToMainMenu; }
		[[nodiscard]] const auto& IsSaveGameOnQuitToDesktop() const noexcept { return bSaveGameOnQuitToDesktop; }

	private:

		Manager() noexcept { Read(); }
		~Manager() = default;

		Manager(const Manager&) = delete;
		Manager(Manager&&) = delete;

		Manager& operator=(const Manager&) = delete;
		Manager& operator=(Manager&&) = delete;

		bool bDisableAutosave{};
		bool bDisableQuicksave{};
		bool bDisableAutoload{};
		bool bSaveGameOnQuitToMainMenu{};
		bool bSaveGameOnQuitToDesktop{};
	};
}