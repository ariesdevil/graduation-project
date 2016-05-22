#pragma once

#include <vector>
#include <map>

class Encoder;
class PaddingPackage
{
	friend class Encoder;
public:
	PaddingPackage(const Encoder& e);
	PaddingPackage(const Encoder& e, const std::vector<char>& rdata);
	PaddingPackage(const Encoder& e, char* rdata, size_t rsize);
	~PaddingPackage();
    std::pair<char*, size_t> getRawData();
    size_t getindex();


private:
	static size_t count; //原始数据的编号
    size_t index;
	size_t rsize; //原始数据大小
	size_t psize; //补零和添加长度头后的数据大小
    std::vector<char> pdata; //补零后的数据
};

