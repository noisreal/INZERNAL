#pragma once
#include <process.h>
#include <Windows.h>
#include <core/globals.h>
#include <core/minhook/hook.h>
#include <core/utils.h>
#include <core/gt.h>
#include <hooks\hooks.h>

bool unload_done = false;
void on_exit() {
	if (unload_done) //since this function can be called by DLL_PROCESS_DETACH and unload loop, we only want to do this once.
		return;

	printf("Cleaning up internal changes....\n");
	hooks::destroy();
	Sleep(300);

	printf("Cleaning done\tDetaching console and exiting process.\n");
	Sleep(300);
	utils::detach_console();

	unload_done = true;
	FreeLibraryAndExitThread(HMODULE(global::self), 1);
}

void on_inject() {
	MH_Initialize();
	utils::attach_console();


	SetConsoleTitleA("[INZERNAL]");
	utils::printc("96", "INZERNAL - internal framework and enhancement suite");
	utils::printc("1;40;31", "\tspecial thanks to ness for patcher");
	utils::printc("92", "\tmade by ama - https://github.com/ama6nen/INZERNAL\n");

	utils::seed_random();
	if (!gt::patch_banbypass())
		on_exit();

	hooks::init();
	while (!global::unload)
		Sleep(10);
	on_exit();
}

//for patcher to work
void dll() { }

BOOL WINAPI DllMain(HINSTANCE dll, DWORD reason, LPVOID reserved)
{
	if (reason == DLL_PROCESS_ATTACH) {
		global::self = HMODULE(dll);
		global::gt = GetModuleHandleW(nullptr);
		DisableThreadLibraryCalls(HMODULE(dll));
		CreateThread(nullptr, 0, LPTHREAD_START_ROUTINE(on_inject), nullptr, 0, nullptr);
		return 1;
	}
	else if (reason == DLL_PROCESS_DETACH) {
		on_exit();
	}
	return 0;
}
