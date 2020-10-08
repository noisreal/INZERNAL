#pragma once
#include <core/gt.h>
#include <core/utils.h>
#include <hooks/hooks.h>
#include <hooks\SendPacketRaw.h>
#include <intrin.h>
#include <sdk/sdk.h>
#include <stdio.h>
#include <windows.h>
#include <iomanip>
#include <thread>
#include <hooks\SendPacket.h>

#define ORIGINAL(x) types::x hooks::orig::##x{};
#define MAKEHOOK(x) MH_CreateHook(LPVOID(##x), hooks::##x, (void**)(&orig::##x));

//necessary to set original hook
ORIGINAL(App_GetVersion);
ORIGINAL(BaseApp_SetFPSLimit);
ORIGINAL(LogMsg);
ORIGINAL(NetAvatar_CanMessageT4);
ORIGINAL(CanPunchOrBuildNow);
ORIGINAL(ObjectMap_HandlePacket);
ORIGINAL(SendPacketRaw);
ORIGINAL(HandleTouch);
ORIGINAL(WorldCamera_OnUpdate);
ORIGINAL(UpdateFromNetAvatar);
ORIGINAL(SendPacket);

WNDPROC hooks::orig::wndproc; //wndproc is special case

IDirect3DDevice9* device = nullptr;
bool canrender = false;

void hooks::init() {
    utils::unprotect_process();

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
    if (FAILED(pD3D->CreateDevice(0, D3DDEVTYPE_HAL, d3dpp.hDeviceWindow, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &device))) {
        printf("fail: creating device\n");
        pD3D->Release();
        return;
    }
    auto vtable = *reinterpret_cast<void***>(device);
    if (device) {
        device->Release();
        device = nullptr;
    }

    // clang-format off


	//this is where all the function signatures are 
	auto
		App_GetVersion                  = utils::find_func_start("28 FF 15 ?? ?? ?? ?? FF 15"),
		BaseApp_SetFPSLimit             = utils::find_func_start("00 00 0F 57 C0 0F 2F C8 72"),
		LogMsg                          = utils::find_func_start("00 28 00 00 45"),
		CanMessageT4                    = detail::get_call("48 8b ce e8 ? ? ? ? 84 c0 74 ? e8", 3),
		CanPunchOrBuildNow              = utils::find_func_start("00 00 83 e9 03 74 ? 83 e9 01 74 ? 83 e9 01"),
		ObjectMap_HandlePacket          = utils::find_func_start("44 8B ?? ?? 41 83 f8 FF 75 ?? 44"),
		SendPacketRaw                   = utils::find_func_start("00 81 FE 40 42 0F 00"),
		HandleTouch                     = utils::find_func_start("83 B8 ?? ?? ?? ?? 12 75"),
        WorldCamera_OnUpdate            = utils::find_func_start("89 43 10 0f 2f"),
        UpdateFromNetAvatar             = utils::find_func_start("32 21 00 00 66 39"),
        SendPacket                      = detail::get_call("02 00 00 00 e8 ? ? ? ? 90 48 8d 4c 24 50", 4);

	MAKEHOOK(App_GetVersion);
	MAKEHOOK(BaseApp_SetFPSLimit);
	MAKEHOOK(LogMsg);
	MAKEHOOK(NetAvatar_CanMessageT4);
	MAKEHOOK(CanPunchOrBuildNow);
	MAKEHOOK(ObjectMap_HandlePacket);
	MAKEHOOK(SendPacketRaw);
	MAKEHOOK(HandleTouch);
    MAKEHOOK(WorldCamera_OnUpdate);
    MAKEHOOK(UpdateFromNetAvatar);
    MAKEHOOK(SendPacket);

	orig::wndproc = WNDPROC(SetWindowLongPtrW(global::hwnd, -4, LONG_PTR(hooked_wndproc)));

    // clang-format on

    MH_EnableHook(MH_ALL_HOOKS);
    utils::printc("93", "Hooks have been setup!");
}

