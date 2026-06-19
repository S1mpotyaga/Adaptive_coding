#include "decoder.hpp"
#include "../adaptive_step.hpp"

#include <stdexcept>

namespace adaptive{

Decoder::Decoder(const AdaptiveConfig& a_adaptive_config):
    m_adaptive_config(a_adaptive_config){}

Decoder::Decoder(
    const AdaptiveConfig& a_adaptive_config,
    const ObserverConfig& a_observe_config)
    : m_adaptive_config(a_adaptive_config),
    m_observer_config(a_observe_config){}

std::int8_t Decoder::get_code(const EncodedData& data, const std::size_t index){
    if (index >= data.code_count) {
        throw std::out_of_range("Encoded code index is out of range");
    }
    const std::size_t byte_index = index / 8;
    const std::size_t bit_offset = 7 - (index % 8);
    if (byte_index >= data.codes.size()) {
        throw std::invalid_argument("Packed codes contain fewer bytes than expected");
    }
    const std::uint8_t byte = data.codes[byte_index];
    const bool positive = (byte & static_cast<std::uint8_t>(std::uint8_t{1} << bit_offset)) != 0;

    return positive ? std::int8_t{1} : std::int8_t{-1};
}

[[nodiscard]] std::vector<double> Decoder::decode_grow_factor(const EncodedData& data) const{
    const std::size_t expected_byte_count = (data.code_count + 7) / 8;
    if (data.codes.size() != expected_byte_count) {
        throw std::invalid_argument("Invalid packed codes size in decode_grow_factor");
    }
    std::vector<double> result;
    result.reserve(data.code_count + 1);
    double current_value = data.initial_value;
    result.push_back(current_value);
    AdaptiveStep step(m_adaptive_config);
    for (std::size_t index = 0; index < data.code_count; ++index){
        const std::int8_t code = get_code(data, index);
        current_value += static_cast<double>(code) * step.value();
        result.push_back(current_value);
        step.update_grow_factor(code);
    }
    return result;
}

[[nodiscard]] std::vector<double> Decoder::decode_fixed(const EncodedData& data) const{
    const std::size_t expected_byte_count = (data.code_count + 7) / 8;
    if (data.codes.size() != expected_byte_count) {
        throw std::invalid_argument("Invalid packed codes size in decode_fixed");
    }
    std::vector<double> result;
    result.reserve(data.code_count + 1);
    double current_value = data.initial_value;
    result.push_back(current_value);
    AdaptiveStep step(m_adaptive_config);
    for (std::size_t index = 0; index < data.code_count; ++index) {
        const std::int8_t code = get_code(data, index);
        current_value += static_cast<double>(code) * step.value();
        result.push_back(current_value);
        step.update_fixed(code);
    }
    return result;
}

[[nodiscard]] std::vector<double> Decoder::decode_zooming(const EncodedData& data) const {
    const std::size_t expected_byte_count = (data.code_count + 7) / 8;
    if (data.codes.size() != expected_byte_count) {
        throw std::invalid_argument("Invalid packed codes size");
    }
    std::vector<double> decoded_signal;
    decoded_signal.reserve(data.code_count + 1);
    ObserverConfig observer_config = m_observer_config;
    observer_config.initial_y = data.initial_value;
    SignalObserver observer(observer_config);
    AdaptiveStep adaptive_step(m_adaptive_config);
    decoded_signal.push_back(data.initial_value);
    for (std::size_t index = 0; index < data.code_count; ++index){
        const std::int8_t code = get_code(data, index);
        const double sigma = static_cast<double>(code);
        const double original_value = observer.value() + adaptive_step.value() * sigma;
        decoded_signal.push_back(original_value);
        observer.update(code);
        adaptive_step.update_zooming(code);
    }
    return decoded_signal;
}


} // namespace adaptive