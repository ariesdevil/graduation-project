#pragma once
#include "Encoder.h"
#include "Serializer.h"
#include <string>
#include <boost/asio.hpp>

using namespace boost::asio;
using namespace boost::asio::ip;

class UDPServer
{
public:
	UDPServer(
		const Encoder& e,
		const std::string& sender_ip,
		const std::string& receiver_ip,
		unsigned sender_port = 8080,
		unsigned receiver_port = 8080);
	~UDPServer();
	virtual void run() = 0;
	virtual void do_read() = 0;

protected:
	Encoder e;
	Serializer s;
    std::vector<char> buf;
	io_service ioservice;
	udp::endpoint sender_addr;
	udp::endpoint receiver_addr;
	udp::socket socket;
};
