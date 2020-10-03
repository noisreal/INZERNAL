#pragma once
#include <windows.h>
#include <stdio.h>
#include <string>
#include <random>
#include <proton/RTFileFormat.h>
#include <core/globals.h>
#include <core/miniz/miniz.h>
#include <sstream>

namespace utils {
	void attach_console();
	void detach_console();
	void printc(std::string color, const char* fmt, ...);
	char read_key();
	uint8_t* read_file(const std::string& file, uint32_t& size);

	template<typename t>
	bool patch_ptr(uintptr_t addr, t data) {
		void* address = reinterpret_cast<void*>(addr);
		DWORD old = 0;
		if (!VirtualProtect(address, sizeof(t), PAGE_EXECUTE_READWRITE, &old))
			return false;
		memcpy(address, (const void*)&data, sizeof(t));
		return VirtualProtect(address, sizeof(t), old, &old);
	}
	template<int size>
	bool patch_bytes(uintptr_t addr, const char* data) {
		void* address = reinterpret_cast<void*>(addr);
		DWORD old = 0;
		if (!VirtualProtect(address, size, PAGE_EXECUTE_READWRITE, &old))
			return false;
		memcpy(address, (const void*)data, size);
		return VirtualProtect(address, size, old, &old);
	}
	template<typename t>
	t& read(uintptr_t addr, uintptr_t offs) {
		return *(t*)(addr + offs);
	}

	template<typename t = uintptr_t>
	t find_pattern(const char* pat, bool notify = true) {
		static auto pat2byt = [](const char* pat) {
			std::vector<int> bytes{};
			auto start = (char*)pat;
			auto end = start + strlen(pat);

			for (auto curr = start; curr < end; ++curr) {
				if (*curr == '?') {
					++curr;
					if (*curr == '?')
						++curr;
					bytes.push_back(-1);
				}
				else bytes.push_back(strtoul(curr, &curr, 16));
			}
			return bytes;
		};

		auto hmod = global::gt;
		auto nthdrs = PIMAGE_NT_HEADERS(uintptr_t(hmod) + PIMAGE_DOS_HEADER(hmod)->e_lfanew);
		auto opthdr = &nthdrs->OptionalHeader;
		auto start = uintptr_t(hmod) + opthdr->BaseOfCode;
		auto end = opthdr->SizeOfCode;
		auto bytes = pat2byt(pat);
		size_t patsize = bytes.size();
		auto data = bytes.data();
		auto found = false;
		if (!start || !end || patsize < 1)
			return t(0x0);
		for (size_t i = start; i <= start + end; i++) {
			for (size_t j = 0; j < patsize; j++) {
				if (*((PBYTE)i + j) == data[j] || data[j] == -1)
					found = true;
				else {
					found = false;
					break;
				}
			}
			if (found)
				return t(i);
		}
		if (notify)
			printf("a pattern has not been found: %s\n", pat);
		return t(0);
	}

	template<typename t = uintptr_t>
	t find_pattern_mod(void* hmod, const char* pat) {
		static auto pat2byt = [](const char* pat) {
			std::vector<int> bytes{};
			auto start = (char*)pat;
			auto end = start + strlen(pat);

			for (auto curr = start; curr < end; ++curr) {
				if (*curr == '?') {
					++curr;
					if (*curr == '?')
						++curr;
					bytes.push_back(-1);
				}
				else bytes.push_back(strtoul(curr, &curr, 16));
			}
			return bytes;
		};

		auto nthdrs = PIMAGE_NT_HEADERS(uintptr_t(hmod) + PIMAGE_DOS_HEADER(hmod)->e_lfanew);
		auto opthdr = &nthdrs->OptionalHeader;
		auto start = uintptr_t(hmod) + opthdr->BaseOfCode;
		auto end = opthdr->SizeOfCode;
		auto bytes = pat2byt(pat);
		size_t patsize = bytes.size();
		auto data = bytes.data();
		auto found = false;
		if (!start || !end || patsize < 1)
			return t(0x0);
		for (size_t i = start; i <= start + end; i++) {
			for (size_t j = 0; j < patsize; j++) {
				if (*((PBYTE)i + j) == data[j] || data[j] == -1)
					found = true;
				else {
					found = false;
					break;
				}
			}
			if (found)
				return t(i);
		}
		printf("a pattern has not been found: %s\n", pat);
		return t(0);
	}

	int random(int min, int max);
	void seed_random();

	std::string hex_str(unsigned char data);
	std::string between(const std::string& s, const std::string& start_delim, const std::string& stop_delim);
	bool replace(std::string& str, const std::string& from, const std::string& to);
	std::string rnd(size_t length);
	float random_float(float a, float b);

	//advanced pattern finding functions that I made for sustaining patterns over 10-40 versions / updates
	uintptr_t find_func_start(uintptr_t addr);
	uintptr_t find_func_start(const char* pattern);
	uintptr_t find_func_end(uintptr_t addr);
	uintptr_t find_pattern_in_area(uintptr_t start, uintptr_t end, const char* pat);
    uintptr_t find_func_has(std::vector<const char*> to_be_found, uint32_t max_length = 0);


	bool dir_exists(LPCSTR name);
	std::string format(const char* msg, ...);
	void unprotect_process();
}

//ported from private internal, code by atipls:
namespace detail {
	template<typename t = void* (__cdecl*)()>
	__forceinline t get_call(uintptr_t call) {
		if (call < 15) {
			printf("[failsafe] get_call got too small address.\n");
			return t();
		}
		auto offset = *(int32_t*)(call + 1);
		return t(call + offset + 0x5);
	}
	template<typename t = uintptr_t>
	__forceinline t get_call(const char* pattern, int offset = 0) {
		return detail::get_call<t>(utils::find_pattern(pattern) + offset);
	}
	template<typename t = void* (__cdecl*)()>
	__forceinline t resolve(const char* sig, size_t offset = 0) {
		static auto ret = get_call<t>(sig, offset);
		return ret;
	}
	template<typename t = void>
	__forceinline t* get(const char* sig, size_t offset = 0) {
		return resolve<t* (__cdecl*)()>(sig, offset)(); //making this into static fucks camera, tilemanager, etc due to them not getting updated again in other worlds
	}
}

