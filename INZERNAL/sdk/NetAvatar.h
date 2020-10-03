#pragma once
#include <string>
#include <proton/clanlib/vec2.h>
#include <proton/VariantDB.h>

class NetAvatar {
public:
	void* vtable;
	CL_Vec2f m_pos;
	CL_Vec2f m_size;
	CL_Vec2f m_pos2;
	CL_Vec2f m_size2;
	std::string name;
	int netid;
	char pad1[76];
	VariantDB vardb;

	void set_pos(const CL_Vec2f& n) { set_pos(n.x, n.y); }
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
	void set_size(const CL_Vec2f& n) { set_size(n.x, n.y); }
	void set_size(float x, float y) {
		m_size.x = x;
		m_size.y = y;
		m_size2.x = x / 6.5999999;
		m_size2.y = y / 1.3;
	}

	//for local only
	CL_Vec2f get_size() {
		//returning the encrypted one cuz its 100% what the server has
		return CL_Vec2f(m_size2.x * 6.5999999f, m_size2.y * 1.3f);
	}
};