#pragma once

#include <boost/thread.hpp>
#include "Config.h"
#include "OSCThread.h"

class OSCConfigImpl
{
public:
	OSCConfigImpl();
	virtual ~OSCConfigImpl();

	bool start(const unsigned short &port);
	bool start_kvs_mode(const unsigned short &port);
	void stop();

	bool save(const std::string &filename);
	bool load(const std::string &filename);

	void clear();

	bool has_key(const std::string &key);

	bool get_bool(const std::string &key, const bool &default_value);
	void set_bool(const std::string &key, const bool &value);

	int get_int(const std::string &key, const int &default_valu);
	void set_int(const std::string &key, const int &value);

	float get_float(const std::string &key, const float &default_value);
	void set_float(const std::string &key, const float &value);

	std::string get_string(const std::string &key, const std::string &default_value);
	void set_string(const std::string &key, const std::string &value);

protected:
	Config config_;
	OSCThread *osc_thread_;
};

