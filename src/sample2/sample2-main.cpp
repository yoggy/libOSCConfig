#include <SDKDDKVer.h>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <stdio.h>

#include "OSCConfig.h"

void main()
{
	OSCConfig config;
	config.start_kvs_mode(12345);
}

