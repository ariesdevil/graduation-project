#include "Encoder.h"
#include "EncodedPackage.h"
#include "PaddingPackage.h"
#include <algorithm>
#include <set>
#include <map>
#include <random>
#include <cassert>
#include <cstdlib>
#include <time.h>
#include <iostream>
#include <cstdint>

using std::find;
using std::copy;
using std::sort;
using std::unique;
using std::distance;
using std::make_heap;
using std::sort_heap;
using std::set;
using std::map;
using std::default_random_engine;
using std::random_device;
using std::uniform_real_distribution; 

Encoder::Encoder(int k, int m, int l, const map<int, double>& pdf):
	k(k), m(m), l(l),
    pdf(vector<pair<int, double>>(pdf.cbegin(), pdf.cend()))
{
	srand(time(NULL));
    sort(this->pdf.begin(), this->pdf.end(),
            [](const pair<int, double> i, const pair<int, double> j) {
                return i < j;
            });
	for (auto iter = this->pdf.cbegin(); iter != this->pdf.cend(); iter++) {
		cdf.push_back(make_pair(iter->first, 
			accumulate(this->pdf.cbegin(), iter + 1, 0.0,
			[](double partialResult, pair<int, double> next) { 
                return partialResult + next.second;
            })));
	}
}


Encoder::~Encoder()
{
}

vector<EncodedPackage>
Encoder::encode(const PaddingPackage& p)
{
	vector<EncodedPackage> vec;
	for (int i = 0; i < m; i++) {
		EncodedPackage ep;
        ep.index = p.index;
		ep.d = popd();
		ep.adjacency = choose(ep.d);
		ep.data = vector<char>(l);

		for (int index: ep.adjacency) {
			for (int i = 0; i < l; i++) {
				ep.data[i] ^= p.pdata[index * l + i];
			}
		}
        //std::cerr << ep << std::endl;
		vec.push_back(ep);
	}
	return vec;
}

PaddingPackage
Encoder::decode(vector<EncodedPackage>& eps)
{
	assert(eps.size() <= m);
	PaddingPackage p(*this);
	set<EncodedPackage> s;

	//auto end=unique(eps.begin(), eps.end());
	//eps.erase(end, eps.end());
	auto compare = [](const EncodedPackage& i, const EncodedPackage& j)
					{ return i > j; };
	while (!eps.empty()) {
		make_heap(eps.begin(), eps.end(), compare);
		pop_heap(eps.begin(), eps.end(), compare);
		const EncodedPackage& this_ep = eps.back();
		if (this_ep.d == 1) {
			int index = this_ep.adjacency.front();
			for (EncodedPackage& that_ep: eps) {
				auto iter = find(that_ep.adjacency.cbegin(),
					that_ep.adjacency.cend(), index);
				if (that_ep.adjacency.cend() != iter) {
					if (this_ep == that_ep) {
						copy(this_ep.data.cbegin(), this_ep.data.cend(),
							p.pdata.begin() + index * l);
					} else {
						that_ep.d--;
						that_ep.adjacency.erase(iter);
						for (int j = 0; j < l; j++) {
							that_ep.data[j] ^= this_ep.data[j];
						}
					}
				}
			}
			s.insert(this_ep);
			//std::cerr << s.size() << std::endl;
			eps.pop_back();
		} else {
			break;
		}
	}

	int size = *(int32_t*)p.pdata.data();
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

vector<int>
Encoder::choose(int d)
{
	assert(d <= k);
	vector<int> v;
	int remain = k;
	int select = d;
	for (int i = 0; i < k; i++) {
		if (rand() % remain < select) {
			v.push_back(i);
			select--;
		}
		remain--;
	}
	return v;
}

int
Encoder::popd() {

    random_device rd;
    default_random_engine engine(rd());
	uniform_real_distribution<double> distribution;
	double random = distribution(engine);
	for (auto c : cdf) {
		if (random < c.second)
			return c.first;
	}
	return -1;
}


int
Encoder::getk() {
    return k;
}
int
Encoder::getm() {
    return m;
}
int
Encoder::getl() {
    return l;
}
int
Encoder::getdeg_max() {
    return pdf.back().first;
}
