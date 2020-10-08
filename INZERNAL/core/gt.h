#pragma once
#include <core/utils.h>
#include <proton/clanlib/vec2.h>
#include <string>

namespace gt {
    //i mean tell me a simpler way to do this
    std::string generate_mac(const std::string& prefix = "02");
    std::string generate_rid();
    std::string generate_meta();
    std::string get_random_flag();
    void sendpacket(int type, std::string message);
    void ghetto_fix();
    bool patch_banbypass();
    void join_world(std::string world);
    void show_message(std::string message, int time, int delay);
    void log(std::string msg);

    //void screen_size(CL_Vec2f& rect);
    //void hit_tile(CL_Vec2i where);
    //void place_tile(int id, CL_Vec2i where);
    //void wrench_tile(CL_Vec2i where);
    //void water_tile(CL_Vec2i where);
    //void tutorial_bypass();
    //void calc_checksum(iteminfo_t& item);
} // namespace gt