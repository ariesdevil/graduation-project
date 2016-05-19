#include "UDPEncodedServer.h"
#include "PaddingPackage.h"
#include <future>
#include <cstddef>
#include <unistd.h>


UDPEncodedServer::UDPEncodedServer(
	const Encoder & e,
	const string & sender_ip,
	const string & receiver_ip,
	unsigned sender_port,
	unsigned receiver_port):
	UDPServer(e, sender_ip, receiver_ip, sender_port, receiver_port)
{
    thd = std::thread(&UDPEncodedServer::encode, this);
}


UDPEncodedServer::~UDPEncodedServer()
{
    thd.join();
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
            std::lock_guard<std::mutex> lck(mtx);
            Q.push(p);
            //std::cerr << "push: " << p.getindex() << std::endl;
            Q_empty.notify_one();

        }
    }
}

void
UDPEncodedServer::encode() {
    while (true) {
        std::unique_lock<std::mutex> lck(mtx);
        Q_empty.wait(lck, [this]{ return !Q.empty(); });
        //std::cerr << "pop: " << Q.front().getindex() << std::endl;
        PaddingPackage p(Q.front());
        Q.pop();
        for (int i = 0; i < e.getm(); i++) {
            EncodedPackage ep(e.pop(p));
            //std::cerr << ep << std::endl;
            size_t size = socket.send_to(buffer(s.serialize(ep)), receiver_addr);
            usleep(100);
        }
    }
}
