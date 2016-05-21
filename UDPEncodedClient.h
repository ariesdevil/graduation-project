#pragma once
#include "UDPClient.h"
#include "BlockQueue.h"
#include <thread>

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
    std::thread dec_thd;
    BlockQueue<vector<char>> des_Q;
    BlockQueue<vector<EncodedPackage>> dec_Q;
    std::vector<EncodedPackage> eps;
    int last_ep_index;
    int this_ep_index;
};

