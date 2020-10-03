#pragma once
#include <sdk/NetAvatar.h>
#include <core/utils.h>

class GameLogic {
public:
	NetAvatar* GetLocalPlayer() {
		//	using GetLocalPlayer_T = NetAvatar * (__cdecl*)(GameLogic*);
		//	static auto ptr = GetLocalPlayer_T(utils::find_pattern<GetLocalPlayer_T*>("CC 48 8B 81 ?? ?? ?? ?? C3 CC") + byte(1));
		return *(NetAvatar**)((char*)this + 448);
		//TODO: better method of getting this !
	}
};