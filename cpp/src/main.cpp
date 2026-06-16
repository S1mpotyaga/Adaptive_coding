#include "generators/signal_generator.hpp"
#include "generators/sampler.hpp"
#include "drawing/drawer.hpp"

#include <iostream>
#include <matplot/matplot.h>

int main()
{
    using namespace adaptive;
    using namespace matplot;
    auto signal_const = generator_constant(2);
    auto signal_inc = generator_inc(3, 3);
    auto signal_dec = generator_dec(3, 2);
    auto signal_sin = generator_sin(4, 2);
    auto signal_random = generator_random(3, 1.5);

    const double duration = 10;
    const double fs = 10;

    const SampledSignal original_values_const =
        sample(signal_const, duration, fs);
    const SampledSignal original_values_inc =
        sample(signal_inc, duration, fs);
    const SampledSignal original_values_dec =
        sample(signal_dec, duration, fs);
    const SampledSignal original_values_sin =
        sample(signal_sin, duration, fs);
    const SampledSignal original_values_random =
        sample(signal_random, duration, fs);

    Drawer drawer;
    drawer.draw(original_values_const, "const");
    drawer.draw(original_values_inc, "inc");
    drawer.draw(original_values_dec, "dec");
    drawer.draw(original_values_sin, "sin");
    drawer.draw(original_values_random, "random");
    return 0;
}