#include "coder.hpp"
#include "../adaptive_step.hpp"
#include "observer/observer.hpp"

namespace adaptive{

Coder::Coder(const AdaptiveConfig& a_config): m_adaptive_config(a_config){};

Coder::Coder(
    const AdaptiveConfig& a_adaptive_config,
    const ObserverConfig& a_observer_config)
    : m_adaptive_config(a_adaptive_config),
    m_observer_config(a_observer_config){}

void Coder::append_code(EncodedData& encoded, bool positive){
    const std::size_t bit_index = encoded.code_count;
    const std::size_t byte_index = bit_index / 8;
    const std::size_t bit_offset = 7 - (bit_index % 8);
    if (byte_index == encoded.codes.size()) {
        encoded.codes.push_back(0);
    }
    if (positive) {
        encoded.codes[byte_index] |= static_cast<std::uint8_t>(1U << bit_offset);
    }
    ++encoded.code_count;
}

[[nodiscard]] EncodedData Coder::encode_grow_factor(const std::vector<double>& signal) const{
    EncodedData result;
    if (signal.empty()) {
        return result;
    }
    result.initial_value = signal.front();
    const std::size_t code_count = signal.size() - 1;
    const std::size_t byte_count = (code_count + 7) / 8;
    result.codes.reserve(byte_count);
    double current_value = signal.front();
    AdaptiveStep step(m_adaptive_config);
    for (std::size_t index = 1; index < signal.size(); ++index){
        const double error = signal[index] - current_value;
        const bool positive = error >= 0.0;
        const std::int8_t code = positive ? std::int8_t{1} : std::int8_t{-1};
        append_code(result, positive);
        current_value += static_cast<double>(code) * step.value();
        step.update_grow_factor(code);
    }
    return result;
}

[[nodiscard]] EncodedData Coder::encode_fixed(const std::vector<double>& signal) const{
    EncodedData result;
    if (signal.empty()) {
        return result;
    }
    result.initial_value = signal.front();
    const std::size_t code_count = signal.size() - 1;
    const std::size_t byte_count = (code_count + 7) / 8;
    result.codes.reserve(byte_count);
    double current_value = signal.front();
    AdaptiveStep step(m_adaptive_config);
    for (std::size_t index = 1; index < signal.size(); ++index){
        const double error = signal[index] - current_value;
        const bool positive = error >= 0.0;
        const std::int8_t code = positive ? std::int8_t{1} : std::int8_t{-1};
        append_code(result, positive);
        current_value += static_cast<double>(code) * step.value();
        step.update_fixed(code);
    }
    return result;
}

[[nodiscard]] EncodedData Coder::encode_zooming(const std::vector<double>& signal) const{
    EncodedData encoded;
    if (signal.empty()){
        return encoded;
    }
    encoded.initial_value = signal.front();
    const std::size_t code_count = signal.size() - 1;
    const std::size_t byte_count = (code_count + 7) / 8;
    encoded.codes.reserve(byte_count);
    ObserverConfig observer_config = m_observer_config;
    observer_config.initial_y = encoded.initial_value;
    SignalObserver observer(observer_config);
    AdaptiveStep adaptive_step(m_adaptive_config);
    for (std::size_t index = 1; index < signal.size(); ++index) {
        const double error = signal[index] - observer.value();
        const bool positive = error >= 0.0;
        append_code(encoded, positive);
        const std::int8_t code = positive ? std::int8_t{1} : std::int8_t{-1};
        observer.update(code);
        adaptive_step.update_zooming(code);
    }
    return encoded;
}


} // namespace adaptive