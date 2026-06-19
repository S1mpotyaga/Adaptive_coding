#include "observer.hpp"

#include <cstdint>
#include <stdexcept>

namespace adaptive{

SignalObserver::SignalObserver(const ObserverConfig& a_config): 
    m_config(a_config),
    m_estimated_y(a_config.initial_y),
    m_estimated_v(a_config.initial_v){
    if (m_config.T <= 0.0) {
        throw std::invalid_argument(
            "Sample interval must be positive"
        );
    }

    if (m_config.l2 < 0.0 || m_config.l1 < 0.0) {
        throw std::invalid_argument(
            "Parametrs l1 and l2 cannot be negative"
        );
    }

}

void SignalObserver::update(std::int8_t code){
    if (code != 1 && code != -1){
        throw std::invalid_argument("Code must be 1 or -1");
    }
    const double previous_y = m_estimated_y;
    const double previous_v = m_estimated_v;
    const double sigma = static_cast<double>(code);

    m_estimated_y = previous_y + m_config.T * previous_v + m_config.l1 * sigma;
    m_estimated_v = previous_v + m_config.l2 * sigma;
}

[[nodiscard]] double SignalObserver::value() const noexcept{
    return m_estimated_y;
}

[[nodiscard]] double SignalObserver::velocity() const noexcept{
    return m_estimated_v;
}

} // namespace adaptive