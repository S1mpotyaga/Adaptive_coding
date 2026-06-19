#include "metrics.hpp"

#include <stdexcept>

namespace adaptive{

std::size_t Metrics::encoded_image_size_bytes(const EncodedImage& image) {
    std::size_t total_size = 0;
    for (const EncodedData& row : image.rows) {
        total_size += sizeof(row.initial_value);
        total_size += row.codes.size() * sizeof(std::int8_t);
    }
    return total_size;
}

std::size_t Metrics::encoded_color_image_size_bytes(const EncodedColorImage& image) {
    return encoded_image_size_bytes(image.blue) + encoded_image_size_bytes(image.green) + encoded_image_size_bytes(image.red);
}

double Metrics::calc_mse(const ColorImageMatrices& original, const ColorImageMatrices& decoded){
    if (original.blue.empty()) {
        if (decoded.blue.empty()) {
            return 0.0;
        }
        throw std::invalid_argument("Original and decoded images have different dimensions");
    }
    const std::size_t height = original.blue.size();
    if (original.green.size() != height ||
        original.red.size() != height ||
        decoded.blue.size() != height ||
        decoded.green.size() != height ||
        decoded.red.size() != height)
    {
        throw std::invalid_argument("Image channels have different heights");
    }
    long double squared_error_sum = 0.0L;
    std::size_t sample_count = 0;
    for (std::size_t row = 0; row < height; ++row) {
        const std::size_t width = original.blue[row].size();
        if (original.green[row].size() != width ||
            original.red[row].size() != width ||
            decoded.blue[row].size() != width ||
            decoded.green[row].size() != width ||
            decoded.red[row].size() != width)
        {
            throw std::invalid_argument("Image channels have different row widths");
        }
        for (std::size_t column = 0; column < width; ++column){
            const long double red_error = static_cast<long double>(original.red[row][column]) - static_cast<long double>(decoded.red[row][column]);
            const long double green_error = static_cast<long double>(original.green[row][column]) - static_cast<long double>(decoded.green[row][column]);
            const long double blue_error = static_cast<long double>(original.blue[row][column]) - static_cast<long double>(decoded.blue[row][column]);
            squared_error_sum += red_error * red_error + green_error * green_error + blue_error * blue_error;
            sample_count += 3;
        }
    }
    if (sample_count == 0) {
        return 0.0;
    }
    return static_cast<double>(squared_error_sum / static_cast<long double>(sample_count));
}


} // namespace adaptive