#include "stdafx.h"

#include "Config.h"
#include "OSCThread.h"

OSCThread::OSCThread(Config *cf) : port_(0), cf_(cf), break_flag_(false), thread_(nullptr), osc_udp_socket_(nullptr)
{
}

OSCThread::~OSCThread()
{
	if (is_open()) close();
}

bool OSCThread::is_open()
{
	if (osc_udp_socket_) true;
	return false;
}

bool OSCThread::open(const unsigned short &port)
{
	try {
		osc_udp_socket_ = new UdpListeningReceiveSocket(IpEndpointName(IpEndpointName::ANY_ADDRESS, port), this);
	}
	catch (osc::Exception& e) {
		printf("OSCThread::open() : osc::Exception!!! e.what()=%s", e.what());
		return false;
	}
	catch (...) {
		printf("OSCThread::open() : Exception!!! ");
		return false;
	}

	thread_ = new boost::thread(&OSCThread::run, this);
	port_ = port;

	return true;
}

void OSCThread::run()
{
	osc_udp_socket_->Run();
}

void OSCThread::close()
{
	osc_udp_socket_->AsynchronousBreak();
	thread_->join();

	delete thread_;
	thread_ = nullptr;

	delete osc_udp_socket_;
	osc_udp_socket_ = nullptr;
}

void OSCThread::ProcessMessage(const osc::ReceivedMessage& msg, const IpEndpointName& remoteEndpoint)
{
	try{
		std::string key = std::string(msg.AddressPattern());
		osc::ReceivedMessage::const_iterator arg = msg.ArgumentsBegin();
		
		if (arg->IsBool()) {
			bool value = arg->AsBool();
			cf_->set_bool(key, value);
		}
		else if (arg->IsInt32()) {
			osc::int32 value = arg->AsInt32();
			cf_->set_int(key, value);
		}
		else if (arg->IsFloat()) {
			float value = arg->AsFloat();
			cf_->set_float(key, value);
		}
		else if (arg->IsString()) {
			const char * value = arg->AsString();
			cf_->set_string(key, value);
		}
	}
	catch (osc::Exception& e) {
		printf("OSCThread::ProcessMessage() : osc::Exception!!!...e.what()=\n", e.what());
	}
}