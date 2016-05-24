#include "PaddingPackage.h"
#include "Encoder.h"
#include <algorithm>
#include <map>
#include <new>
#include <cassert>
#include <cstdint>
#include <iostream>

size_t PaddingPackage::count = 0;

PaddingPackage::PaddingPackage(const Encoder& e)
	:index(count), rsize(0), psize(e.getk() * e.getl()), pdata(psize)
{
	if (count < 65535) {
		count++;
	} else {
		count = 0;
	}
}

PaddingPackage::PaddingPackage(const Encoder& e, const std::vector<char>& rdata)
	:index(count), rsize(rdata.size()), psize(e.getk() * e.getl())
{
	assert(rsize + 4 <= psize);
	pdata.resize(psize);
	char s[4];
	*(uint32_t*)s = rsize;
	copy(s, s + 4, pdata.begin());
	copy(rdata.cbegin(), rdata.cend(), pdata.begin() + 4);
	if (count < 65535) {
		count++;
	} else {
		count = 0;
	}
}

PaddingPackage::PaddingPackage(const Encoder& e, char* rdata, size_t rsize)
	:index(count), rsize(rsize), psize(e.getk() * e.getl())
{
	assert(rsize + 4 <= psize);
	pdata.resize(psize);
	char s[4];
	*(uint32_t*)s = rsize;
	copy(s, s + 4, pdata.begin());
	copy(rdata, rdata + rsize, pdata.begin() + 4);
	if (count < 65535) {
		count++;
	}
	else {
		count = 0;
	}
}


PaddingPackage::~PaddingPackage()
{
}

std::pair<char*, size_t>
PaddingPackage::getRawData() {
	return std::make_pair(pdata.data() + 4, rsize);
}

size_t
PaddingPackage::getindex() {
    return index;
}
