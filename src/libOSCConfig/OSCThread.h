#pragma once

#include "osc/OscPacketListener.h"

class Config;

class OSCThread : public osc::OscPacketListener
{
public:
	OSCThread(Config *cf);
	virtual ~OSCThread();

	bool is_open();

	bool open(const unsigned short &port);
	void run();
	void close();

	virtual void ProcessMessage(const osc::ReceivedMessage& msg, const IpEndpointName& remoteEndpoint);

protected:
	unsigned short port_;
	Config *cf_;
	bool break_flag_;

	boost::thread *thread_;
	UdpListeningReceiveSocket *osc_udp_socket_;
};

