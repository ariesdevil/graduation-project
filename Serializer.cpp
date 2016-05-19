#include "Serializer.h"
#include <algorithm>
#include <cstdint>
#include <cassert>

using std::copy;


Serializer::Serializer()
{
}


Serializer::~Serializer()
{
}

vector<char>
Serializer::serialize(const EncodedPackage & ep)
{
	vector<char> rv;
	char c[4];

	*(int32_t*)c = ep.index;
	rv.insert(rv.end(), c, c + 4);

	*(int32_t*)c = ep.d;
	rv.insert(rv.end(), c, c + 4);

	for (int i = 0; i < ep.d; i++) {
		*(int32_t*)c = ep.adjacency[i];
		rv.insert(rv.end(), c, c + 4);
	}

	rv.insert(rv.end(), ep.data.begin(), ep.data.end());

	return rv;
}

EncodedPackage
Serializer::deserialize(const vector<char>& rd)
{
    assert(rd.size() >= 4 * 2);
	EncodedPackage ep;

	ep.index = *(int32_t*)rd.data();

	ep.d = *(int32_t*)(rd.data() + 4);

	for (int i = 0; i < ep.d; i++) {
		int d = *(int32_t*)(rd.data() + 4 * (2 + i));
		ep.adjacency.push_back(d);
	}

	ep.data.insert(ep.data.end(), rd.begin() + 4 * (2 + ep.d), rd.end());

	return ep;
}


EncodedPackage
Serializer::deserialize(const vector<char>& rd, int N)
{
    assert(N >= 4 * 2);
	EncodedPackage ep;

	ep.index = *(int32_t*)rd.data();

	ep.d = *(int32_t*)(rd.data() + 4);

	for (int i = 0; i < ep.d; i++) {
		int d = *(int32_t*)(rd.data() + 4 * (2 + i));
		ep.adjacency.push_back(d);
	}

	ep.data.insert(ep.data.end(), rd.begin() + 4 * (2 + ep.d), rd.begin() + N);

	return ep;
}
