#pragma once
#include <core/utils.h>
#include <proton/clanlib/vec2.h>
#include <proton/variant2.hpp>
#include <string>

#pragma pack(push, 1)
struct __declspec(align(1)) alignas(1) NetAvatar { //how fucking annoying it is to get align to work
   public:
    void* vtable;
    CL_Vec2f m_pos;
    CL_Vec2f m_size;
    CL_Vec2f m_pos2;
    std::string name;
    int netid;

    void set_pos(const CL_Vec2f& n) {
        set_pos(n.x, n.y);
    }
    void set_pos(float x, float y) {
        m_pos.x = x;
        m_pos.y = y;
        m_pos2.x = x / 6.5999999;
        m_pos2.y = y / 1.3;
    }
    void set_pos_at_tile(int x, int y) {
        set_pos(x * 32 + 8, y * 32);
    }

    //for local only
    CL_Vec2f get_pos() {
        //returning the encrypted one cuz its 100% what the server has
        return CL_Vec2f(m_pos2.x * 6.5999999f, m_pos2.y * 1.3f);
    }
    void set_size(const CL_Vec2f& n) {
        set_size(n.x, n.y);
    }
    void set_size(float x, float y) {
        m_size.x = x;
        m_size.y = y;
    }

    //for local only
    CL_Vec2f get_size() {
        return m_size;
    }

    void SetModStatus(bool mod, bool supermod) {
        static std::vector<const char*> ondataconfig_patt{ "C7 43 ?? 05 00 00 00 C7 43 ?? 00", "40 0F ?? ?? 83", "0F 95 C0 40" };
        static auto OnDataConfig = types::OnDataConfig(utils::find_func_has(ondataconfig_patt, 250));

        variantlist_t va{};
        va[0] = uint32_t(mod);
        va[1] = uint32_t(supermod);

        OnDataConfig(this, &va);
    }
};
#pragma pack(pop)

//for debugging purposes, feel free to use offsetof to match in CE or IDA
constexpr auto offset = offsetof(NetAvatar, name);