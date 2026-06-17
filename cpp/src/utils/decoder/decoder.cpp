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

[[nodiscard]] std::vector<double> Decoder::decode_grow_factor(const EncodedData& data) const{
    std::vector<double> result;
    result.reserve(data.codes.size() + 1);
    double cur = data.initial_value;
    result.push_back(cur);
    AdaptiveStep step(m_adaptive_config);
    for (const std::int8_t code: data.codes){
        cur += static_cast<double>(code) * step.value();
        result.push_back(cur);
        step.update_grow_factor(code);
    }
    return result;
}

[[nodiscard]] std::vector<double> Decoder::decode_fixed(const EncodedData& data) const{
    std::vector<double> result;
    result.reserve(data.codes.size() + 1);
    double cur = data.initial_value;
    result.push_back(cur);
    AdaptiveStep step(m_adaptive_config);
    for (const std::int8_t code: data.codes){
        cur += static_cast<double>(code) * step.value();
        result.push_back(cur);
        step.update_fixed(code);
    }
    return result;
}

[[nodiscard]] std::vector<double> Decoder::decode_zooming(const EncodedData& data) const{
    std::vector<double> decoded_signal;
    decoded_signal.reserve(data.codes.size() + 1);
    ObserverConfig observer_config = m_observer_config;
    observer_config.initial_y = data.initial_value;
    SignalObserver observer(observer_config);
    AdaptiveStep adaptive_step(m_adaptive_config);

    decoded_signal.push_back(data.initial_value);
    for (const std::int8_t code : data.codes) {
        if (code != 1 && code != -1) {
            throw std::invalid_argument("Code must be 1 or -1");
        }
        const double sigma = static_cast<double>(code);
        const double orig_value = observer.value() + adaptive_step.value() * sigma;
        decoded_signal.push_back(orig_value);
        observer.update(code);
        adaptive_step.update_zooming(code);
    }
    return decoded_signal;
}


} // namespace adaptive