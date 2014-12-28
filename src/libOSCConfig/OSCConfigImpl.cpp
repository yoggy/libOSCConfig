#include "stdafx.h"
#include "Config.h"
#include "OscConfigImpl.h"

OSCConfigImpl::OSCConfigImpl() : osc_thread_(nullptr)
{

}

OSCConfigImpl::~OSCConfigImpl()
{

}

bool OSCConfigImpl::start(const unsigned short &port)
{
	if (osc_thread_) return false;
	osc_thread_ = new OSCThread(&config_);
	bool rv = osc_thread_->start(port);
	if (rv == false) {
		delete osc_thread_;
		return false;
	}

	return true;
}

bool OSCConfigImpl::start_kvs_mode(const unsigned short &port)
{
	if (osc_thread_) return false;
	osc_thread_ = new OSCThread(&config_);
	bool rv = osc_thread_->start_kvs_mode(port);

	delete osc_thread_;
	return rv;
}

void OSCConfigImpl::stop()
{
	if (osc_thread_) {
		osc_thread_->stop();
		delete osc_thread_;
		osc_thread_ = nullptr;
	}
}

bool OSCConfigImpl::save(const std::string &filename)
{
	return config_.save(filename);
}

bool OSCConfigImpl::load(const std::string &filename)
{
	return config_.load(filename);
}

void OSCConfigImpl::clear()
{
	config_.clear();
}

bool OSCConfigImpl::has_key(const std::string &key)
{
	return config_.has_key(key);
}

bool OSCConfigImpl::get_bool(const std::string &key, const bool &default_value)
{
	return config_.get_bool(key, default_value);
}

void OSCConfigImpl::set_bool(const std::string &key, const bool &value)
{
	config_.set_bool(key, value);
}

int OSCConfigImpl::get_int(const std::string &key, const int &default_value)
{
	return config_.get_int(key, default_value);
}

void OSCConfigImpl::set_int(const std::string &key, const int &value)
{
	return config_.set_int(key, value);
}

float OSCConfigImpl::get_float(const std::string &key, const float &default_value)
{
	return config_.get_float(key, default_value);
}

void OSCConfigImpl::set_float(const std::string &key, const float &value)
{
	config_.set_float(key, value);
}

std::string OSCConfigImpl::get_string(const std::string &key, const std::string &default_value)
{
	return config_.get_string(key, default_value);
}

void OSCConfigImpl::set_string(const std::string &key, const std::string &value)
{
	config_.set_string(key, value);
}
