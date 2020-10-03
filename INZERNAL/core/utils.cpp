#pragma once
#include <core/utils.h>
#include <algorithm>
#include <time.h>
#include <chrono>
#include <aclapi.h>
#include <io.h>
#include <fcntl.h>
#include <iostream>
#include <core/gt.h>

HANDLE _out = NULL, _old_out = NULL;
HANDLE _err = NULL, _old_err = NULL;
HANDLE _in = NULL, _old_in = NULL;
FILE* _out_b = nullptr;
int _out_h = 0;

typedef std::mt19937 rng_type;
rng_type rng;

void utils::attach_console()
{
	//set up utils::print to work using WriteConsoleA
	_old_out = GetStdHandle(STD_OUTPUT_HANDLE);
	_old_err = GetStdHandle(STD_ERROR_HANDLE);
	_old_in = GetStdHandle(STD_INPUT_HANDLE);
	::AllocConsole() && ::AttachConsole(GetCurrentProcessId());
	_out = GetStdHandle(STD_OUTPUT_HANDLE);
	_err = GetStdHandle(STD_ERROR_HANDLE);
	_in = GetStdHandle(STD_INPUT_HANDLE);

	SetConsoleMode(_out,
		ENABLE_PROCESSED_OUTPUT | ENABLE_WRAP_AT_EOL_OUTPUT | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
	SetConsoleMode(_in,
		ENABLE_INSERT_MODE | ENABLE_EXTENDED_FLAGS |
		ENABLE_PROCESSED_INPUT | ENABLE_QUICK_EDIT_MODE);

	//setup printf, cout, fwrite to stdout to work
	freopen_s(&_out_b, "nul", "w", stdout);
	auto outfd = _open_osfhandle((intptr_t)_out, _O_TEXT);
	auto outf = _fdopen(outfd, "w");
	if (outf) {
		_out_h = _fileno(stdout);
		_dup2(_fileno(outf), _out_h);
		setvbuf(stdout, NULL, _IONBF, 0);
	}
}

void utils::detach_console() {
	if (_out && _err && _in) {
		FreeConsole();
		if (_out_h != -1 && _out_h != 0)
			_dup2(_fileno(stdout), _out_h);
		
		if (_old_out)
			SetStdHandle(STD_OUTPUT_HANDLE, _old_out);
		if (_old_err)
			SetStdHandle(STD_ERROR_HANDLE, _old_err);
		if (_old_in)
			SetStdHandle(STD_INPUT_HANDLE, _old_in);
	}
}

std::string utils::format(const char* msg, ...) {
	char fmt[1024] = { 0 };
	va_list va;
	va_start(va, msg);
	vsnprintf(fmt, 1024, msg, va);
	va_end(va);
	return std::string(fmt);
}
char utils::read_key() {
	if (!_in)
		return false;
	char key = 0;
	DWORD keysread = 0;

	ReadConsoleA(_in, &key, 1, &keysread, nullptr);
	return key;
}


int utils::random(int min, int max) {
	std::uniform_int_distribution<int> distribution(min, max);
	return distribution(rng);
}
void utils::printc(std::string color, const char* fmt, ...)
{
	if (!_out)
		return;

	char buf[2048];
	int len = -1;
	va_list va;
	va_start(va, fmt);
	if (0 > (len = _vsnprintf_s(buf, sizeof(buf), fmt, va))) {
		va_end(va);
		return;
	}
	va_end(va);
	printf("\x1B[%sm%s\n\033[0m", color.c_str(), buf);
}
void utils::seed_random()
{
	auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
	rng.seed(seed);
	printf("set random seed: %lld\n", seed);
}