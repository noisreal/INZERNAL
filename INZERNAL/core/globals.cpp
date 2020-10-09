#pragma once
#include "globals.h"


// clang-format off

//options
namespace opt {

	//enhanchements 
	float		fps_limit	=	144.0f;		//fps limit gets forced to this
	float		gt_version	=	3.45f;		//gt version gets spoofed to this
	bool		tp_click	=	true;		//when on, holding CTRL allows you to tp to mouse location.
	bool		mod_zoom	=	true;		//unlimited zoom. Cant ban you. cant punch further, only view. NOTE: If for some reason you can punch further, re-join.
	bool		spoof_login	=	true;		//whether or not to spoof mac, hash2, wk, rid, etc on login. GT version spoofed even when off.
	bool		spoof_name	=	true;		//Also set the requestedName field to random. Might be unwanted for non grow-id accounts.
	WinSpoof	spoof_win	=	::wsMimic;	//Mode of zf/fz spoofing mode. See the WinSpoof enum for more details.
	FlagMode	flag_mode	=	::fmRandom; //See FlagMode enum for details.
	std::string	flag		=	"us";		//set if flag_mode is set to fmCustom

	//cheats
	namespace cheat {
		bool	punch_cooldown_on	=	true;	//toggle: punch cooldown
		float	punch_cooldown_val	=	0.15f;	//punch cooldown timer. i.e fast punch
		bool	dev_zoom			=	false;	//unlimited zoom + unlimited build/punch. The latter can ban, use at your own caution.
	}
	
}

//other variables
namespace global {
	HMODULE		self	=	NULL;		//pointer to the dll module
	HWND		hwnd	=	NULL;		//pointer to hwnd of Growtopia window
	App*		app		=	nullptr;	//pointer to App instance
	bool		unload	=	false;		//proces terminates when set to true
	void*		gt		=	nullptr;	//pointer to growtopia base address
	std::string	version	=	"0.3";		//INZERNAL version
}

// clang-format on

//unused for now, from private internal, might use later.
namespace col {
    int col::im_to_gt(im& c) {
        auto a = uint8_t(c.alpha * 255);
        auto r = uint8_t(c.red * 255);
        auto g = uint8_t(c.green * 255);
        auto b = uint8_t(c.blue * 255);
        return RGBA(r, g, b, a);
    }
    im gt_to_im(int gt) {
        return { (float)HIBYTE(LOWORD(gt)) / 255.f, (float)LOBYTE(HIWORD(gt)) / 255.f, (float)HIBYTE(HIWORD(gt)) / 255.f, (float)LOBYTE(LOWORD(gt)) / 255.f };
    }
} // namespace col