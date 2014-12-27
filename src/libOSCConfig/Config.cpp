#include "stdafx.h"

#include <map>
#include <string>
#include <iostream>

#include <boost/shared_ptr.hpp>
#include <boost/lexical_cast.hpp>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include <boost/optional.hpp>

#include <boost/thread.hpp>

#include "Config.h"

Config::Config(void)
{

}

Config::~Config(void)
{

}

void Config::clear()
{
	map_.clear();
}

void Config::write_pt_(boost::property_tree::ptree &pt)
{
	typedef std::map<std::string, std::string>::const_reference type;
	BOOST_FOREACH(type p, map_) {
		pt.put(p.first, p.second);
	}
}

bool Config::save(const std::string &filename) {
	boost::mutex::scoped_lock scl(mutex_);

	boost::property_tree::ptree pt;
	write_pt_(pt);

	try {
		boost::property_tree::json_parser::write_json(filename, pt);
	}
	catch(...) {
		std::cout << "error : save() : write_json() failed...filename=" << filename.c_str() << std::endl;
		return false;
	}

	return true;
}

void Config::read_pt_(boost::property_tree::ptree &pt)
{
	BOOST_FOREACH(const boost::property_tree::ptree::value_type& child, pt) {
		std::string key = child.first;
		std::string val = pt.get<std::string>(key);
		set_string(key, val);
	}
}

bool Config::load(const std::string &filename) {
	boost::mutex::scoped_lock scl(mutex_);

	clear();
	boost::property_tree::ptree pt;

	try {
		boost::property_tree::json_parser::read_json(filename, pt);
	}
	catch(...) {
		std::cout << "error : load() : read_json() failed...filename=" << filename.c_str() << std::endl;
		return false;
	}

	read_pt_(pt);

	return true;
}

bool Config::has_key_(const std::string &key)
{
	if (map_.find(key) == map_.end()) return false;
	return true;
}

bool Config::has_key(const std::string &key) {
	boost::mutex::scoped_lock scl(mutex_);
	return has_key_(key);
}

bool Config::get_bool(const std::string &key, const bool &default_val) {
	std::string val_str = get_string(key, boost::lexical_cast<std::string>(default_val));
	return boost::lexical_cast<bool>(val_str);
}

void Config::set_bool(const std::string &key, const bool &value) {
	set_string(key, boost::lexical_cast<std::string>(value));
}

int Config::get_int(const std::string &key, const int &default_val) {
	std::string val_str = get_string(key, boost::lexical_cast<std::string>(default_val));
	return boost::lexical_cast<int>(val_str);
}

void Config::set_int(const std::string &key, const int &value) {
	set_string(key, boost::lexical_cast<std::string>(value));
}

float Config::get_float(const std::string &key, const float &default_val) {
	std::string val_str = get_string(key, boost::lexical_cast<std::string>(default_val));
	return boost::lexical_cast<float>(val_str);
}

void Config::set_float(const std::string &key, const float &value) {
	set_string(key, boost::lexical_cast<std::string>(value));
}

std::string Config::get_string(const std::string &key, const std::string &default_val) {
	if (has_key_(key) == false) return default_val;
	boost::mutex::scoped_lock scl(mutex_);

	return map_[key];
}

void Config::set_string(const std::string &key, const std::string &value) {
	boost::mutex::scoped_lock scl(mutex_);

	map_[key] = value;
}
