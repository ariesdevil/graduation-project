#pragma once
#include <vector>
#include <iostream>

class EncodedPackage
{
	friend class Encoder;
	friend class Serializer;
	friend bool operator==(const EncodedPackage& lep, const EncodedPackage& rep);
	friend bool operator!=(const EncodedPackage& lep, const EncodedPackage& rep);
	friend bool operator<(const EncodedPackage& lep, const EncodedPackage& rep);
	friend bool operator>(const EncodedPackage& lep, const EncodedPackage& rep);
	friend std::ostream& operator<<(std::ostream& os, const EncodedPackage& ep);

public:
	EncodedPackage();
	~EncodedPackage();
	unsigned getd();
	unsigned getindex();

private:
	unsigned index; //所属原包的索引
	unsigned d; //此编码包的度
    std::vector<unsigned> adjacency; //邻接关系
    std::vector<char> data; //编码后的数据
};

