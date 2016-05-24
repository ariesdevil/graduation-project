#pragma once
#include <vector>

/**
 * http://ieeexplore.ieee.org/stamp/stamp.jsp?tp=&arnumber=6491868
 * Analysis of Robust Soliton Distribution for LT Code 
 */
class RobustSolitonDistribution {
public:
    RobustSolitonDistribution(
            const unsigned k=100,
            const double c=0.1,
            const double delta=0.5);
    const std::vector<std::pair<unsigned, double>>& getpdf();
private:
    void calpdf();

    unsigned k;
    double c;
    double delta;
    unsigned R;
    unsigned x;
    std::vector<std::pair<unsigned, double>> pdf;
};

