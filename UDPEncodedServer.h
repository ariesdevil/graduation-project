#pragma once
#include "UDPServer.h"
#include "BlockQueue.h"
#include <thread>

class UDPEncodedServer :
	public UDPServer
{
public:
	UDPEncodedServer(
		const Encoder& e,
		const std::string& sender_ip,
		const std::string& receiver_ip,
		unsigned sender_port = 8080,
		unsigned receiver_port = 8080);
	~UDPEncodedServer();
	virtual void run();
	virtual void do_read();

private:
    void encode();

    std::thread enc_thd;
    BlockQueue<PaddingPackage> Q;
};

