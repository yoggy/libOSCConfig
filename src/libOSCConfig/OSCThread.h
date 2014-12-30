#pragma once

#include "osc/OscPacketListener.h"

#define SEND_PACKET_BUF_SIZE 4096

class Config;

class OSCThread : public osc::OscPacketListener
{
public:
	OSCThread(Config *cf);
	virtual ~OSCThread();

	bool debug_mode() const;
	void debug_mode(const bool &val);

	bool is_open();

	bool start(const unsigned short &port);
	bool start_kvs_mode(const unsigned short &port);
	void run();
	void stop();

	virtual void ProcessPacket(const char *data, int size, const IpEndpointName& remoteEndpoint);
	virtual void ProcessMessage(const osc::ReceivedMessage& msg, const IpEndpointName& remoteEndpoint);

protected:
	bool open_(const unsigned short &port);
	void reply_config_key_value_(const osc::ReceivedMessage& msg, const IpEndpointName& remoteEndpoint);
	void save_config_(const osc::ReceivedMessage& msg, const IpEndpointName& remoteEndpoint);
	void load_config_(const osc::ReceivedMessage& msg, const IpEndpointName& remoteEndpoint);

protected:
	unsigned short port_;
	Config *cf_;
	bool break_flag_;
	bool debug_mode_;

	boost::thread *thread_;
	UdpListeningReceiveSocket *osc_recv_socket_;

	char send_packet_buf_[SEND_PACKET_BUF_SIZE];
};

