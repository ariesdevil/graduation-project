#include "RobustSolitonDistribution.h"
#include <cassert>
#include <utility>
#include <cstdlib>
#include <cmath>
#include <numeric>
#include <iostream>

RobustSolitonDistribution::RobustSolitonDistribution(
        const unsigned k,
        const double c,
        const double delta)
{
    assert(k > 0);
    this->k = k;
    this->c = c;
    this->delta = delta;
    this->R = c * log(k/delta) * sqrt(k);
    assert(k > R);
    this->x = ceil(k/R);
    calpdf();
}

void
RobustSolitonDistribution::calpdf()
{
    auto sum = [](double partialResult, const std::pair<unsigned, double>& next) { 
        return partialResult + next.second;
    };

    std::vector<std::pair<unsigned, double>> p;
    p.push_back(std::make_pair(1, 1.0/k));
    for (size_t i = 2; i <= k; ++i) {
        p.push_back(std::make_pair(i, 1.0 / (i * (i - 1))));
    }

    std::vector<std::pair<unsigned, double>> t;
    for (size_t i = 1; i < x; ++i) {
        t.push_back(std::make_pair(i, 1.0 / (i * x)));
    }
    t.push_back(std::make_pair(x, log(R/delta) / x));
    for (size_t i = x + 1; i <= k; ++i) {
        t.push_back(std::make_pair(i, 0.0));
    }

    double beta = 1.0 + std::accumulate(t.cbegin(), t.cend(), 0.0, sum);
    for (unsigned i = 0; i < x; ++i) {
        pdf.push_back(std::make_pair(i + 1, (p[i].second + t[i].second) / beta));
    }
    pdf.back().second += 1 - std::accumulate(pdf.cbegin(), pdf.cend(), 0.0, sum);
}

const std::vector<std::pair<unsigned, double>>&
RobustSolitonDistribution::getpdf()
{
    return pdf;
}
