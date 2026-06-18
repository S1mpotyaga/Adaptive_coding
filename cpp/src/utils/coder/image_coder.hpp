#pragma once

#include "data/encoded_image.hpp"
#include "data/adaptive_config.hpp"
#include "data/observer_config.hpp"
#include "data/image_io.hpp"

#include <vector>

namespace adaptive{

class ImageCoder{
public:
    ImageCoder(const AdaptiveConfig& a_adaptive_config, const ObserverConfig& a_observer_config);

    [[nodiscard]] EncodedColorImage encode_zooming(const ColorImageMatrices& image) const;

    [[nodiscard]] EncodedColorImage encode_fixed(const ColorImageMatrices& image) const;

    [[nodiscard]] EncodedColorImage encode_grow_factor(const ColorImageMatrices& image) const;

private:
    EncodedImage encode_channel_zooming(const ImageMatrix& channel) const;

    EncodedImage encode_channel_fixed(const ImageMatrix& channel) const;

    EncodedImage encode_channel_grow_factor(const ImageMatrix& channel) const;

    AdaptiveConfig m_adaptive_config;
    ObserverConfig m_observer_config;
};

} // namespace adaptive