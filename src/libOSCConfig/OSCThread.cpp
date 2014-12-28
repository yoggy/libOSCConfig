#include "stdafx.h"

#include "Config.h"
#include "OSCThread.h"

#include "osc/OscOutboundPacketStream.h"

OSCThread::OSCThread(Config *cf) : port_(0), cf_(cf), break_flag_(false), thread_(nullptr), osc_recv_socket_(nullptr)
{
}

OSCThread::~OSCThread()
{
	if (is_open()) stop();
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
		printf("OSCThread::open() : osc::Exception!!! e.what()=%s", e.what());
		return false;
	}
	catch (...) {
		printf("OSCThread::open() : Exception!!! ");
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

void OSCThread::ProcessMessage(const osc::ReceivedMessage& msg, const IpEndpointName& remoteEndpoint)
{
	try{
		std::string path = std::string(msg.AddressPattern());

		// magic address pattern...
		if (path.size() > 5 && path.substr(0, 5) == "/get/") {
			reply_config_key_value_(msg, remoteEndpoint);
			return;
		}

		std::string key = path;
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

//
// magic address pattern for getting config 
//
//   address pattern : /get/[type]
//   address pattern : /get/[type]/[reply_port]
//   address pattern : /get/[type]/[reply_port]/[reply_host]
//
//   parameter       : arg0  string  (key)
//
void OSCThread::reply_config_key_value_(const osc::ReceivedMessage& msg, const IpEndpointName& remoteEndpoint)
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
			printf("OSCThread::reply_config_() : invalid address pattern...path=%s\n", path.c_str());
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
			printf("OSCThread::reply_config_() : invalid argument packet...please set arg0 as the key string...\n", path.c_str());
			return;
		}

		std::string key;
		if (arg->IsString()) {
			key = arg->AsString();
		}
		else {
			printf("OSCThread::reply_config_() : invalid argument typet...please set arg0 as the key string...\n", path.c_str());
			return;
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
				printf("OSCThread::reply_config_() : warning! truncate large string value...value.size()=%d, max_len=%d\n", value.size(), max_len);
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
