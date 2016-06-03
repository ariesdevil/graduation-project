#include "UDPRawServer.h"
#include <future>

UDPRawServer::UDPRawServer(
	const Encoder & e,
	const std::string & sender_ip,
	const std::string & receiver_ip,
	unsigned sender_port,
	unsigned receiver_port):
	UDPServer(e, sender_ip, receiver_ip, sender_port, receiver_port)
{
}


UDPRawServer::~UDPRawServer()
{
}

void UDPRawServer::run()
{
	buf.resize(1024);
	do_read();
	ioservice.run();
}

void
UDPRawServer::do_read()
{
    while (true) {
        size_t size = read(STDIN_FILENO, buf.data(), buf.size());
        write(STDOUT_FILENO, buf.data(), size);
        socket.send_to(buffer(buf, size), receiver_addr);
        usleep(200);
    }
}
