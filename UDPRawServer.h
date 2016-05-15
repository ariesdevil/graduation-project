#pragma once
#include "UDPServer.h"
class UDPRawServer :
	public UDPServer
{
public:
	UDPRawServer(
		const Encoder& e,
		const string& sender_ip,
		const string& receiver_ip,
		unsigned sender_port = 8081,
		unsigned receiver_port = 8081);
	~UDPRawServer();
	virtual void run();
	virtual void do_read();
};

