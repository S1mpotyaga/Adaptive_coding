#include "adaptive_step.hpp"

#include <stdexcept>
#include <algorithm>

namespace adaptive{

AdaptiveStep::AdaptiveStep(const AdaptiveConfig& a_config):
    m_config(a_config),
    m_step(a_config.initial_step){

    if (a_config.initial_step <= 0.0){
        throw std::invalid_argument("Initial step must be positive.");
    }

    if (a_config.min_step <= 0.0 || a_config.max_step < a_config.min_step){
        throw std::invalid_argument("Invalid step limits");
    }

    if (a_config.grow_factor <= 1.0){
        throw std::invalid_argument("grow_factor must be greater than 1");
    }

    if (a_config.decay_factor <= 0.0 || a_config.decay_factor > 1.0){
        throw std::invalid_argument("decay_factor must be between 0 and 1");
    }
}

void AdaptiveStep::update_fixed(const std::int8_t code){
    if (code != 1 && code != -1){
        throw std::invalid_argument("Code must be 1 or -1");
    }
    m_previous_code = code;
    m_has_previous_code = true;
}

void AdaptiveStep::update_grow_factor(const std::int8_t code){
    if (code != 1 && code != -1){
        throw std::invalid_argument("Code must be 1 or -1");
    }
    if (m_has_previous_code){
        if (code == m_previous_code){
            m_step *= m_config.grow_factor; 
        } else{
            m_step *= m_config.decay_factor;
        }
        m_step = std::clamp(m_step, m_config.min_step, m_config.max_step);
    }

    m_previous_code = code;
    m_has_previous_code = true;
}

void AdaptiveStep::update_zooming(const std::int8_t code){
    if (code != 1 && code != -1) {
        throw std::invalid_argument(
            "Code must be 1 or -1"
        );
    }
    m_code_window[m_window_position] = code;
    m_window_position = (m_window_position + 1) % WINDOW_SIZE;
    if (m_window_count < WINDOW_SIZE) {
        ++m_window_count;
    }
    if (m_window_count < WINDOW_SIZE) {
        return;
    }
    const int code_sum =
        static_cast<int>(m_code_window[0]) +
        static_cast<int>(m_code_window[1]) +
        static_cast<int>(m_code_window[2]);

    const double lambda =
        static_cast<double>(code_sum) /
        static_cast<double>(WINDOW_SIZE);

    const double next_step =
        m_config.zoom_offset +
        (std::abs(lambda) + 0.5) * m_step;

    m_step = std::clamp(next_step, m_config.min_step, m_config.max_step);
}

double AdaptiveStep::value() const noexcept{
    return m_step;
}

} // namespace adaptive