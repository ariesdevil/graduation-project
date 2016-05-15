#pragma once
#include <vector>
#include <iostream>

using std::vector;
using std::ostream;

class EncodedPackage
{
	friend class Encoder;
	friend class Serializer;
	friend bool operator==(const EncodedPackage& lep, const EncodedPackage& rep);
	friend bool operator!=(const EncodedPackage& lep, const EncodedPackage& rep);
	friend bool operator<(const EncodedPackage& lep, const EncodedPackage& rep);
	friend bool operator>(const EncodedPackage& lep, const EncodedPackage& rep);
	friend ostream& operator<<(ostream& os, const EncodedPackage& ep);

public:
	EncodedPackage();
	~EncodedPackage();
	int getd();
	int getindex();

private:
	int index; //所属原包的索引
	int d; //此编码包的度
	vector<int> adjacency; //邻接关系
	vector<char> data; //编码后的数据
};

