#pragma once

#include "../../data/adaptive_config.hpp"
#include "../../data/encoded_data.hpp"
#include "data/observer_config.hpp"

#include <vector>

namespace adaptive{

class Coder{
public:
    explicit Coder(const AdaptiveConfig& a_adaptive_config);

    explicit Coder(const AdaptiveConfig& a_adaptive_config, const ObserverConfig& a_observer_config);

    [[nodiscard]] EncodedData encode_grow_factor(const std::vector<double>& signal) const;

    [[nodiscard]] EncodedData encode_fixed(const std::vector<double>& signal) const;

    [[nodiscard]] EncodedData encode_zooming(const std::vector<double>& signal) const;

private:
    AdaptiveConfig m_adaptive_config;
    ObserverConfig m_observer_config;
};

} // namespace adaptive