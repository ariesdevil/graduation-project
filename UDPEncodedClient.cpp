#include "UDPEncodedClient.h"
#include "PaddingPackage.h"
#include <future>

UDPEncodedClient::UDPEncodedClient(
	const Encoder & e,
	const string & sender_ip,
	const string & receiver_ip,
	unsigned sender_port,
	unsigned receiver_port) :
	UDPClient(e, sender_ip, receiver_ip, sender_port, receiver_port)
{
    for (int i = 0; i < 3; i++) {
        threads.push_back(std::thread(&UDPEncodedClient::decode, this));
    }
}


UDPEncodedClient::~UDPEncodedClient()
{
    for (int i = 0; i < 3; i++) {
        threads[i].join();
    }
}

void
UDPEncodedClient::run()
{
	buf.resize(e.getl() + 4 * (66 + 1 + 1));
	do_read();
	ioservice.run();
}

void
UDPEncodedClient::do_read()
{
	int last_ep_index = 1;
    int this_ep_index = 1;
	vector<EncodedPackage> eps;
	while (true) {
        buf.resize(e.getl() + 4 * (e.getdeg_max() + 1 + 1));
		size_t size = socket.receive_from(buffer(buf), sender_addr);
        if (size != 0) {
            buf.resize(size);
            EncodedPackage ep(s.deserialize(buf));
            this_ep_index = ep.getindex();
            if (this_ep_index == last_ep_index) {
                eps.push_back(ep);
            } else {
                std::cerr << eps.size() << std::endl;
                std::lock_guard<std::mutex> locker(mtx);
                Q.push(eps);
                std::cerr << "主线程同步队列长度：" << Q.size() << std::endl;
                Q_empty.notify_one();
                eps.clear();
                eps.push_back(ep);
            }
            last_ep_index = this_ep_index;
        }
	}
}

void
UDPEncodedClient::decode() {
    while (true) {
        std::unique_lock<std::mutex> locker(mtx);
        Q_empty.wait(locker, [this]{ return !Q.empty(); });
        vector<EncodedPackage> eps(Q.front());
        std::cerr << "工作线程同步队列长度：" << Q.size() << std::endl;
        Q.pop();
        PaddingPackage p(e.decode(eps));
        pair<char*, int> data(p.getRawData());
        write(STDOUT_FILENO, data.first, data.second);
    }
}
