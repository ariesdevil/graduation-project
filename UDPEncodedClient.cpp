#include "UDPEncodedClient.h"
#include "PaddingPackage.h"
#include <future>

UDPEncodedClient::UDPEncodedClient(
	const Encoder & e,
	const string & sender_ip,
	const string & receiver_ip,
	unsigned sender_port,
	unsigned receiver_port):
	UDPClient(e, sender_ip, receiver_ip, sender_port, receiver_port),
    last_ep_index(0),
    this_ep_index(0)
{
    des_thd = std::thread(&UDPEncodedClient::deserialize, this);
    dec_thd = std::thread(&UDPEncodedClient::decode, this);
}


UDPEncodedClient::~UDPEncodedClient()
{
    des_thd.join();
    dec_thd.join();
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
	vector<EncodedPackage> eps;
	while (true) {
		size_t size = socket.receive_from(buffer(buf), sender_addr);
        if (size != 0) {
            std::lock_guard<std::mutex> des_lck(des_mtx);
            des_Q.push(std::vector<char>(buf.begin(), buf.begin() + size));
            //std::cerr << "解序列队列长度：" << des_Q.size() << std::endl;
            des_Q_empty.notify_one();
        }
	}
}


void
UDPEncodedClient::deserialize() {
    while (true) {
        std::unique_lock<std::mutex> des_lck(des_mtx);
        des_Q_empty.wait(des_lck, [this]() { return !des_Q.empty(); });
        EncodedPackage ep(s.deserialize(des_Q.front()));
        des_Q.pop();
        des_lck.unlock();
        this_ep_index = ep.getindex();
        if (this_ep_index == last_ep_index) {
            eps.push_back(std::move(ep));
        } else {
            std::lock_guard<std::mutex> dec_lck(dec_mtx);
            dec_Q.push(std::move(eps));
            //std::cerr << "解码序列长度：" << dec_Q.size() << std::endl;
            dec_Q_empty.notify_one();
            eps.clear();
            eps.push_back(std::move(ep));
        }
        last_ep_index = this_ep_index;
    }
}


void
UDPEncodedClient::decode() {
    while (true) {
        std::unique_lock<std::mutex> dec_lck(dec_mtx);
        dec_Q_empty.wait(dec_lck, [this]{ return !dec_Q.empty(); });
        vector<EncodedPackage> eps(dec_Q.front());
        //std::cerr << "编码包数量" << eps.size() << std::endl;
        for (const auto& ep: eps) {
            std::cerr << ep << std::endl;
        }
        dec_Q.pop();
        PaddingPackage p(e.decode(eps));
        pair<char*, int> data(p.getRawData());
        write(STDOUT_FILENO, data.first, data.second);
    }
}
