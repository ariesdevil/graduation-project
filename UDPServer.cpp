#include "UDPServer.h"
#include <vector>
#include <functional>
#include <unistd.h>

using std::vector;
using std::mem_fn;
using namespace::std::placeholders;
using boost::asio::ip::address;


UDPServer::UDPServer(
	const Encoder & e,
	const string & sender_ip,
	const string & receiver_ip,
	unsigned sender_port,
	unsigned receiver_port) :
	e(e),
	sender_addr(address::from_string(sender_ip), sender_port),
	receiver_addr(address::from_string(receiver_ip), receiver_port),
	socket(ioservice, sender_addr)
{
}

UDPServer::~UDPServer()
{
}
