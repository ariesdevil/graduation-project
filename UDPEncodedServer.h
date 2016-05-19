#pragma once
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>

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

private:
    void encode();

    std::thread thd;
    //std::queue<PaddingPackage> Q;
    std::queue<PaddingPackage> Q;
    std::mutex mtx;
    std::condition_variable Q_empty;
};

