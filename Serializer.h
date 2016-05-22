#pragma once

#include "EncodedPackage.h"
#include <vector>


class Serializer
{
public:
	Serializer();
	~Serializer();
    std::vector<char> serialize(const EncodedPackage& ep);
	EncodedPackage deserialize(const std::vector<char>& rd);
	EncodedPackage deserialize(const std::vector<char>& rd, size_t N);
};

