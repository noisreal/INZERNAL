#pragma once
#include <windows.h>
#include <string.h>
#include <core/minhook/hook.h>
#include <proton/Variant.h>
#include <core/globals.h>

typedef void(__cdecl* ENetPacketFreeCallback) (struct _ENetPacket*);

typedef struct _ENetPacket {
	size_t reference_count;
	int flags;
	uint8_t* data;
	size_t dataLength;
	ENetPacketFreeCallback freeCallback;
	void* userData;
} ENetPacket;


namespace hooks {

	namespace orig {
		extern types::App_GetVersion App_GetVersion;
		extern types::BaseApp_SetFPSLimit BaseApp_SetFPSLimit;
		extern types::LogMsg LogMsg;
		extern types::NetAvatar_CanMessageT4 NetAvatar_CanMessageT4;
		extern types::CanPunchOrBuildNow CanPunchOrBuildNow;
		extern types::WorldObjectMap_HandlePacket WorldObjectMap_HandlePacket;
		extern types::SendPacketRaw SendPacketRaw;

		extern WNDPROC wndproc;
	}

	void init();
	void destroy();


	float	__cdecl		App_GetVersion(App* app);
	void	__cdecl		BaseApp_SetFPSLimit(BaseApp* ba, float fps);
	int		__cdecl		LogMsg(const char* msg, ...);
	bool	__cdecl		NetAvatar_CanMessageT4(NetAvatar* player);
	bool	__cdecl		CanPunchOrBuildNow(AvatarRenderData* render_data);
	bool	__cdecl		WorldObjectMap_HandlePacket(WorldObjectMap* map, GameUpdatePacket* packet);
	void	__cdecl		SendPacketRaw(int type, void* data, int size, void* a4, void* peer, int flag);





	LRESULT __stdcall hooked_wndproc(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam);

}