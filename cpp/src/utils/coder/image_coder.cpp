#include "image_coder.hpp"
#include "coder.hpp"

#include <stdexcept>

namespace adaptive{

ImageCoder::ImageCoder(
    const AdaptiveConfig& a_adaptive_config,
    const ObserverConfig& a_observer_config)
    : m_adaptive_config(a_adaptive_config),
    m_observer_config(a_observer_config){}

EncodedImage ImageCoder::encode_channel_zooming(const ImageMatrix& channel) const {
    EncodedImage encoded_channel;
    if (channel.empty()) {
        return encoded_channel;
    }
    if (channel.front().empty()) {
        throw std::invalid_argument("Image channel rows cannot be empty");
    }
    const std::size_t height = channel.size();
    const std::size_t width = channel.front().size();
    for (const auto& row : channel) {
        if (row.size() != width) {
            throw std::invalid_argument("All image channel rows must have equal width");
        }
    }
    encoded_channel.width = width;
    encoded_channel.height = height;
    encoded_channel.rows.reserve(height);
    ObserverConfig observer_config = m_observer_config;
    observer_config.T = 1.0;
    Coder row_coder(m_adaptive_config, observer_config);
    for (const auto& row : channel) {
        encoded_channel.rows.push_back(row_coder.encode_zooming(row));
    }
    return encoded_channel;
}

EncodedImage ImageCoder::encode_channel_fixed(const ImageMatrix& channel) const {
    EncodedImage encoded_channel;
    if (channel.empty()) {
        return encoded_channel;
    }
    if (channel.front().empty()) {
        throw std::invalid_argument("Image channel rows cannot be empty");
    }
    const std::size_t height = channel.size();
    const std::size_t width = channel.front().size();
    for (const auto& row : channel) {
        if (row.size() != width) {
            throw std::invalid_argument("All image channel rows must have equal width");
        }
    }
    encoded_channel.width = width;
    encoded_channel.height = height;
    encoded_channel.rows.reserve(height);
    ObserverConfig observer_config = m_observer_config;
    observer_config.T = 1.0;
    Coder row_coder(m_adaptive_config, observer_config);
    for (const auto& row : channel) {
        encoded_channel.rows.push_back(row_coder.encode_fixed(row));
    }
    return encoded_channel;
}

EncodedImage ImageCoder::encode_channel_grow_factor(const ImageMatrix& channel) const {
    EncodedImage encoded_channel;
    if (channel.empty()) {
        return encoded_channel;
    }
    if (channel.front().empty()) {
        throw std::invalid_argument("Image channel rows cannot be empty");
    }
    const std::size_t height = channel.size();
    const std::size_t width = channel.front().size();
    for (const auto& row : channel) {
        if (row.size() != width) {
            throw std::invalid_argument("All image channel rows must have equal width");
        }
    }
    encoded_channel.width = width;
    encoded_channel.height = height;
    encoded_channel.rows.reserve(height);
    ObserverConfig observer_config = m_observer_config;
    observer_config.T = 1.0;
    Coder row_coder(m_adaptive_config, observer_config);
    for (const auto& row : channel) {
        encoded_channel.rows.push_back(row_coder.encode_grow_factor(row));
    }
    return encoded_channel;
}

EncodedColorImage ImageCoder::encode_zooming(const ColorImageMatrices& image) const {
    EncodedColorImage encoded_image;
    const bool blue_empty = image.blue.empty();
    const bool green_empty = image.green.empty();
    const bool red_empty = image.red.empty();
    if (blue_empty && green_empty && red_empty) {
        return encoded_image;
    }
    if (blue_empty || green_empty || red_empty) {
        throw std::invalid_argument("All color channels must be present");
    }
    const std::size_t height = image.blue.size();
    if (image.green.size() != height || image.red.size() != height) {
        throw std::invalid_argument("All color channels must have equal height");
    }
    if (image.blue.front().empty() || image.green.front().empty() || image.red.front().empty()) {
        throw std::invalid_argument("Color channel rows cannot be empty");
    }
    const std::size_t width = image.blue.front().size();
    for (std::size_t row = 0; row < height; ++row) {
        if (image.blue[row].size() != width || image.green[row].size() != width || image.red[row].size() != width) {
            throw std::invalid_argument("All color channels must have equal dimensions");
        }
    }
    encoded_image.blue = encode_channel_zooming(image.blue);
    encoded_image.green = encode_channel_zooming(image.green);
    encoded_image.red = encode_channel_zooming(image.red);
    return encoded_image;
}

EncodedColorImage ImageCoder::encode_fixed(const ColorImageMatrices& image) const {
    EncodedColorImage encoded_image;
    const bool blue_empty = image.blue.empty();
    const bool green_empty = image.green.empty();
    const bool red_empty = image.red.empty();
    if (blue_empty && green_empty && red_empty) {
        return encoded_image;
    }
    if (blue_empty || green_empty || red_empty) {
        throw std::invalid_argument("All color channels must be present");
    }
    const std::size_t height = image.blue.size();
    if (image.green.size() != height || image.red.size() != height) {
        throw std::invalid_argument("All color channels must have equal height");
    }
    if (image.blue.front().empty() || image.green.front().empty() || image.red.front().empty()) {
        throw std::invalid_argument("Color channel rows cannot be empty");
    }
    const std::size_t width = image.blue.front().size();
    for (std::size_t row = 0; row < height; ++row) {
        if (image.blue[row].size() != width || image.green[row].size() != width || image.red[row].size() != width) {
            throw std::invalid_argument("All color channels must have equal dimensions");
        }
    }
    encoded_image.blue = encode_channel_fixed(image.blue);
    encoded_image.green = encode_channel_fixed(image.green);
    encoded_image.red = encode_channel_fixed(image.red);
    return encoded_image;
}

EncodedColorImage ImageCoder::encode_grow_factor(const ColorImageMatrices& image) const {
    EncodedColorImage encoded_image;
    const bool blue_empty = image.blue.empty();
    const bool green_empty = image.green.empty();
    const bool red_empty = image.red.empty();
    if (blue_empty && green_empty && red_empty) {
        return encoded_image;
    }
    if (blue_empty || green_empty || red_empty) {
        throw std::invalid_argument("All color channels must be present");
    }
    const std::size_t height = image.blue.size();
    if (image.green.size() != height || image.red.size() != height) {
        throw std::invalid_argument("All color channels must have equal height");
    }
    if (image.blue.front().empty() || image.green.front().empty() || image.red.front().empty()) {
        throw std::invalid_argument("Color channel rows cannot be empty");
    }
    const std::size_t width = image.blue.front().size();
    for (std::size_t row = 0; row < height; ++row) {
        if (image.blue[row].size() != width || image.green[row].size() != width || image.red[row].size() != width) {
            throw std::invalid_argument("All color channels must have equal dimensions");
        }
    }
    encoded_image.blue = encode_channel_grow_factor(image.blue);
    encoded_image.green = encode_channel_grow_factor(image.green);
    encoded_image.red = encode_channel_grow_factor(image.red);
    return encoded_image;
}


} // namespace adaptive