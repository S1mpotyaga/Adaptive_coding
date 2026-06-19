#pragma once

#include "../../data/adaptive_config.hpp"
#include "../../data/encoded_data.hpp"
#include "observer/observer.hpp"

#include <vector>

namespace adaptive{

class Decoder{
public:
    explicit Decoder(const AdaptiveConfig& a_adaptive_config);

    explicit Decoder(const AdaptiveConfig& a_adaptive_config, const ObserverConfig& m_observer_config);

    [[nodiscard]] std::vector<double> decode_grow_factor(const EncodedData& data) const;

    [[nodiscard]] std::vector<double> decode_fixed(const EncodedData& data) const;

    [[nodiscard]] std::vector<double> decode_zooming(const EncodedData& data) const;

private:
    [[nodiscard]] static std::int8_t get_code(const EncodedData& data, std::size_t index);

    AdaptiveConfig m_adaptive_config;
    ObserverConfig m_observer_config;
};

} // namespace adaptive