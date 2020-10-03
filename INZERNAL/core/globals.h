#pragma once
#include <Windows.h>
#include <d3d9.h>
#include <proton/Variant.h>
#include <proton/clanlib/vec2.h>
#include <stdio.h>
#include <atomic>
#include <core/json.hpp>
#include <map>
#include <string>

// clang-format off


//from private internal, dont remember what I needed these for so I just brought them over for now
template<typename t> inline constexpr t operator&(t x, t y) { return static_cast<t> (static_cast<int>(x) & static_cast<int>(y)); }
template<typename t> inline constexpr t operator|(t x, t y) { return static_cast<t> (static_cast<int>(x) | static_cast<int>(y)); }
template<typename t> inline constexpr t operator^(t x, t y) { return static_cast<t> (static_cast<int>(x) ^ static_cast<int>(y)); }
template<typename t> inline constexpr t operator~(t x) { return static_cast<t>(~static_cast<int>(x)); }
template<typename t> inline t& operator&=(t& x, t y) { x = x & y;	return x; }
template<typename t> inline t& operator|=(t& x, t y) { x = x | y;	return x; }
template<typename t> inline t& operator^=(t& x, t y) { x = x ^ y;	return x; }

class BaseApp {public:};
class GameUpdatePacket;
class App : BaseApp {public:};
class AvatarRenderData;
class WorldObjectMap;
class NetAvatar;
class LevelTouchComponent;
class WorldCamera;

namespace types {
	//hooks
	using	App_GetVersion				= float(__cdecl*)(App*);
	using	BaseApp_SetFPSLimit			= void(__cdecl*)(BaseApp*, float);
	using	LogMsg						= int(__cdecl*)(const char*, ...);
	using	NetAvatar_CanMessageT4		= bool(__cdecl*)(NetAvatar*);
	using	CanPunchOrBuildNow			= bool(__cdecl*)(AvatarRenderData*);
	using	ObjectMap_HandlePacket		= bool(__cdecl*)(WorldObjectMap*, GameUpdatePacket*);
	using	SendPacketRaw				= void(__cdecl*)(int, void*, int, void*, void*, int);
	using	HandleTouch					= void(__cdecl*)( LevelTouchComponent*, CL_Vec2f, bool);


	//other functions, not hooked
	using	WorldToScreen				= void(__cdecl*)(WorldCamera*, CL_Vec2f&, CL_Vec2f&);
}

//from private internal, might be used later on, so bringing it for now
namespace col {
	struct im {
		float red;
		float green;
		float blue;
		float alpha;
	};

	//GT argb
#define RGBA(r,g,b,a)(((unsigned char)(a)) +  ((unsigned char)(r) << 8) + ((unsigned char)(g) << 16) + ((unsigned char)(b) << 24))

	int im_to_gt(im& c);
	im gt_to_im(int gt);
}


namespace opt {
	extern float	gt_version;
	extern float	fps_limit;
	extern bool		tp_click;

	namespace cheat {
		extern bool		punch_cooldown_on;
		extern float	punch_cooldown_val;
	}
	
}

namespace global {
	extern HMODULE	self;
	extern App*		app;
	extern bool		unload;
	extern HWND		hwnd;
	extern void*	gt;
	//	extern nlohmann::json m_cfg;
}

// clang-format on