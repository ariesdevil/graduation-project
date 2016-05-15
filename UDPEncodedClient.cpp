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
}


UDPEncodedClient::~UDPEncodedClient()
{
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
            std::cerr << ep << std:: endl;
            if (this_ep_index == last_ep_index) {
                eps.push_back(ep);
            } else {
                /*
                PaddingPackage p(e.decode(eps));
                pair<char*, int> data(p.getRawData());
                write(STDOUT_FILENO, data.first, data.second);
                */
                std::thread decode_thread(&UDPEncodedClient::decode, this, eps);
                decode_thread.detach();
                eps.clear();
                eps.push_back(ep);
            }
            last_ep_index = this_ep_index;
        }
	}
}

void
UDPEncodedClient::decode(vector<EncodedPackage> eps) {
    PaddingPackage p(e.decode(eps));
    pair<char*, int> data(p.getRawData());
    write(STDOUT_FILENO, data.first, data.second);
}
