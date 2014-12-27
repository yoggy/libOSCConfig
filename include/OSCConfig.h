#pragma once

#ifdef LIB_OSCCONFIG_API_EXPORTS
#define LIB_OSCCONFIG_API __declspec(dllexport)
#else
#define LIB_OSCCONFIG_API __declspec(dllimport)
#ifdef _DEBUG
#pragma comment(lib, "libOSCConfigD.lib")
#else
#pragma comment(lib, "libOSCConfig.lib")
#endif
#endif

#include <string>

class OSCConfigImpl;

class LIB_OSCCONFIG_API OSCConfig {
public:
	OSCConfig();
	virtual ~OSCConfig();

	bool start(const unsigned short &port);
	void stop();

	bool save(const std::string &filename);
	bool load(const std::string &filename);

	void clear();

	bool has_key(const std::string &key);

	bool get_bool(const std::string &key, const bool &default_value = false);
	void set_bool(const std::string &key, const bool &value);

	int get_int(const std::string &key, const int &default_value = 0);
	void set_int(const std::string &key, const int &value);

	float get_float(const std::string &key, const float &default_value = 0.0f);
	void set_float(const std::string &key, const float &value);

	std::string get_string(const std::string &key, const std::string &default_value = "");
	void set_string(const std::string &key, const std::string &value);

protected:
	OSCConfigImpl *impl_;
};
