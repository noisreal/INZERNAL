#pragma once
#include <core/utils.h>
#include <menu/menu.h>

void menu::EndScene(IDirect3DDevice9* device, bool active) {
    if (!global::load) { //init imgui
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        ImGui_ImplDX9_Init(device);

        ImGui::StyleColorsDark();
        //load_config();
        ImGui::GetStyle().ItemSpacing = ImVec2(5.5, 5.5);

        utils::printc("93", "dx9 and imgui init done");
        global::load = true;
    }
    if (global::load) {
        ImGui_ImplDX9_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        ImGui::SetNextWindowPos(ImVec2{ 0, 0 }, ImGuiCond_Once);
        ImGui::SetNextWindowSize(ImVec2{ 600, 400 }, ImGuiCond_Once);

        if (global::draw && ImGui::Begin(std::string("INZERNAL " + global::version).c_str(), &global::draw, ImGuiWindowFlags_NoCollapse)) {
            if (ImGui::BeginTabBar("##tabs", ImGuiTabBarFlags_None)) {
                if (ImGui::BeginTabItem("Main")) {
                    main_tab();
                    ImGui::EndTabItem();
                }
            }

            ImGui::End();
        }
        ImGui::EndFrame();
        ImGui::Render();
        ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
    }
}

bool menu::WndProc(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam) {
    static bool init = false;
    if (global::load && !init) {
        ImGui_ImplWin32_Init(wnd);
        init = true;
    }

    if (msg == WM_KEYUP && wparam == VK_F2)
        global::draw = !global::draw;

    /*  if (msg == WM_KEYUP && wparam == VK_F4)
        load_config();*/

    return ImGui_ImplWin32_WndProcHandler(wnd, msg, wparam, lparam);
}
