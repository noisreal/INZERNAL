#pragma once
#include <string>
#include "globals.h"


//options
namespace opt {

	//enhanchements 
	float	fps_limit	=	144.0f;		//fps limit gets forced to this
	float	gt_version	=	3.45f;		//gt version gets spoofed to this

	//cheats
	namespace cheat {
		bool	punch_cooldown_on	=	true;	//toggle: punch cooldown
		float	punch_cooldown_val	=	0.15f;	//punch cooldown timer. i.e fast punch
	}
	
}

//other variables
namespace global {
	HMODULE	self	=	NULL;		//pointer to the dll module
	HWND	hwnd	=	NULL;		//pointer to hwnd of Growtopia window
	App*	app		=	nullptr;	//pointer to App instance
	bool	unload	=	false;		//proces terminates when set to true
	void*	gt		=	nullptr;	//pointer to growtopia base address
}




//unused for now, from private internal, might use later.
namespace col {
	int col::im_to_gt(im& c) {
		auto a = uint8_t(c.alpha * 255);
		auto r = uint8_t(c.red * 255);
		auto g = uint8_t(c.green * 255);
		auto b = uint8_t(c.blue * 255);
		return RGBA(r, g, b, a);
	}
	im gt_to_im(int gt)
	{
		return {
		 (float)HIBYTE(LOWORD(gt)) / 255.f,
		 (float)LOBYTE(HIWORD(gt)) / 255.f,
		 (float)HIBYTE(HIWORD(gt)) / 255.f,
		 (float)LOBYTE(LOWORD(gt)) / 255.f
		};
	}
}