#include "UDPRawClient.h"
#include "cstddef"


UDPRawClient::UDPRawClient(
	const Encoder & e,
	const std::string & sender_ip,
	const std::string & receiver_ip,
	unsigned sender_port,
	unsigned receiver_port):
	UDPClient(e, sender_ip, receiver_ip, sender_port, receiver_port)
{
}

UDPRawClient::~UDPRawClient()
{
}

void 
UDPRawClient::run()
{
	buf.resize(1024);
	do_read();
	ioservice.run();
}

void
UDPRawClient::do_read()
{
    while (true) {
        size_t size = socket.receive_from(buffer(buf), sender_addr);
        write(STDOUT_FILENO, buf.data(), size);
    }
}
