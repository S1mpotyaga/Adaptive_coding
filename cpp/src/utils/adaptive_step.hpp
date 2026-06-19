#pragma once

#include <cstdint>
#include <array>

#include "../data/adaptive_config.hpp"

namespace adaptive{

class AdaptiveStep{
public:
    explicit AdaptiveStep(const AdaptiveConfig& a_config);

    void update_fixed(const std::int8_t code);

    void update_grow_factor(const std::int8_t code);

    void update_zooming(const std::int8_t code);

    double value() const noexcept;

private:
    static constexpr std::size_t WINDOW_SIZE = 3;
    AdaptiveConfig m_config;
    double m_step = 1.0;
    double m_previous_code;
    bool m_has_previous_code = false;

    std::array<std::int8_t, WINDOW_SIZE> m_code_window{};
    std::size_t m_window_position = 0;
    std::size_t m_window_count = 0;
};

} // namespace adaptive