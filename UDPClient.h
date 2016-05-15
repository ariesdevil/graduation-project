#pragma once
#include "Encoder.h"
#include "Serializer.h"
#include <string>
#include <boost/asio.hpp>

using std::string;
using namespace boost::asio;
using namespace boost::asio::ip;

class UDPClient
{
public:
	UDPClient(
		const Encoder& e,
		const string& sender_ip,
		const string& receiver_ip,
		unsigned sender_port = 8080,
		unsigned receiver_port = 8080);
	~UDPClient();
	virtual void run() = 0;
	virtual void do_read() = 0;

protected:
	Encoder e;
	Serializer s;
	vector<char> buf;
	io_service ioservice;
	udp::endpoint sender_addr;
	udp::endpoint receiver_addr;
	udp::socket socket;
};

