#include <sdk/sdk.h>

GameLogic* sdk::gamelogic()
{
	return detail::get<GameLogic>("E8 ? ? ? ? 8b 17 ? 8d");
}

uintptr_t* sdk::enetclient()
{

	static auto client = detail::get_call<uintptr_t* (__cdecl*)()>("E8 ? ? ? ? 45 ? ? 89 ? ? ? 48 8D ? ? 48", 0);
	return client();

}

//been this way for over 20 versions, probably will not get updated as its in enetclient.
EnetPeer* sdk::enetpeer() {
    return (EnetPeer*)((uintptr_t)enetclient() + 200);
}
