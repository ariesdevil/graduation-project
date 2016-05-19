#pragma once

#include "EncodedPackage.h"
#include <vector>

using std::vector;

class Serializer
{
public:
	Serializer();
	~Serializer();
	vector<char> serialize(const EncodedPackage& ep);
	EncodedPackage deserialize(const vector<char>& rd);
	EncodedPackage deserialize(const vector<char>& rd, int N);
};

