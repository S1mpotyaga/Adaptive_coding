#pragma once

#include <cstdint>

#include "../data/adaptive_config.hpp"

namespace adaptive{

class AdaptiveStep{
public:
    explicit AdaptiveStep(const AdaptiveConfig& a_config);

    void update_fixed(const std::int8_t code);

    void update_grow_factor(const std::int8_t code);

    double value() const noexcept;

private:
    AdaptiveConfig m_config;
    double m_step = 1.0;
    double m_previous_code;
    bool m_has_previous_code = false;
};

} // namespace adaptive