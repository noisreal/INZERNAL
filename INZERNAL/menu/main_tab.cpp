#pragma once
#include <menu/menu.h>

void menu::main_tab() {
    ImGui::Text("Menu will be updated in later versions.");
    ImGui::SliderFloat("Punch cooldown", &opt::cheat::punch_cooldown_val, 0.01f, 1.f, "%0.2f");
	
}