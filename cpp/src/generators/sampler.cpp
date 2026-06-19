#include "sampler.hpp"

namespace adaptive{

SampledSignal sample(const ContinuousFunc& f,
                     double duration,
                     double Fs) {
    SampledSignal out;

    double dt = 1.0 / Fs;

    for (double t = 0; t < duration; t += dt) {
        out.t.push_back(t);
        out.x.push_back(f(t));
    }

    return out;
}

} // namespace adaptive