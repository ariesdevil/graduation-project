#pragma once
#include "UDPServer.h"
class UDPEncodedServer :
	public UDPServer
{
public:
	UDPEncodedServer(
		const Encoder& e,
		const string& sender_ip,
		const string& receiver_ip,
		unsigned sender_port = 8080,
		unsigned receiver_port = 8080);
	~UDPEncodedServer();
	virtual void run();
	virtual void do_read();
};

