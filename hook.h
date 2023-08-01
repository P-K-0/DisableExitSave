#pragma once

#include "PCH.h"

namespace Hook {

	void Install() 
	{
		static bool installed{};

		if (installed)
			return;

		std::uint8_t buff[]{ 0x90, 0x90, 0x90, 0x90, 0x90 };

		stl::write_value(REL::ID(927422), 0x16b, buff, sizeof buff);

		installed = true;	
	}
}
