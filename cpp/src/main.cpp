#include "generators/signal_generator.hpp"
#include "generators/sampler.hpp"
#include "drawing/drawer.hpp"

#include <iostream>
#include <matplot/matplot.h>

int main(){
    using namespace adaptive;
    using namespace matplot;
    auto signal_const = generator_constant(2);
    auto signal_inc = generator_inc(3, 3);
    auto signal_dec = generator_dec(3, 2);
    auto signal_sin = generator_sin(4, 0.1);
    auto signal_random = generator_random(3, 1.1);

    const double duration = 10;
    const double fs = 20;

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
    std::cerr << "Draw one-demensial signal by grow-factor\n";
    drawer.draw_grow_factor(original_values_const, "const");
    drawer.draw_grow_factor(original_values_inc, "inc");
    drawer.draw_grow_factor(original_values_dec, "dec");
    drawer.draw_grow_factor(original_values_sin, "sin");
    drawer.draw_grow_factor(original_values_random, "random");
    std::cerr << "Drawing one-demensial signal by grow-factor is finished\n";

    std::cerr << "Draw one-demensial signal by fixed\n";
    drawer.draw_fixed(original_values_const, "const");
    drawer.draw_fixed(original_values_inc, "inc");
    drawer.draw_fixed(original_values_dec, "dec");
    drawer.draw_fixed(original_values_sin, "sin");
    drawer.draw_fixed(original_values_random, "random");
    std::cerr << "Drawing one-demensial signal by fixed is finished\n";

    std::cerr << "Draw one-demensial signal by zooming\n";
    drawer.draw_zooming(original_values_const, "const", fs);
    drawer.draw_zooming(original_values_inc, "inc", fs);
    drawer.draw_zooming(original_values_dec, "dec", fs);
    drawer.draw_zooming(original_values_sin, "sin", fs);
    drawer.draw_zooming(original_values_random, "random", fs);
    std::cerr << "Drawing one-demensial signal by zooming is finished\n";

    const std::filesystem::path image_path = 
        std::filesystem::path(PROJECT_ROOT) /
        "test_images" /
        "test_image.png";
    if (!std::filesystem::exists(image_path)) {
        throw std::runtime_error("Image file does not exist: " + image_path.string());
    }

    std::cerr << "Image: " + image_path.string() + "is reading\n";
    ColorImageMatrices original_image = load_color_image(image_path);
    std::cerr << "Image: " + image_path.string() + " readed\n";
    std::cerr << "Draw image by fixed\n";
    drawer.draw_image_fixed(original_image);
    std::cerr << "Drawing image by fixed is finished\n";
    std::cerr << "Draw image by grow-factor\n";
    drawer.draw_image_grow_factor(original_image);
    std::cerr << "Drawing image by grow-factor is finished\n";
    std::cerr << "Draw image by zooming\n";
    drawer.draw_image_zooming(original_image);
    std::cerr << "Drawing image by zooming is finished\n";
    return 0;
}