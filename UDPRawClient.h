#pragma once
#include "UDPClient.h"
class UDPRawClient :
	public UDPClient
{
public:
	UDPRawClient(
		const Encoder& e,
		const std::string& sender_ip,
		const std::string& receiver_ip,
		unsigned sender_port = 8081,
		unsigned receiver_port = 8081);
	~UDPRawClient();
	virtual void run();
	virtual void do_read();
};

