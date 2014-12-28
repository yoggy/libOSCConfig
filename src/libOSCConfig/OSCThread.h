#pragma once

#include "osc/OscPacketListener.h"

#define SEND_PACKET_BUF_SIZE 4096

class Config;

class OSCThread : public osc::OscPacketListener
{
public:
	OSCThread(Config *cf);
	virtual ~OSCThread();

	bool is_open();

	bool start(const unsigned short &port);
	bool start_kvs_mode(const unsigned short &port);
	void run();
	void stop();

	virtual void ProcessMessage(const osc::ReceivedMessage& msg, const IpEndpointName& remoteEndpoint);

protected:
	bool open_(const unsigned short &port);
	void reply_config_key_value_(const osc::ReceivedMessage& msg, const IpEndpointName& remoteEndpoint);

protected:
	unsigned short port_;
	Config *cf_;
	bool break_flag_;

	boost::thread *thread_;
	UdpListeningReceiveSocket *osc_recv_socket_;

	char send_packet_buf_[SEND_PACKET_BUF_SIZE];
};

