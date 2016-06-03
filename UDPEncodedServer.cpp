#include "UDPEncodedServer.h"
#include "PaddingPackage.h"
#include <future>
#include <cstddef>
#include <unistd.h>


UDPEncodedServer::UDPEncodedServer(
	const Encoder & e,
	const std::string & sender_ip,
	const std::string & receiver_ip,
	unsigned sender_port,
	unsigned receiver_port):
	UDPServer(e, sender_ip, receiver_ip, sender_port, receiver_port),
    Q(1)
{
    enc_thd = std::thread(&UDPEncodedServer::encode, this);
}


UDPEncodedServer::~UDPEncodedServer()
{
    enc_thd.join();
}

void
UDPEncodedServer::run()
{
	buf.resize(e.getk() * e.getl() - 4);
	do_read();
	ioservice.run();
}

void
UDPEncodedServer::do_read()
{
    while (true) {
        size_t size = read(STDIN_FILENO, buf.data(), buf.size());
        if (size > 0) {
            write(STDOUT_FILENO, buf.data(), size);
            PaddingPackage p(e, buf.data(), size);
            Q.push(p);
            //std::cerr << "编码队列：" << Q.size() << std::endl;

        }
    }
}

void
UDPEncodedServer::encode() {
    while (true) {
        PaddingPackage p(Q.pop());
        for (size_t i = 0; i < e.getm(); i++) {
            EncodedPackage ep(e.pop(p));
            //std::cerr << ep << std::endl;
            socket.send_to(buffer(s.serialize(ep)), receiver_addr);
            usleep(100);
        }
    }
}
