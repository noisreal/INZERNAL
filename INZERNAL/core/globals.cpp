#pragma once
#include <string>
#include "globals.h"


//options
float opt::fps_limit = 144.0f;				//fps limit gets forced to this
std::string opt::gt_version = "3.45";		//gt version gets spoofed to this

//other variables
HMODULE global::self = NULL;
HWND global::hwnd = NULL;
App* global::app = nullptr;
bool global::unload = false;


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