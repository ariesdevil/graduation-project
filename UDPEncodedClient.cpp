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
            des_Q.push(std::vector<char>(buf.begin(), buf.begin() + size));
            //std::cerr << "解序列队列：" << des_Q.size() << std::endl;
        }
	}
}


void
UDPEncodedClient::deserialize() {
    vector<EncodedPackage> eps;
    while (true) {
        EncodedPackage ep(s.deserialize(des_Q.pop()));
        this_ep_index = ep.getindex();
        if (this_ep_index == last_ep_index) {
            eps.push_back(std::move(ep));
        } else {
            dec_Q.push(std::move(eps));
            //std::cerr << "解码队列：" << dec_Q.size() << std::endl;
            eps.clear();
            eps.push_back(std::move(ep));
        }
        last_ep_index = this_ep_index;
    }
}


void
UDPEncodedClient::decode() {
    while (true) {
        vector<EncodedPackage> eps(dec_Q.pop());
        std::cerr << "编码包数量" << eps.size() << std::endl;
        /*
        for (const auto& ep: eps) {
            std::cerr << ep << std::endl;
        }
        */
        PaddingPackage p(e.decode(eps));
        pair<char*, int> data(p.getRawData());
        write(STDOUT_FILENO, data.first, data.second);
    }
}
