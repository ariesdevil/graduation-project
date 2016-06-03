#include "UDPEncodedClient.h"
#include "PaddingPackage.h"
#include <future>

UDPEncodedClient::UDPEncodedClient(
	const Encoder & e,
	const std::string & sender_ip,
	const std::string & receiver_ip,
	unsigned sender_port,
	unsigned receiver_port):
	UDPClient(e, sender_ip, receiver_ip, sender_port, receiver_port),
    dec_Q(4),
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
	buf.resize(e.getl() + 4 * (e.getdeg_max() + 1 + 1));
	do_read();
	ioservice.run();
}

void
UDPEncodedClient::do_read()
{
    std::vector<EncodedPackage> eps;
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
    std::vector<EncodedPackage> eps;
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
        std::vector<EncodedPackage> eps(dec_Q.pop());
        //std::cerr << "编码包数量" << eps.size() << std::endl;
        /*
        for (const auto& ep: eps) {
            std::cerr << ep << std::endl;
        }
        */
        PaddingPackage p(e.decode(eps));
        std::pair<char*, size_t> data(p.getRawData());
        write(STDOUT_FILENO, data.first, data.second);
    }
}
