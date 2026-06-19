#include "image_decoder.hpp"
#include "decoder.hpp"

#include <algorithm>
#include <stdexcept>

namespace adaptive {

ImageDecoder::ImageDecoder(
    const AdaptiveConfig& adaptive_config,
    const ObserverConfig& observer_config)
    : m_adaptive_config(adaptive_config),
      m_observer_config(observer_config){}


ImageMatrix ImageDecoder::decode_channel_zooming(const EncodedImage& image) const {
    if (image.width == 0 && image.height == 0) {
        return {};
    }
    if (image.width == 0 || image.height == 0) {
        throw std::invalid_argument("Encoded image has invalid dimensions");
    }
    if (image.rows.size() != image.height) {
        throw std::invalid_argument("Encoded row count does not match image height");
    }
    ObserverConfig observer_config = m_observer_config;
    observer_config.T = 1.0;
    Decoder row_decoder(m_adaptive_config, observer_config);
    ImageMatrix decoded_channel;
    decoded_channel.reserve(image.height);
    for (const EncodedData& encoded_row : image.rows) {
        std::vector<double> row = row_decoder.decode_zooming(encoded_row);
        if (row.size() != image.width) {
            throw std::runtime_error("Decoded row width does not match image width");
        }
        for (double& pixel : row) {
            pixel = std::clamp(pixel, 0.0, 1.0);
        }
        decoded_channel.push_back(std::move(row));
    }
    return decoded_channel;
}

ImageMatrix ImageDecoder::decode_channel_fixed(const EncodedImage& image) const {
    if (image.width == 0 && image.height == 0) {
        return {};
    }
    if (image.width == 0 || image.height == 0) {
        throw std::invalid_argument("Encoded image has invalid dimensions");
    }
    if (image.rows.size() != image.height) {
        throw std::invalid_argument("Encoded row count does not match image height");
    }
    ObserverConfig observer_config = m_observer_config;
    observer_config.T = 1.0;
    Decoder row_decoder(m_adaptive_config, observer_config);
    ImageMatrix decoded_channel;
    decoded_channel.reserve(image.height);
    for (const EncodedData& encoded_row : image.rows) {
        std::vector<double> row = row_decoder.decode_fixed(encoded_row);
        if (row.size() != image.width) {
            throw std::runtime_error("Decoded row width does not match image width");
        }
        for (double& pixel : row) {
            pixel = std::clamp(pixel, 0.0, 1.0);
        }
        decoded_channel.push_back(std::move(row));
    }
    return decoded_channel;
}

ImageMatrix ImageDecoder::decode_channel_grow_factor(const EncodedImage& image) const {
    if (image.width == 0 && image.height == 0) {
        return {};
    }
    if (image.width == 0 || image.height == 0) {
        throw std::invalid_argument("Encoded image has invalid dimensions");
    }
    if (image.rows.size() != image.height) {
        throw std::invalid_argument("Encoded row count does not match image height");
    }
    ObserverConfig observer_config = m_observer_config;
    observer_config.T = 1.0;
    Decoder row_decoder(m_adaptive_config, observer_config);
    ImageMatrix decoded_channel;
    decoded_channel.reserve(image.height);
    for (const EncodedData& encoded_row : image.rows) {
        std::vector<double> row = row_decoder.decode_grow_factor(encoded_row);
        if (row.size() != image.width) {
            throw std::runtime_error("Decoded row width does not match image width");
        }
        for (double& pixel : row) {
            pixel = std::clamp(pixel, 0.0, 1.0);
        }
        decoded_channel.push_back(std::move(row));
    }
    return decoded_channel;
}

ColorImageMatrices ImageDecoder::decode_zooming(const EncodedColorImage& image) const {
    ColorImageMatrices decoded_image;
    const bool blue_empty = image.blue.width == 0 && image.blue.height == 0;
    const bool green_empty = image.green.width == 0 && image.green.height == 0;
    const bool red_empty = image.red.width == 0 && image.red.height == 0;

    if (blue_empty && green_empty && red_empty) {
        return decoded_image;
    }
    if (blue_empty || green_empty || red_empty) {
        throw std::invalid_argument("All encoded color channels must be present");
    }
    if (image.blue.width != image.green.width ||
        image.blue.width != image.red.width ||
        image.blue.height != image.green.height ||
        image.blue.height != image.red.height) {
        throw std::invalid_argument("Encoded color channels must have equal dimensions");
    }

    decoded_image.blue = decode_channel_zooming(image.blue);
    decoded_image.green = decode_channel_zooming(image.green);
    decoded_image.red = decode_channel_zooming(image.red);
    return decoded_image;
}

ColorImageMatrices ImageDecoder::decode_fixed(const EncodedColorImage& image) const {
    ColorImageMatrices decoded_image;
    const bool blue_empty = image.blue.width == 0 && image.blue.height == 0;
    const bool green_empty = image.green.width == 0 && image.green.height == 0;
    const bool red_empty = image.red.width == 0 && image.red.height == 0;

    if (blue_empty && green_empty && red_empty) {
        return decoded_image;
    }
    if (blue_empty || green_empty || red_empty) {
        throw std::invalid_argument("All encoded color channels must be present");
    }
    if (image.blue.width != image.green.width ||
        image.blue.width != image.red.width ||
        image.blue.height != image.green.height ||
        image.blue.height != image.red.height) {
        throw std::invalid_argument("Encoded color channels must have equal dimensions");
    }

    decoded_image.blue = decode_channel_fixed(image.blue);
    decoded_image.green = decode_channel_fixed(image.green);
    decoded_image.red = decode_channel_fixed(image.red);
    return decoded_image;
}

ColorImageMatrices ImageDecoder::decode_grow_factor(const EncodedColorImage& image) const {
    ColorImageMatrices decoded_image;
    const bool blue_empty = image.blue.width == 0 && image.blue.height == 0;
    const bool green_empty = image.green.width == 0 && image.green.height == 0;
    const bool red_empty = image.red.width == 0 && image.red.height == 0;

    if (blue_empty && green_empty && red_empty) {
        return decoded_image;
    }
    if (blue_empty || green_empty || red_empty) {
        throw std::invalid_argument("All encoded color channels must be present");
    }
    if (image.blue.width != image.green.width ||
        image.blue.width != image.red.width ||
        image.blue.height != image.green.height ||
        image.blue.height != image.red.height) {
        throw std::invalid_argument("Encoded color channels must have equal dimensions");
    }

    decoded_image.blue = decode_channel_grow_factor(image.blue);
    decoded_image.green = decode_channel_grow_factor(image.green);
    decoded_image.red = decode_channel_grow_factor(image.red);
    return decoded_image;
}

} // namespace adaptive