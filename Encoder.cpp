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
    ep.data = vector<char>(l);

    for (int index: ep.adjacency) {
        for (int i = 0; i < l; i++) {
            ep.data[i] ^= p.pdata[index * l + i];
        }
    }
    //std::cerr << ep << std::endl;
    return ep;
}

PaddingPackage
Encoder::decode(vector<EncodedPackage>& eps)
{
	assert(eps.size() <= m);
	PaddingPackage p(*this);
	set<EncodedPackage> s;
    vector<int> eps_index;
    for (size_t i = 0; i < eps.size(); i++) {
        eps_index.push_back(i);
    }

	//auto end=unique(eps.begin(), eps.end());
	//eps.erase(end, eps.end());
	auto compare = [&eps](const int i, const int j)
					{ return eps.at(i) > eps.at(j); };
	while (!eps_index.empty()) {
		make_heap(eps_index.begin(), eps_index.end(), compare);
		pop_heap(eps_index.begin(), eps_index.end(), compare);
		const EncodedPackage& this_ep = eps.at(eps_index.back());
		if (this_ep.d == 1) {
			int index = this_ep.adjacency.front();
            for (int ep_index: eps_index) {
			//for (EncodedPackage& that_ep: eps) {
                EncodedPackage& that_ep = eps.at(ep_index);
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
            eps_index.pop_back();
			//eps.pop_back();
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
