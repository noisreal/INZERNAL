#pragma once
#include <windows.h>
#include <thread>
#include <hooks/hooks.h>
#include <core/gt.h>
#include <stdio.h>
#include <intrin.h>
#include <iomanip>
#include <core/utils.h>
#include <sdk/sdk.h>

#define ORIGINAL(x) types::x  hooks::orig::##x {}; 
#define MAKEHOOK(x) MH_CreateHook(LPVOID(##x), hooks::##x, (void**)(&orig::##x));

//necessary to set original hook
ORIGINAL(App_GetVersion);
ORIGINAL(BaseApp_SetFPSLimit);
ORIGINAL(LogMsg);
ORIGINAL(NetAvatar_CanMessageT4);
ORIGINAL(CanPunchOrBuildNow);
ORIGINAL(WorldObjectMap_HandlePacket);
ORIGINAL(SendPacketRaw);

WNDPROC hooks::orig::wndproc; //wndproc is special case

IDirect3DDevice9* device = nullptr;
bool canrender = false;

void hooks::init()
{
	global::hwnd = FindWindowA(nullptr, "Growtopia");

	const auto base = global::gt;
	IDirect3D9* pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (!pD3D)
		return;

	D3DPRESENT_PARAMETERS d3dpp{ 0 };
	d3dpp.hDeviceWindow = global::hwnd;
	d3dpp.SwapEffect = D3DSWAPEFFECT_COPY;
	d3dpp.Windowed = TRUE;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
	if (FAILED(pD3D->CreateDevice(0, D3DDEVTYPE_HAL, d3dpp.hDeviceWindow, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &device)))
	{
		printf("fail: creating device\n");
		pD3D->Release();
		return;
	}
	auto vtable = *reinterpret_cast<void***>(device);;
	if (device) {
		device->Release();
		device = nullptr;
	}

	//this is where all the function signatures are 
	auto
		App_GetVersion = utils::find_func_start("28 FF 15 ?? ?? ?? ?? FF 15"),
		BaseApp_SetFPSLimit = utils::find_func_start("00 00 0F 57 C0 0F 2F C8 72"),
		LogMsg = utils::find_func_start("00 28 00 00 45"),
		CanMessageT4 = detail::get_call("48 8b ce e8 ? ? ? ? 84 c0 74 ? e8", 3),
		CanPunchOrBuildNow = utils::find_func_start("00 00 83 e9 03 74 ? 83 e9 01 74 ? 83 e9 01"),
		WorldObjectMap_HandlePacket = utils::find_func_start("44 8B ?? ?? 41 83 f8 FF 75 ?? 44"),
		SendPacketRaw = utils::find_func_start("00 81 FE 40 42 0F 00");

	MAKEHOOK(App_GetVersion);
	MAKEHOOK(BaseApp_SetFPSLimit);
	MAKEHOOK(LogMsg);
	MAKEHOOK(NetAvatar_CanMessageT4);
	MAKEHOOK(CanPunchOrBuildNow);
	MAKEHOOK(WorldObjectMap_HandlePacket);
	MAKEHOOK(SendPacketRaw);

	orig::wndproc = WNDPROC(SetWindowLongPtrW(global::hwnd, -4, LONG_PTR(hooked_wndproc)));


	MH_EnableHook(MH_ALL_HOOKS);
	utils::printc("93", "Hooks have been setup!");

}


void hooks::destroy() {
RETRY:
	if (MH_OK != MH_DisableHook(MH_ALL_HOOKS)) {
		auto res = MessageBox(GetForegroundWindow(), L"Hooks could not be reverted", L"Internal", MB_ABORTRETRYIGNORE | MB_ICONERROR | MB_TOPMOST);
		switch (res) {
		case IDABORT:
			while (!TerminateProcess(GetCurrentProcess(), EXIT_SUCCESS));
		case IDRETRY:
			goto RETRY;
		case IDIGNORE: // just ignore
			break;
		}
	}
	SetWindowLongPtr(global::hwnd, -4, LONG_PTR(orig::wndproc));
}

float __cdecl hooks::App_GetVersion(App* app)
{
	if (!global::app) {
		global::app = app;
		orig::BaseApp_SetFPSLimit((BaseApp*)app, opt::fps_limit);
		utils::printc("93", "Modified FPS limit!");
	}
	;
	float version = opt::gt_version;
	utils::printc("93", "spoofed version: %.3f", version);
	Sleep(100);
	return version;
}

