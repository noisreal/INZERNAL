#pragma once
#include <core/gt.h>
#include <algorithm>

std::string gt::generate_mac(const std::string& prefix) {
    std::string x = prefix + ":";
    for (int i = 0; i < 5; i++) {
        x += utils::hex_str(utils::random(0, 255));
        if (i != 4)
            x += ":";
    }
    return x;
}

std::string gt::generate_rid() {
    std::string rid_str;

    for (int i = 0; i < 16; i++)
        rid_str += utils::hex_str(utils::random(0, 255));

    std::transform(rid_str.begin(), rid_str.end(), rid_str.begin(), std::toupper);

    return rid_str;
}

std::string gt::generate_meta() {
    return utils::rnd(utils::random(5, 10)) + ".com";
}

bool gt::patch_banbypass() {
	try {
		static auto banbypass = utils::find_pattern("00 3B C1 75 ? 85 C9", false) + 3;
		if (banbypass <= 0x3) //did not find ban bypass, checking if its already patched
		{
			banbypass = utils::find_pattern("00 3B C1 90 90 85 C9") + 3;
			if (banbypass <= 0x3) //did not find it being even patched, throw error
				throw std::runtime_error("could not find ban bypass");

			printf("ban bypass already patched\n");
			return true;
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

