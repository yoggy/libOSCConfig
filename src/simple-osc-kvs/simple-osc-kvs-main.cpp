#include <SDKDDKVer.h>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <stdio.h>

#include "OSCConfig.h"

OSCConfig config;
std::string filename = "config.txt";

BOOL WINAPI handler_routine_(DWORD dwCtrlType)
{
	// see also... http://msdn.microsoft.com/en-us/library/windows/desktop/ms683242
	switch (dwCtrlType) {
	case CTRL_C_EVENT:
	case CTRL_BREAK_EVENT:
	case CTRL_CLOSE_EVENT:
	case CTRL_LOGOFF_EVENT:
	case CTRL_SHUTDOWN_EVENT:
		printf("simple-osc-kvs.exe : shutdown...\n");
		printf("simple-osc-kvs.exe : saving...filename=%s\n", filename.c_str());
		printf("\n\n");
		config.save();
		break;
	}
	return FALSE;
}

void change_current_dir_()
{
	char path[MAX_PATH];
	char drive[MAX_PATH];
	char dir[MAX_PATH];
	char filename[MAX_PATH];
	char ext[MAX_PATH];
	
	char current_dir[MAX_PATH];

	HMODULE h = ::GetModuleHandle(NULL);
	::GetModuleFileName(h, path, MAX_PATH);
	_splitpath_s(path, drive, MAX_PATH, dir, MAX_PATH, filename, MAX_PATH, ext, MAX_PATH);
	_makepath_s(current_dir, drive, dir, NULL, NULL);

	::SetCurrentDirectory(current_dir);
}

int main(int argc, char* argv[])
{
	change_current_dir_();
	::SetConsoleCtrlHandler(handler_routine_, TRUE);

	config.debug_mode(true);
	config.filename(filename);
	config.load();
	printf("simple-osc-kvs.exe : loading...filename=%s\n", filename.c_str());

	unsigned short port = 7000;
	if (argc == 2) {
		port = (unsigned short)atoi(argv[1]);
	}

	printf("simple-osc-kvs.exe : start listening for input on port %d\npress ctrl-c to end.\n", port);

	bool rv = config.start_kvs_mode(port);
	if (rv == false) {
		printf("error : config.starg_kvs_mode() failed...\n");
		return -1;
	}
	return 0;
}

