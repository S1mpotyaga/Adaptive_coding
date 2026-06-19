#include "signal_generator.hpp"

#include <cmath>
#include <numbers>
#include <random>
#include <stdexcept>
#include <ctime>

namespace adaptive{

ContinuousFunc generator_constant(double value) {
    return [=](double /*t*/) {
        return value;
    };
}

ContinuousFunc generator_inc(double start, double k) {
    return [=](double t) {
        return start + k * t;
    };
}

ContinuousFunc generator_dec(double start, double k) {
    return [=](double t) {
        return start - k * t;
    };
}

ContinuousFunc generator_sin(double amplitude, double frequency, double phase) {
    return [=](double t) {
        return amplitude * std::sin(2.0 * M_PI * frequency * t + phase);
    };
}

ContinuousFunc generator_random(double start, double strength) {
    return [=](double t) mutable {
        static std::mt19937 gen(std::random_device{}());
        static std::normal_distribution<double> dist(0.0, 1.0);

        return start + strength * dist(gen) * t;
    };
}

} // namespace adaptive