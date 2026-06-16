#include "coder.hpp"
#include "../adaptive_step.hpp"

namespace adaptive{

Coder::Coder(const AdaptiveConfig& a_config): m_config(a_config){};

[[nodiscard]] EncodedData Coder::encode(const std::vector<double>& signal) const{    
    if (signal.empty()){
        return {};
    }
    EncodedData result;
    result.initial_value = signal.front();
    result.codes.reserve((int)signal.size() - 1);
    double cur = signal.front();
    AdaptiveStep step(m_config);
    for (std::size_t i = 1; i < signal.size(); ++i){
        const double error = signal[i] - cur;
        const std::int8_t code = (error >= 0.0 ? 1 : -1);
        cur += static_cast<double>(code) * step.value();
        result.codes.push_back(code);
        step.update(code);
    }
    return result;
}

} // namespace adaptive