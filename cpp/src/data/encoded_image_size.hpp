#pragma once

#include "encoded_image.hpp"

namespace adaptive{

std::size_t encoded_image_size_bytes(const EncodedImage& image) {
    std::size_t total_size = 0;
    for (const EncodedData& row : image.rows) {
        total_size += sizeof(row.initial_value);
        total_size += row.codes.size() * sizeof(std::int8_t);
    }
    return total_size;
}

std::size_t encoded_color_image_size_bytes(const EncodedColorImage& image) {
    return encoded_image_size_bytes(image.blue) + encoded_image_size_bytes(image.green) + encoded_image_size_bytes(image.red);
}

} // namespace adaptive