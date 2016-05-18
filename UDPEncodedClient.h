#pragma once
#include "UDPClient.h"
#include <array>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>

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
    void decode();
    std::queue<vector<EncodedPackage>> Q;
    std::vector<std::thread> threads;
    std::mutex mtx;
    std::condition_variable Q_empty;
};

