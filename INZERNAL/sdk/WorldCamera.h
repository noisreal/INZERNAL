#pragma once
#include <proton/clanlib/vec2.h>
#include <core/gt.h>
#include <core/utils.h>

class WorldCamera {
   public:
    CL_Vec2f WorldToScreen(CL_Vec2f& in) {
        CL_Vec2f out;
        static auto func = types::WorldToScreen(detail::get_call("00 e8 ? ? ? ? 49 8b ? ? 41 ? 00 04", 1));
        func(this, out, in);
        return out;
    }

    //outdated offset, from private internal.
   /* CL_Vec2f screen_to_world(CL_Vec2f& in) {
        CL_Vec2f out;
        static auto screentoworld = screen2world_t(utils::find_func_start("f3 0f 5e 49 20 f3 41 0f 59 08"));
        screentoworld(this, out, in);
        return out;
    }*/
};
