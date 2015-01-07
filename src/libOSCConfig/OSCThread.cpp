#include "stdafx.h"

#include "Config.h"
#include "OSCThread.h"

std::string get_local_time_str_()
{
	boost::posix_time::ptime p = boost::posix_time::microsec_clock::local_time();
	return boost::posix_time::to_iso_extended_string(p);
}

std::string get_address_str_(const unsigned long &addr)
{
	std::stringstream ss;

	int a0 = (addr & 0xff000000) >> 24;
	int a1 = (addr & 0x00ff0000) >> 16;
	int a2 = (addr & 0x0000ff00) >> 8;
	int a3 = (addr & 0x000000ff) >> 0;

	ss << a0 << "." << a1 << "." << a2 << "." << a3;

	return ss.str();
}

OSCThread::OSCThread(Config *cf) 
	: port_(0), cf_(cf), break_flag_(false), debug_mode_(false), 
	thread_(nullptr), osc_recv_socket_(nullptr)
{
}

OSCThread::~OSCThread()
{
	if (is_open()) stop();
}

bool OSCThread::debug_mode() const
{
	return debug_mode_;
}

void OSCThread::debug_mode(const bool &val)
{
	debug_mode_ = val;
}

bool OSCThread::is_open()
{
	if (osc_recv_socket_) true;
	return false;
}

bool OSCThread::open_(const unsigned short &port)
{
	try {
		osc_recv_socket_ = new UdpListeningReceiveSocket(IpEndpointName(IpEndpointName::ANY_ADDRESS, port), this);
	}
	catch (osc::Exception& e) {
		printf("OSCThread::open_() : osc::Exception!!! port %d already in use...e.what()=%s\n", port, e.what());
		return false;
	}
	catch (...) {
		printf("OSCThread::open_() : Exception!!! port %d already in use...\n", port);
		return false;
	}
	return true;
}

bool OSCThread::start(const unsigned short &port)
{
	if (open_(port) == false) return false;

	thread_ = new boost::thread(&OSCThread::run, this);

	return true;
}

bool OSCThread::start_kvs_mode(const unsigned short &port)
{
	if (open_(port) == false) return false;
	run();
	return true;
}

void OSCThread::run()
{
	osc_recv_socket_->Run();
}

void OSCThread::stop()
{
	osc_recv_socket_->AsynchronousBreak();
	thread_->join();

	delete thread_;
	thread_ = nullptr;

	delete osc_recv_socket_;
	osc_recv_socket_ = nullptr;
}

void OSCThread::ProcessPacket(const char *data, int size, const IpEndpointName& remoteEndpoint)
{
	osc::ReceivedPacket p(data, size);

	if (debug_mode_) {
		std::cout << get_local_time_str_() << ",debug,recv,from=" << get_address_str_(remoteEndpoint.address) << ":" << remoteEndpoint.port << "," << p;
	}

	if (p.IsBundle()) {
		ProcessBundle(osc::ReceivedBundle(p), remoteEndpoint);
	}
	else {
		ProcessMessage(osc::ReceivedMessage(p), remoteEndpoint);
	}
}

