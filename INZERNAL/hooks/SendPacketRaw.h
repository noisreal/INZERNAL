#pragma once
#include <core/globals.h>
#include <sdk/GameUpdatePacket.h>
#include <string>

class SendPacketRaw {
   public:
    static void Hook(types::SendPacketRaw orig, int type, GameUpdatePacket* packet, int size, void* ext, EnetPeer* peer, int flag) {
     
        printf("sending raw packet with type %d\n", packet->type);
        if (packet->type == PACKET_APP_INTEGRITY_FAIL)
            return;

        if (packet->velocity_x == 1000.f || packet->type == PACKET_PING_REPLY || packet->type == PACKET_PING_REQUEST) {
            utils::printc("95", "[ping check] sending emulated response so we dont get dc or ban");
            GameUpdatePacket pk{ 0 };
            pk.type = PACKET_PING_REPLY;
            pk.item = packet->item; // tick count (speedhack)
            printf("tick of packet: %d, GetTickCount(): %d\n", packet->item, GetTickCount());
            pk.pos_x = 64.f; // punch range in pixels
            pk.pos_y = 64.f; // build range in pixels
            pk.velocity_x = 1000.f; //gravity
            pk.velocity_y = 250.f;                     // movement speed

            int anybody = 0;
            orig(NET_MESSAGE_GAME_PACKET, &pk, 56, &anybody, sdk::enetpeer(), flag);
            return;
        }

        orig(type, packet, size, ext, peer, flag);
    }
};