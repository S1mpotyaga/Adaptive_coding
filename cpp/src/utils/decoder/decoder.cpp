#include "decoder.hpp"
#include "../adaptive_step.hpp"

namespace adaptive{

Decoder::Decoder(const AdaptiveConfig& a_config): m_config(a_config){}

[[nodiscard]] std::vector<double> Decoder::decode(const EncodedData& data) const{
    std::vector<double> result;
    result.reserve(data.codes.size() + 1);
    double cur = data.initial_value;
    result.push_back(cur);
    AdaptiveStep step(m_config);
    for (const std::int8_t code: data.codes){
        cur += static_cast<double>(code) * step.value();
        result.push_back(cur);
        step.update(code);
    }
    return result;
}

} // namespace adaptive