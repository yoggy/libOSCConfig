#include "stdafx.h"
#include "OSCConfigImpl.h"

#define LIB_OSCCONFIG_API_EXPORTS
#include "OSCConfig.h"

LIB_OSCCONFIG_API OSCConfig::OSCConfig()
{
	impl_ = new OSCConfigImpl();
}

LIB_OSCCONFIG_API OSCConfig::~OSCConfig()
{
	if (impl_ != nullptr) {
		delete impl_;
		impl_ = nullptr;
	}
}

LIB_OSCCONFIG_API std::string OSCConfig::filename()
{
	return impl_->filename();
}

LIB_OSCCONFIG_API void OSCConfig::filename(const std::string &val)
{
	impl_->filename(val);
}

LIB_OSCCONFIG_API bool OSCConfig::debug_mode() const
{
	return impl_->debug_mode();
}

LIB_OSCCONFIG_API void OSCConfig::debug_mode(const bool &val)
{
	impl_->debug_mode(val);
}

LIB_OSCCONFIG_API bool OSCConfig::start(const unsigned short &port)
{
	return impl_->start(port);
}

LIB_OSCCONFIG_API bool OSCConfig::start_kvs_mode(const unsigned short &port)
{
	return impl_->start_kvs_mode(port);
}

LIB_OSCCONFIG_API void OSCConfig::stop()
{
	impl_->stop();
}

LIB_OSCCONFIG_API bool OSCConfig::save()
{
	return impl_->save();
}

LIB_OSCCONFIG_API bool OSCConfig::load()
{
	return impl_->load();
}

LIB_OSCCONFIG_API void OSCConfig::clear()
{
	impl_->clear();
}

LIB_OSCCONFIG_API bool OSCConfig::has_key(const std::string &key)
{
	return impl_->has_key(key);
}

LIB_OSCCONFIG_API void OSCConfig::remove(const std::string &key)
{
	impl_->remove(key);
}

LIB_OSCCONFIG_API bool OSCConfig::get_bool(const std::string &key, const bool &default_value)
{
	return impl_->get_bool(key, default_value);
}

LIB_OSCCONFIG_API void OSCConfig::set_bool(const std::string &key, const bool &value)
{
	impl_->set_bool(key, value);
}

LIB_OSCCONFIG_API int OSCConfig::get_int(const std::string &key, const int &default_value)
{
	return impl_->get_int(key, default_value);
}

LIB_OSCCONFIG_API void OSCConfig::set_int(const std::string &key, const int &value)
{
	impl_->set_int(key, value);
}

LIB_OSCCONFIG_API float OSCConfig::get_float(const std::string &key, const float &default_value)
{
	return impl_->get_float(key, default_value);
}

LIB_OSCCONFIG_API void OSCConfig::set_float(const std::string &key, const float &value)
{
	impl_->set_float(key, value);
}

LIB_OSCCONFIG_API std::string OSCConfig::get_string(const std::string &key, const std::string &default_value)
{
	return impl_->get_string(key, default_value);
}

LIB_OSCCONFIG_API void OSCConfig::set_string(const std::string &key, const std::string &value)
{
	impl_->set_string(key, value);
}
