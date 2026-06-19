#pragma once

#include "data/encoded_image.hpp"

namespace adaptive{

class Metrics{
public:
    static std::size_t encoded_color_image_size_bytes(const EncodedColorImage& image);

    static double calc_mse(const ColorImageMatrices& original, const ColorImageMatrices& decoded);

private:
    static std::size_t encoded_image_size_bytes(const EncodedImage& image);

};

} // namespace adaptive