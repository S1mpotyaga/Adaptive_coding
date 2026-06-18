#pragma once

#include "data/adaptive_config.hpp"
#include "data/encoded_image.hpp"
#include "data/observer_config.hpp"
#include "data/image_io.hpp"

#include <vector>

namespace adaptive{

using GrayImage = std::vector<std::vector<double>>;

class ImageDecoder {
public:
    ImageDecoder(const AdaptiveConfig& a_adaptive_config, const ObserverConfig& a_observer_config);

    [[nodiscard]] ColorImageMatrices decode_zooming(const EncodedColorImage& image) const;

    [[nodiscard]] ColorImageMatrices decode_fixed(const EncodedColorImage& image) const;

    [[nodiscard]] ColorImageMatrices decode_grow_factor(const EncodedColorImage& image) const;


private:
    ImageMatrix decode_channel_zooming(const EncodedImage& image) const;

    ImageMatrix decode_channel_fixed(const EncodedImage& image) const;

    ImageMatrix decode_channel_grow_factor(const EncodedImage& image) const;

    AdaptiveConfig m_adaptive_config;
    ObserverConfig m_observer_config;
};


} // namespace adaptive