#include "Encoder.h"
#include "EncodedPackage.h"
#include "PaddingPackage.h"
#include <algorithm>
#include <numeric>
#include <set>
#include <map>
#include <random>
#include <cassert>
#include <cstdlib>
#include <time.h>
#include <iostream>
#include <cstdint>

Encoder::Encoder(const unsigned k, const unsigned m, const unsigned l, const std::map<unsigned, double>& pdf):
	k(k), m(m), l(l),
    pdf(std::vector<std::pair<unsigned, double>>(pdf.cbegin(), pdf.cend()))
{
	srand(time(NULL));
    std::sort(this->pdf.begin(), this->pdf.end(),
            [](const std::pair<unsigned, double> i, const std::pair<unsigned, double> j) {
                return i < j;
            });
	for (auto iter = this->pdf.cbegin(); iter != this->pdf.cend(); iter++) {
		cdf.push_back(std::make_pair(iter->first, 
			accumulate(this->pdf.cbegin(), iter + 1, 0.0,
			[](double partialResult, std::pair<unsigned, double> next) { 
                return partialResult + next.second;
            })));
	}
}


Encoder::~Encoder()
{
}

std::vector<EncodedPackage>
Encoder::encode(const PaddingPackage& p)
{
    std::vector<EncodedPackage> vec;
	for (size_t i = 0; i < m; i++) {
		vec.push_back(pop(p));
	}
	return vec;
}

EncodedPackage
Encoder::pop(const PaddingPackage& p) {
    EncodedPackage ep;
    ep.index = p.index;
    ep.d = popd();
    ep.adjacency = choose(ep.d);
    ep.data = std::vector<char>(l);

    for (unsigned index: ep.adjacency) {
        for (size_t i = 0; i < l; i++) {
            ep.data[i] ^= p.pdata[index * l + i];
        }
    }
    //std::cerr << ep << std::endl;
    return ep;
}

PaddingPackage
Encoder::decode(std::vector<EncodedPackage>& eps)
{
	assert(eps.size() <= m);
	PaddingPackage p(*this);
    std::set<EncodedPackage> s;
    std::vector<unsigned> eps_index;
    for (size_t i = 0; i < eps.size(); i++) {
        eps_index.push_back(i);
    }

	//auto end=std::unique(eps.begin(), eps.end());
	//eps.erase(end, eps.end());
	auto compare = [&eps](const unsigned i, const unsigned j)
					{ return eps.at(i) > eps.at(j); };
	while (!eps_index.empty()) {
        std::make_heap(eps_index.begin(), eps_index.end(), compare);
		pop_heap(eps_index.begin(), eps_index.end(), compare);
		const EncodedPackage& this_ep = eps.at(eps_index.back());
		if (this_ep.d == 1) {
			unsigned index = this_ep.adjacency.front();
            for (unsigned ep_index: eps_index) {
			//for (EncodedPackage& that_ep: eps) {
                EncodedPackage& that_ep = eps.at(ep_index);
				auto iter = std::find(that_ep.adjacency.cbegin(),
					that_ep.adjacency.cend(), index);
				if (that_ep.adjacency.cend() != iter) {
					if (this_ep == that_ep) {
                        std::copy(this_ep.data.cbegin(), this_ep.data.cend(),
							p.pdata.begin() + index * l);
					} else {
						that_ep.d--;
						that_ep.adjacency.erase(iter);
						for (size_t j = 0; j < l; j++) {
							that_ep.data[j] ^= this_ep.data[j];
						}
					}
				}
			}
			s.insert(this_ep);
			//std::cerr << s.size() << std::endl;
            eps_index.pop_back();
			//eps.pop_back();
		} else {
			break;
		}
	}

	size_t size = *(uint32_t*)p.pdata.data();
	p.rsize = size ? size : (p.psize - 4);

    /*
    for (const EncodedPackage& ep: eps) {
        std::cerr << ep << std::endl;
    }
	
	if (eps.empty()) {
		std::cerr << "success " << s.size() << std::endl;
	} else {
		std::cerr << "failure " << s.size() << std::endl;
	}
	std::cerr << "rsize: " << p.rsize << std::endl;
    */
	return p;
}

std::vector<unsigned>
Encoder::choose(unsigned d)
{
	assert(d <= k);
    std::vector<unsigned> v;
	unsigned remain = k;
	unsigned select = d;
	for (size_t i = 0; i < k; i++) {
		if (rand() % remain < select) {
			v.push_back(i);
			select--;
		}
		remain--;
	}
	return v;
}

unsigned
Encoder::popd() {

    std::random_device rd;
    std::default_random_engine engine(rd());
    std::uniform_real_distribution<double> distribution;
	double random = distribution(engine);
	for (const auto& c : cdf) {
		if (random < c.second)
			return c.first;
	}
	return 0;
}


unsigned
Encoder::getk() const {
    return k;
}
unsigned
Encoder::getm() const {
    return m;
}
unsigned
Encoder::getl() const {
    return l;
}
unsigned
Encoder::getdeg_max() const {
    return pdf.back().first;
}
