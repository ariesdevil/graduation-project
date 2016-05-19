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
    void deserialize();
    void decode();

    std::thread des_thd;
    std::queue<vector<char>> des_Q;
    std::mutex des_mtx;
    std::condition_variable des_Q_empty;

    std::vector<EncodedPackage> eps;

    std::thread dec_thd;
    std::queue<vector<EncodedPackage>> dec_Q;
    std::mutex dec_mtx;
    std::condition_variable dec_Q_empty;

    int last_ep_index;
    int this_ep_index;
};

