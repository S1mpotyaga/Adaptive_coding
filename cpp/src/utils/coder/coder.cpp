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

[[nodiscard]] EncodedData Coder::encode_grow_factor(const std::vector<double>& signal) const{    
    EncodedData result;
    if (signal.empty()){
        return result;
    }
    result.initial_value = signal.front();
    result.codes.reserve((int)signal.size() - 1);
    double cur = signal.front();
    AdaptiveStep step(m_adaptive_config);
    for (std::size_t i = 1; i < signal.size(); ++i){
        const double error = signal[i] - cur;
        const std::int8_t code = (error >= 0.0 ? 1 : -1);
        cur += static_cast<double>(code) * step.value();
        result.codes.push_back(code);
        step.update_grow_factor(code);
    }
    return result;
}

[[nodiscard]] EncodedData Coder::encode_fixed(const std::vector<double>& signal) const{    
    EncodedData result;
    if (signal.empty()){
        return result;
    }
    result.initial_value = signal.front();
    result.codes.reserve((int)signal.size() - 1);
    double cur = signal.front();
    AdaptiveStep step(m_adaptive_config);
    for (std::size_t i = 1; i < signal.size(); ++i){
        const double error = signal[i] - cur;
        const std::int8_t code = (error >= 0.0 ? 1 : -1);
        cur += static_cast<double>(code) * step.value();
        result.codes.push_back(code);
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
    encoded.codes.reserve(signal.size() - 1);
    ObserverConfig observer_config = m_observer_config;
    observer_config.initial_y = encoded.initial_value;
    SignalObserver observer(observer_config);
    AdaptiveStep adaptive_step(m_adaptive_config);
    for (std::size_t index = 1; index < signal.size(); ++index) {
        const double error = signal[index] - observer.value();
        const std::int8_t code = error >= 0.0 ? std::int8_t{1} : std::int8_t{-1};
        encoded.codes.push_back(code);
        // const double quantized_error = adaptive_step.value() * static_cast<double>(code);
        observer.update(code);
        adaptive_step.update_zooming(code);
    }
    return encoded;
}


} // namespace adaptive