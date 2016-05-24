#pragma once

#include <vector>
#include <map>
#include <utility>
#include <random>

class EncodedPackage;
class PaddingPackage;

class Encoder
{
public:
	Encoder(const unsigned k = 2000, const unsigned m = 3000, const unsigned l=128,
            const std::map<unsigned, double>& pdf = {
                std::make_pair(0, 0),
                std::make_pair(1, 0.007971),
                std::make_pair(2, 0.493570),
                std::make_pair(3, 0.166220),
                std::make_pair(4, 0.072646),
                std::make_pair(5, 0.082558),
                std::make_pair(8, 0.056058),
                std::make_pair(9, 0.037229),
                std::make_pair(19, 0.055590),
                std::make_pair(65, 0.025023),
                std::make_pair(66, 0.003135)}
            );
	~Encoder();

    std::vector<EncodedPackage> encode(const PaddingPackage& p);
    EncodedPackage pop(const PaddingPackage& p);
	PaddingPackage decode(std::vector<EncodedPackage>& eps);
    unsigned getk() const;
    unsigned getm() const;
    unsigned getl() const;
    unsigned getdeg_max() const;

private:
    unsigned popd();
    std::vector<unsigned> choose(unsigned d); //从k个包里面随机选取d个包

    std::default_random_engine engine;
	unsigned k;
	unsigned m; //(m - k)/k即译码开销
	unsigned l; //输入符号的长度
    std::vector<std::pair<unsigned, double>> pdf;
    std::vector<std::pair<unsigned, double>> cdf;

};