void OSCThread::ProcessMessage(const osc::ReceivedMessage& msg, const IpEndpointName& remoteEndpoint)
{
	try{
		std::string path = std::string(msg.AddressPattern());

		// magic address pattern...
		if (path.size() > 5 && path.substr(0, 5) == "/get/") {
			command_reply_config_key_value_(msg, remoteEndpoint);
			return;
		}
		else if (path == "/save") {
			command_save_(msg, remoteEndpoint);
			return;
		}
		else if (path == "/load") {
			command_load_(msg, remoteEndpoint);
			return;
		}
		else if (path == "/clear") {
			command_clear_(msg, remoteEndpoint);
			return;
		}
		else if (path == "/remove") {
			command_remove_(msg, remoteEndpoint);
			return;
		}

		std::string key = path;
		osc::ReceivedMessage::const_iterator arg = msg.ArgumentsBegin();
		if (arg == msg.ArgumentsEnd()) {
			std::cout << get_local_time_str_() << ",error,recv invalid format packet...,from=" << get_address_str_(remoteEndpoint.address) << ":" << remoteEndpoint.port;
			return;
		}

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

//
// magic address pattern for getting config 
//
//   address pattern : /get/[type]
//   address pattern : /get/[type]/[reply_port]
//   address pattern : /get/[type]/[reply_port]/[reply_host]
//
//   parameter       : arg0  string  (key)
//
void OSCThread::command_reply_config_key_value_(const osc::ReceivedMessage& msg, const IpEndpointName& remoteEndpoint)
{
	try{
		//
		// parse address pattern
		//
		std::string path = std::string(msg.AddressPattern());

		unsigned short reply_port = remoteEndpoint.port;
		unsigned long reply_addr = remoteEndpoint.address;

		std::deque<std::string> v;
		boost::algorithm::split(v, path, boost::is_any_of("/"));
		v.pop_front(); // v[0] is ""...

		if (v.size() < 2) {
			printf("OSCThread::command_reply_config_key_value_() : invalid address pattern...path=%s\n", path.c_str());
			return;
		}

		// value type string
		// (int, bool, float, string) -> (i, b, f, s)
		std::string type = v[1].substr(0, 1);
		boost::algorithm::to_lower(type);

		// reply port, if arg2 is not null
		if (v.size() >= 3) {
			reply_port = (unsigned short)boost::lexical_cast<int>(v[2]);
		}

		IpEndpointName endpoint(reply_addr, reply_port);

		// reply hostname, if arg3 is not null
		if (v.size() >= 4) {
			endpoint = IpEndpointName(v[3].c_str(), reply_port);
		}

		// 
		// parse the argument as key string
		//
		osc::ReceivedMessage::const_iterator arg = msg.ArgumentsBegin();
		if (arg == msg.ArgumentsEnd()) {
			printf("OSCThread::command_reply_config_key_value_() : invalid argument packet...please set arg0 as the key string...\n", path.c_str());
			return;
		}

		std::string key;
		if (arg->IsString()) {
			key = arg->AsString();
		}
		else {
			printf("OSCThread::command_reply_config_key_value_() : invalid argument typet...please set arg0 as the key string...\n", path.c_str());
			return;
		}

		if (debug_mode_) {
			std::string reply_host = get_address_str_(endpoint.address);
			printf("%s,debug,send,to=%s:%d,addr=%s,val=%s\n", get_local_time_str_().c_str(), reply_host.c_str(), reply_port, key.c_str(), cf_->get_string(key).c_str());
		}

		//
		// build a reply message
		//
		osc::OutboundPacketStream p(send_packet_buf_, SEND_PACKET_BUF_SIZE);
		p << osc::BeginMessage(key.c_str());
		
		if (type == "b") {
			bool value = cf_->get_bool(key);
			p << value;
		}
		else if (type == "i") {
			int value = cf_->get_int(key);
			p << value;
		}
		else if (type == "f") {
			float value = cf_->get_float(key);
			p << value;
		}
		else if (type == "s") {
			std::string value = cf_->get_string(key);

			// check buffer size...
			int max_len = SEND_PACKET_BUF_SIZE - key.size() - 100;
			if ((int)value.size() > max_len) {
				printf("OSCThread::command_reply_config_key_value_() : warning! truncate large string value...value.size()=%d, max_len=%d\n", value.size(), max_len);
				value = value.substr(0, max_len);
			}

			p << value.c_str();
		}
		p << osc::EndMessage;

		//
		// send OSC packet
		//
		UdpTransmitSocket osc_send_socket(IpEndpointName(reply_addr, reply_port));
		osc_send_socket.Send(p.Data(), p.Size());
	}
	catch (osc::Exception& e) {
		printf("OSCThread::ProcessMessage() : osc::Exception!!!...e.what()=\n", e.what());
	}
}

void OSCThread::command_save_(const osc::ReceivedMessage& msg, const IpEndpointName& remoteEndpoint)
{
	bool rv = cf_->save();
	printf("%s,debug,save,filename=%s,result=%s\n", get_local_time_str_().c_str(), cf_->filename().c_str(), rv ? "true" : "false");
}

void OSCThread::command_load_(const osc::ReceivedMessage& msg, const IpEndpointName& remoteEndpoint)
{
	bool rv = cf_->load();
	printf("%s,debug,load,filename=%s,result=%s\n", get_local_time_str_().c_str(), cf_->filename().c_str(), rv ? "true" : "false");
}

void OSCThread::command_clear_(const osc::ReceivedMessage& msg, const IpEndpointName& remoteEndpoint)
{
	cf_->clear();
	printf("%s,debug,clear\n", get_local_time_str_().c_str());
}

void OSCThread::command_remove_(const osc::ReceivedMessage& msg, const IpEndpointName& remoteEndpoint)
{
	try {
		osc::ReceivedMessage::const_iterator arg = msg.ArgumentsBegin();
		if (arg == msg.ArgumentsEnd()) {
			printf("OSCThread::command_remove_config_() : invalid argument packet...please set arg0 as the key string...\n");
			return;
		}

		std::string key;
		if (arg->IsString()) {
			key = arg->AsString();

			cf_->remove(key);
			printf("%s,debug,remove,key=%s\n", get_local_time_str_().c_str(), key.c_str());
		}
		else {
			printf("OSCThread::command_remove_config_() : invalid argument packet...please set arg0 as the key string...\n");
		}
	}
	catch (osc::Exception& e) {
		printf("OSCThread::command_remove_config_() : osc::Exception!!!...e.what()=\n", e.what());
	}

}