void __cdecl hooks::BaseApp_SetFPSLimit(BaseApp* ba, float fps)
{
	utils::printc("93", "[INZERNAL]\trequest to set fps to %.0f, setting to %.0f instead", fps, opt::fps_limit);
	orig::BaseApp_SetFPSLimit(ba, opt::fps_limit);
}

int __cdecl hooks::LogMsg(const char* msg, ...)
{
	char buf[0x1000u];
	int len = -1;
	va_list va;
	va_start(va, msg);
	if (0 > (len = _vsnprintf_s(buf, sizeof(buf), msg, va))) {
		va_end(va);
		return 0;
	}
	va_end(va);

	if (len < 90 && len > 2)
		utils::printc("88", "LogMsg\t%s", buf);
	return orig::LogMsg(buf);
}

bool __cdecl hooks::NetAvatar_CanMessageT4(NetAvatar* player)
{
	printf("CanMessageT4 called!!!\n");
	return false;
}

bool active = false;
LRESULT __stdcall hooks::hooked_wndproc(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	active = GetActiveWindow() == global::hwnd;

	if (msg == WM_KEYDOWN && wparam == VK_ESCAPE)
		return true;

	//TODO: with imgui
	//if (orig::wndproc(wnd, msg, wparam, lparam))
	//	return true;

	if (msg == WM_KEYDOWN && (wparam == VK_CONTROL || wparam == VK_LCONTROL || wparam == VK_RCONTROL))
		return true;

	if (msg == WM_KEYUP && wparam == VK_F3)
		global::unload = true;

	return CallWindowProcW(hooks::orig::wndproc, wnd, msg, wparam, lparam);
}
bool __cdecl hooks::CanPunchOrBuildNow(AvatarRenderData* render_data)
{
	if (opt::cheat::punch_cooldown_on) {
		static auto time_before = std::chrono::system_clock::now();
		std::chrono::duration<double> elapsed_sec = std::chrono::system_clock::now() - time_before;
		if (elapsed_sec.count() > opt::cheat::punch_cooldown_val) {
			time_before = std::chrono::system_clock::now();
			return true;
		}
		else return false;
	}

	return orig::CanPunchOrBuildNow(render_data);
}

bool __cdecl hooks::WorldObjectMap_HandlePacket(WorldObjectMap* map, GameUpdatePacket* packet)
{
	//CL_Vec2f pos_obj{ packet->pos_x, packet->pos_y };
	//auto player = sdk::gamelogic()->GetLocalPlayer();
	//auto ret = orig::WorldObjectMap_HandlePacket(map, packet);
	//if (player && pos_obj.x > 0.1f && pos_obj.y > 0.1f && ret) {

	//	/*	player->set_pos(packet->pos_x - 4.f, packet->pos_y - 4.f);
	//		player->set_pos(packet->pos_x, packet->pos_y - 4.f);
	//		player->set_pos(packet->pos_x - 4.f, packet->pos_y);
	//		player->set_pos(packet->pos_x , packet->pos_y );
	//		player->set_pos(packet->pos_x + 4.f, packet->pos_y + 4.f);
	//		player->set_pos(packet->pos_x, packet->pos_y + 4.f);
	//		player->set_pos(packet->pos_x + 4.f, packet->pos_y);*/

	//		/*pickup.active = true;
	//		pickup.pos = pos_obj;
	//		pickup.netid = packet->netid;
	//		time_ = std::chrono::system_clock::now() + std::chrono::milliseconds(150);*/

	//		/*	auto pos_pl = player->get_pos();
	//			utils::print("%f %f\n", pos_pl.x, pos_pl.y);*/
	//			//utils::print("%f %f\n", player->m_pos.x, player->m_pos.y);
	//	packet->debug_print("packet");
	//	utils::print("%d %f %f\n", packet->netid, packet->pos_x, packet->pos_y);
	//}

	return orig::WorldObjectMap_HandlePacket(map, packet);
}

void __cdecl hooks::SendPacketRaw(int type, void* data, int size, void* a4, void* peer, int flag)
{
	GameUpdatePacket* packet = (GameUpdatePacket*)data;
	packet->debug_print("sendpacketraw");
	orig::SendPacketRaw(type, data, size, a4, peer, flag);
}