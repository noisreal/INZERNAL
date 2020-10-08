#pragma once
#include <core/globals.h>
#include <sdk/sdk.h>
#include <string>

class SendPacketHook {
   public:
    static void Execute(types::SendPacket orig, int type, std::string packet, EnetPeer* peer) {
      
        if (type == NET_MESSAGE_CLIENT_LOG_RESPONSE) //dont send crash log to gt.
            return;

        orig(type, packet, peer);
    }
};