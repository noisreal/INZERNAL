#pragma once
#include <core/gt.h>
#include <algorithm>

bool gt::patch_banbypass()
{
	try {
		static auto banbypass = utils::find_pattern("00 3B C1 75 ? 85 C9") + 3;
		if (banbypass <= 0x3) //did not find ban bypass, checking if its already patched
		{
			banbypass = utils::find_pattern("00 3B C1 90 90 85 C9") + 3;
			if (banbypass <= 0x3) //did not find it being even patched, throw error
				throw std::runtime_error("could not find ban bypass");

			printf("ban bypass already patched\n");
		}

		auto bypassed = utils::patch_bytes<2>(banbypass, "\x90\x90");
		if (!bypassed)
			throw std::runtime_error("could not patch ban bypass");
		else {
			printf("patched ban bypass\n");
			return true;
		}
	}
	catch (std::exception exception) {
		printf("exception thrown: %s\n", exception.what());
		utils::read_key();
		return false;
	}
	return false;
}

