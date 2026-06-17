#pragma once

#include <cstdint>

#include "data/observer_config.hpp"

namespace adaptive{

class SignalObserver{
public:
    explicit SignalObserver(const ObserverConfig& a_config);

    void update(std::int8_t code);

    [[nodiscard]] double value() const noexcept;

    [[nodiscard]] double velocity() const noexcept;

private:
    ObserverConfig m_config;
    double m_estimated_y;
    double m_estimated_v;
};

} //namespace adaptive