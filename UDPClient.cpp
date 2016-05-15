#include "UDPClient.h"
#include <unistd.h>



UDPClient::UDPClient(
	const Encoder & e,
	const string & sender_ip,
	const string & receiver_ip,
	unsigned sender_port,
	unsigned receiver_port):
	e(e),
	sender_addr(address::from_string(sender_ip), sender_port),
	receiver_addr(address::from_string(receiver_ip), receiver_port),
	socket(ioservice, receiver_addr)
{
}

UDPClient::~UDPClient()
{
}
