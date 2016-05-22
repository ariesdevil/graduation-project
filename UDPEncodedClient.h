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
		const std::string& sender_ip,
		const std::string& receiver_ip,
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
    BlockQueue<std::vector<char>> des_Q;
    BlockQueue<std::vector<EncodedPackage>> dec_Q;
    size_t last_ep_index;
    size_t this_ep_index;
};

