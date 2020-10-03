#pragma once
//include everything here so no need to include them individually in other files
#include <sdk/GameLogic.h>
#include <sdk/GameUpdatePacket.h>
#include <sdk/NetAvatar.h>

namespace sdk {
	GameLogic* gamelogic();
	uintptr_t* enetclient();
	uintptr_t* enetpeer();

}