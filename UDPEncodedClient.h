#pragma once
#include "UDPClient.h"
class UDPEncodedClient :
	public UDPClient
{
public:
	UDPEncodedClient(
		const Encoder& e,
		const string& sender_ip,
		const string& receiver_ip,
		unsigned sender_port = 8080,
		unsigned receiver_port = 8080);
	~UDPEncodedClient();
	virtual void run();
	virtual void do_read();

private:
    void decode(vector<EncodedPackage> eps);
};

