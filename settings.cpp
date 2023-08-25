
#include "settings.h"

namespace Settings {

	inline const char* GetValue(CSimpleIni& ini, const char* section, const char* key, const char* def) noexcept
	{
		auto ret = ini.KeyExists(section, key);

		if (ret)
			return ini.GetValue(section, key, def);

		ini.SetValue(section, key, def);
		ini.SaveFile(IniFile);

		return def;
	}

	void Manager::Read() noexcept
	{

		CSimpleIni ini;

		ini.SetUnicode();
		auto error = ini.LoadFile(IniFile);

		if (error != SI_OK) {

			ini.LoadData(
				"[Main]\n\n"
				"bDisableAutosave = 0\n"
				"bDisableQuicksave = 0\n"
				"bDisableAutoLoad = 0\n"
				"bSaveGameOnQuitToMainMenu = 1\n"
				"bSaveGameOnQuitToDesktop = 1\n"
			);

			ini.SaveFile(IniFile);
		}

#define INI_GETVALUE(v, f, s, d) \
		v = f(GetValue(ini, s, # v, std::to_string(d).c_str()));

		INI_GETVALUE(bDisableAutosave, std::stoi, "Main", 0);
		INI_GETVALUE(bDisableQuicksave, std::stoi, "Main", 0);
		INI_GETVALUE(bDisableAutoload, std::stoi, "Main", 0);
		INI_GETVALUE(bSaveGameOnQuitToMainMenu, std::stoi, "Main", 1);
		INI_GETVALUE(bSaveGameOnQuitToDesktop, std::stoi, "Main", 1);

		logger::info("Settings read!");
	}
}
