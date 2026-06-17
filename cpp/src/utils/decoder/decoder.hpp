#pragma once

#include "../../data/adaptive_config.hpp"
#include "../../data/encoded_data.hpp"

namespace adaptive{

class Decoder{
public:
    explicit Decoder(const AdaptiveConfig& a_config);

    [[nodiscard]] std::vector<double> decode_grow_factor(const EncodedData& data) const;

    [[nodiscard]] std::vector<double> decode_fixed(const EncodedData& data) const;

private:
    AdaptiveConfig m_config;
};

} // namespace adaptive