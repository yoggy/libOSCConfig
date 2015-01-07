#include "stdafx.h"
#include "Config.h"
#include "OscConfigImpl.h"

OSCConfigImpl::OSCConfigImpl() : osc_thread_(nullptr), debug_mode_(false)
{

}

OSCConfigImpl::~OSCConfigImpl()
{

}

std::string OSCConfigImpl::filename()
{
	return cf_.filename();
}

void OSCConfigImpl::filename(const std::string &val)
{
	cf_.filename(val);
}

bool OSCConfigImpl::debug_mode() const
{
	return debug_mode_;
}

void OSCConfigImpl::debug_mode(const bool &val)
{
	if (osc_thread_) {
		osc_thread_->debug_mode(val);
	}
	debug_mode_ = val;
}

bool OSCConfigImpl::start(const unsigned short &port)
{
	if (osc_thread_) return false;

	osc_thread_ = new OSCThread(&cf_);
	osc_thread_->debug_mode(debug_mode_);

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

	osc_thread_ = new OSCThread(&cf_);
	osc_thread_->debug_mode(debug_mode_);

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

bool OSCConfigImpl::save()
{
	return cf_.save();
}

bool OSCConfigImpl::load()
{
	return cf_.load();
}

void OSCConfigImpl::clear()
{
	cf_.clear();
}

bool OSCConfigImpl::has_key(const std::string &key)
{
	return cf_.has_key(key);
}

void OSCConfigImpl::remove(const std::string &key)
{
	cf_.remove(key);
}

bool OSCConfigImpl::get_bool(const std::string &key, const bool &default_value)
{
	return cf_.get_bool(key, default_value);
}

void OSCConfigImpl::set_bool(const std::string &key, const bool &value)
{
	cf_.set_bool(key, value);
}

int OSCConfigImpl::get_int(const std::string &key, const int &default_value)
{
	return cf_.get_int(key, default_value);
}

void OSCConfigImpl::set_int(const std::string &key, const int &value)
{
	return cf_.set_int(key, value);
}

float OSCConfigImpl::get_float(const std::string &key, const float &default_value)
{
	return cf_.get_float(key, default_value);
}

void OSCConfigImpl::set_float(const std::string &key, const float &value)
{
	cf_.set_float(key, value);
}

std::string OSCConfigImpl::get_string(const std::string &key, const std::string &default_value)
{
	return cf_.get_string(key, default_value);
}

void OSCConfigImpl::set_string(const std::string &key, const std::string &value)
{
	cf_.set_string(key, value);
}
