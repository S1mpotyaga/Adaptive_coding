#pragma once

#include <vector>
#include <functional>

namespace adaptive{

using ContinuousFunc = std::function<double(double)>;

struct SampledSignal {
    std::vector<double> t;
    std::vector<double> x;
};

SampledSignal sample(const ContinuousFunc& f,
                     double duration,
                     double Fs);
}