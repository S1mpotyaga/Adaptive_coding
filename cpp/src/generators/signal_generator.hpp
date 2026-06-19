#pragma once

#include <cstddef>
#include <vector>
#include <functional>

namespace adaptive{

using ContinuousFunc = std::function<double(double)>;

ContinuousFunc generator_constant(double value);

ContinuousFunc generator_inc(double start, double k);

ContinuousFunc generator_dec(double start, double k);

ContinuousFunc generator_sin(double amplitude, double frequency, double phase = 0.0);

ContinuousFunc generator_random(double start, double strength);

} // namespace adaptive