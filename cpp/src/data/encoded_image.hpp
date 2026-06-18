#pragma once

#include "encoded_data.hpp"

namespace adaptive{

using ImageMatrix = std::vector<std::vector<double>>;

struct ColorImageMatrices {
    ImageMatrix blue;
    ImageMatrix green;
    ImageMatrix red;
};

struct EncodedImage{
    std::size_t width = 0;
    std::size_t height = 0;
    std::vector<EncodedData> rows;
};

struct EncodedColorImage {
    EncodedImage blue;
    EncodedImage green;
    EncodedImage red;
};

} // namespace adaptive