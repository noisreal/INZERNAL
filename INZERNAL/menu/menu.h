#pragma once
#include <core/globals.h>

//TODO: setup config

namespace menu {

	void main_tab();

	void endscene(IDirect3DDevice9* device, bool active);
	bool wndproc(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam);
}