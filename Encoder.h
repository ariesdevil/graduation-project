#pragma once

#include <vector>
#include <map>
#include <utility>
#include <random>

using std::vector;
using std::map;
using std::pair;
using std::make_pair;
using std::default_random_engine;
using std::uniform_int_distribution;

class EncodedPackage;
class PaddingPackage;

class Encoder
{
	friend class PaddingPackage;
	friend class Encodedpackage;
public:
	Encoder(int k = 2000, int m = 3000, int l=128,
            const map<int, double>& pdf = {
                make_pair(0, 0),
                make_pair(1, 0.007971),
                make_pair(2, 0.493570),
                make_pair(3, 0.166220),
                make_pair(4, 0.072646),
                make_pair(5, 0.082558),
                make_pair(8, 0.056058),
                make_pair(9, 0.037229),
                make_pair(19, 0.055590),
                make_pair(65, 0.025023),
                make_pair(66, 0.003135)}
            );
	~Encoder();

	vector<EncodedPackage> encode(const PaddingPackage& p);
	PaddingPackage decode(vector<EncodedPackage>& eps);
    int getk();
    int getm();
    int getl();
    int getdeg_max();

public:
    int popd();
	vector<int> choose(int d); //从k个包里面随机选取d个包

	default_random_engine engine;
	int k;
	int m; //(m - k)/k即译码开销
	int l; //输入符号的长度
    vector<pair<int, double>> pdf;
    vector<pair<int, double>> cdf;

};

