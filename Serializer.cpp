#include "Serializer.h"
#include <algorithm>
#include <cstdint>
#include <cassert>

Serializer::Serializer()
{
}


Serializer::~Serializer()
{
}

std::vector<char>
Serializer::serialize(const EncodedPackage & ep)
{
    std::vector<char> rv;
	char c[4];

	*(uint32_t*)c = ep.index;
	rv.insert(rv.end(), c, c + 4);

	*(uint32_t*)c = ep.d;
	rv.insert(rv.end(), c, c + 4);

	for (size_t i = 0; i < ep.d; i++) {
		*(uint32_t*)c = ep.adjacency[i];
		rv.insert(rv.end(), c, c + 4);
	}

	rv.insert(rv.end(), ep.data.begin(), ep.data.end());

	return rv;
}

EncodedPackage
Serializer::deserialize(const std::vector<char>& rd)
{
    assert(rd.size() >= 4 * 2);
	EncodedPackage ep;

	ep.index = *(uint32_t*)rd.data();

	ep.d = *(uint32_t*)(rd.data() + 4);

	for (size_t i = 0; i < ep.d; i++) {
		unsigned d = *(uint32_t*)(rd.data() + 4 * (2 + i));
		ep.adjacency.push_back(d);
	}

	ep.data.insert(ep.data.end(), rd.begin() + 4 * (2 + ep.d), rd.end());

	return ep;
}


EncodedPackage
Serializer::deserialize(const std::vector<char>& rd, size_t N)
{
    assert(N >= 4 * 2);
	EncodedPackage ep;

	ep.index = *(uint32_t*)rd.data();

	ep.d = *(uint32_t*)(rd.data() + 4);

	for (size_t i = 0; i < ep.d; i++) {
		unsigned d = *(uint32_t*)(rd.data() + 4 * (2 + i));
		ep.adjacency.push_back(d);
	}

	ep.data.insert(ep.data.end(), rd.begin() + 4 * (2 + ep.d), rd.begin() + N);

	return ep;
}
