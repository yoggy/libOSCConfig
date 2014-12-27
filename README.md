libOSCConfig
====
libOSCConfig is Simple KVS server using liboscpack.

Usage
====
sample-main.cpp
<pre>
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
</pre>

sendosc.exe
<pre>
> sendosc.exe localhost 12345 /test/bool b true
> sendosc.exe localhost 12345 /test/int i 12345
> sendosc.exe localhost 12345 /test/float 1234.5
> sendosc.exe localhost 12345 /test/string s teststring...
</pre>

Libraries
====
libOSCConfig uses the following libraries.

* liboscpack_1_1_0-vc12
  * https://github.com/yoggy/liboscpack_1_1_0-vc12

* boost 1.56.0
  * http://www.boost.org/