void hooks::destroy() {
    SetWindowLongPtr(global::hwnd, -4, LONG_PTR(orig::wndproc));
RETRY:
    if (MH_OK != MH_DisableHook(MH_ALL_HOOKS)) {
        auto res = MessageBox(GetForegroundWindow(), L"Hooks could not be reverted", L"Internal", MB_ABORTRETRYIGNORE | MB_ICONERROR | MB_TOPMOST);
        switch (res) {
            case IDABORT:
                while (!TerminateProcess(GetCurrentProcess(), EXIT_SUCCESS))
                    ;
            case IDRETRY: goto RETRY;
            case IDIGNORE: // just ignore
                break;
        }
    }
}

float __cdecl hooks::App_GetVersion(App* app) {
    if (!global::app) {
        global::app = app;
        orig::BaseApp_SetFPSLimit((BaseApp*)app, opt::fps_limit);
        utils::printc("93", "Modified FPS limit!");
    };
    float version = opt::gt_version;
    static float real_ver = orig::App_GetVersion(app);
    if (real_ver > version) //dont spoof if we are running newer client. internal might just be outdated, avoid recompilation.
        version = real_ver;
    else
        utils::printc("93", "spoofed version: %.3f", version);

    return version;
}

void __cdecl hooks::BaseApp_SetFPSLimit(BaseApp* ba, float fps) {
    utils::printc("93", "[INZERNAL]\trequest to set fps to %.0f, setting to %.0f instead", fps, opt::fps_limit);
    orig::BaseApp_SetFPSLimit(ba, opt::fps_limit);
}

int __cdecl hooks::LogMsg(const char* msg, ...) {
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

bool __cdecl hooks::NetAvatar_CanMessageT4(NetAvatar* player) {
    printf("CanMessageT4 called!!!\n");
    return false;
}

bool active = false;
LRESULT __stdcall hooks::hooked_wndproc(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam) {
    active = GetActiveWindow() == global::hwnd;

    //TODO: with imgui
    //if (orig::wndproc(wnd, msg, wparam, lparam))
    //	return true;

    if (msg == WM_KEYDOWN && (wparam == VK_CONTROL || wparam == VK_LCONTROL || wparam == VK_RCONTROL))
        return true;

    if (msg == WM_KEYUP && wparam == VK_F3)
        global::unload = true;

    return CallWindowProcW(hooks::orig::wndproc, wnd, msg, wparam, lparam);
}
bool __cdecl hooks::CanPunchOrBuildNow(AvatarRenderData* render_data) {
    if (opt::cheat::punch_cooldown_on) {
        static auto time_before = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_sec = std::chrono::system_clock::now() - time_before;
        if (elapsed_sec.count() > opt::cheat::punch_cooldown_val) {
            time_before = std::chrono::system_clock::now();
            return true;
        }
        else
            return false;
    }

    return orig::CanPunchOrBuildNow(render_data);
}

bool __cdecl hooks::ObjectMap_HandlePacket(WorldObjectMap* map, GameUpdatePacket* packet) {
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

    return orig::ObjectMap_HandlePacket(map, packet);
}

void __cdecl hooks::SendPacketRaw(int type, GameUpdatePacket* packet, int size, void* ext, EnetPeer* peer, int flag) {
    SendPacketRawHook::Execute(orig::SendPacketRaw, type, packet, size, ext, peer, flag);
}

void __cdecl hooks::HandleTouch(LevelTouchComponent* touch, CL_Vec2f pos, bool started) {
    if (opt::tp_click && GetAsyncKeyState(VK_CONTROL)) {
        //localplayer is guaranteed to be a valid pointer here according to xrefs
        auto local = sdk::gamelogic()->GetLocalPlayer();
        auto new_pos = pos - (local->get_size() / 2.f);
        local->set_pos(new_pos);
        pos = new_pos;
    }
    else
        orig::HandleTouch(touch, pos, started);
}

void __cdecl hooks::WorldCamera_OnUpdate(WorldCamera* camera, CL_Vec2f unk, CL_Vec2f unk2) {
    if (opt::tp_click && GetAsyncKeyState(VK_CONTROL)) //if we dont do this then there is major sliding when teleporting.
        return;

    orig::WorldCamera_OnUpdate(camera, unk, unk2);
}

//for future usage
void __cdecl hooks::UpdateFromNetAvatar(AvatarRenderData* render_data, NetAvatar* player) {
    orig::UpdateFromNetAvatar(render_data, player);
}

void __cdecl hooks::SendPacket(int type, std::string packet, EnetPeer* peer) {
    SendPacketHook::Execute(orig::SendPacket, type, packet, peer);
}
