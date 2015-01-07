#pragma once

#include <string>
#include <sstream>
#include <map>
#include <boost/shared_ptr.hpp>
#include <boost/property_tree/ptree.hpp>

#include <boost/thread.hpp>

class Config
{
public:
	Config();
	virtual ~Config(void);

	std::string filename() const;
	void filename(const std::string &val);

	void clear();

	bool save();
	bool load();

	bool has_key(const std::string &key);

	void remove(const std::string &key);

	bool get_bool(const std::string &key, const bool &default_val = false);
	void set_bool(const std::string &key, const bool &value); 

	int get_int(const std::string &key, const int &default_val = 0);
	void set_int(const std::string &key, const int &value);

	float get_float(const std::string &key, const float &default_val = 0.0f);
	void set_float(const std::string &key, const float &value);

	std::string get_string(const std::string &key, const std::string &default_val = "");
	void set_string(const std::string &key, const std::string &value);
	
protected:
	void Config::write_pt_(boost::property_tree::ptree &pt);
	void Config::read_pt_(boost::property_tree::ptree &pt);
	bool has_key_(const std::string &key);

protected:
	std::string filename_;
	std::map<std::string, std::string> map_;
	boost::mutex mutex_;
};

