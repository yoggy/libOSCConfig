#include <SDKDDKVer.h>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <stdio.h>

#include "OSCConfig.h"

int main(int argc, char* argv[])
{
	OSCConfig config;

	config.start(12345);

	while (true) {
		bool  b = config.get_bool("/test/bool");
		int   i = config.get_int("/test/int");
		float f = config.get_float("/test/float");
		std::string s = config.get_string("/test/string");

		printf("b=%s, i=%d, f=%f, s=\"%s\"\n", b ? "true" : "false", i, f, s.c_str());
		::Sleep(500);
	}

	return 0;
}

