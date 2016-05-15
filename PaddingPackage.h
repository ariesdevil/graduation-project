#pragma once

#include <vector>
#include <map>
using std::vector;
using std::pair;
using std::make_pair;

class Encoder;

class PaddingPackage
{
	friend class Encoder;
public:
	PaddingPackage(const Encoder& e);
	PaddingPackage(const Encoder& e, const vector<char>& rdata);
	PaddingPackage(const Encoder& e, char* rdata, int rsize);
	~PaddingPackage();
	pair<char*, int> getRawData();


private:
	static int index; //原始数据的编号
	int rsize; //原始数据大小
	int psize; //补零和添加长度头后的数据大小
	vector<char> pdata; //补零后的数据
};

