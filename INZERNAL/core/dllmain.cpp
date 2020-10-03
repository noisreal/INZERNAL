#pragma once
#include <process.h>
#include <Windows.h>
#include <core/globals.h>
#include <core/minhook/hook.h>

bool unload_done = false;
void on_exit() {
	if (unload_done) //since this function can be called by DLL_PROCESS_DETACH and unload loop, we only want to do this once.
		return;

	unload_done = true;
	FreeLibraryAndExitThread(HMODULE(global::self), 1);
}

void on_inject() {
	MH_Initialize();

	SetConsoleTitleA("[INZERNAL]");


	on_exit();
}

BOOL WINAPI DllMain(HINSTANCE dll, DWORD reason, LPVOID reserved)
{
	if (reason == DLL_PROCESS_ATTACH) {
		global::self = HMODULE(dll);
		DisableThreadLibraryCalls(HMODULE(dll));
		CreateThread(nullptr, 0, LPTHREAD_START_ROUTINE(on_inject), nullptr, 0, nullptr);
		return 1;
	}
	else if (reason == DLL_PROCESS_DETACH) {
		on_exit();
	}
	return 0;
}
