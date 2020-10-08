#pragma once
#include <core/globals.h>
#include <sdk/sdk.h>
#include <string>
#include <proton/RTParam.hpp>
#include <core/gt.h>
#include <proton\MiscUtils.h>

class SendPacketHook {
   public:
    static void Execute(types::SendPacket orig, int type, std::string packet, EnetPeer* peer) {
      
        if (type == NET_MESSAGE_CLIENT_LOG_RESPONSE) //dont send crash log to gt.
            return;

        if (packet.find("game_version|") != -1 && opt::spoof_login) {
            auto var = RTVar::parse(packet);
            auto mac = gt::generate_mac();
            auto hash_str = mac + "RT";
            auto hash2 = HashString(hash_str.c_str(), hash_str.length());

            var.set("mac", mac);
            var.set("wk", gt::generate_rid());
            var.set("rid", gt::generate_rid());
            var.set("fz", std::to_string(utils::random(INT_MIN, INT_MAX)));
            var.set("zf", std::to_string(utils::random(INT_MIN, INT_MAX)));
            var.set("hash", std::to_string(utils::random(INT_MIN, INT_MAX)));
            var.set("hash2", std::to_string(hash2));
            var.set("meta", gt::generate_meta());

            //never really paid attention to this before but this indeed could be used to track too
            var.set("player_age", std::to_string(utils::random(21, 60))); 

            if (opt::spoof_name)
                var.set("requestedName", utils::rnd(utils::random(4, 10)));
          

            packet = var.serialize();
        }
     

        orig(type, packet, peer);
    }
};