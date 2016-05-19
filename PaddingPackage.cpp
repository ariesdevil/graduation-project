#include "PaddingPackage.h"
#include "Encoder.h"
#include <algorithm>
#include <map>
#include <new>
#include <cassert>
#include <cstdint>
#include <iostream>

int PaddingPackage::count = 0;

PaddingPackage::PaddingPackage(const Encoder& e)
	:index(count), rsize(0), psize(e.k * e.l), pdata(psize)
{
	if (count < 65535) {
		count++;
	} else {
		count = 0;
	}
}

PaddingPackage::PaddingPackage(const Encoder& e, const vector<char>& rdata)
	:index(count), rsize(rdata.size()), psize(e.k * e.l)
{
	assert(rsize + 4 <= psize);
	pdata.resize(psize);
	char s[4];
	*(int32_t*)s = rsize;
	copy(s, s + 4, pdata.begin());
	copy(rdata.cbegin(), rdata.cend(), pdata.begin() + 4);
	if (count < 65535) {
		count++;
	} else {
		count = 0;
	}
}

PaddingPackage::PaddingPackage(const Encoder& e, char* rdata, int rsize)
	:index(count), rsize(rsize), psize(e.k * e.l)
{
	assert(rsize + 4 <= psize);
	pdata.resize(psize);
	char s[4];
	*(int32_t*)s = rsize;
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

pair<char*, int>
PaddingPackage::getRawData() {
	return make_pair(pdata.data() + 4, rsize);
}

int
PaddingPackage::getindex() {
    return index;
}
