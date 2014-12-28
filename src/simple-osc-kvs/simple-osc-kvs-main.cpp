#include <SDKDDKVer.h>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <stdio.h>

#include "OSCConfig.h"

int main(int argc, char* argv[])
{
	OSCConfig config;
	config.debug_mode(true);

	unsigned short port = 7000;
	if (argc == 2) {
		port = (unsigned short)atoi(argv[1]);
	}

	printf("simple-osc-kvs.exe : start listening for input on port %d\npress ctrl-c to end.\n", port);

	bool rv = config.start_kvs_mode(port);
	if (rv == false) {
		printf("config.starg_kvs_mode() failed...\n");
		return -1;
	}
	return 0;
}

