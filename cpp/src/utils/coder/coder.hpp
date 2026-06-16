#pragma once

#include "../../data/adaptive_config.hpp"
#include "../../data/encoded_data.hpp"

#include <vector>

namespace adaptive{

class Coder{
public:
    explicit Coder(const AdaptiveConfig& a_config);

    [[nodiscard]] EncodedData encode(const std::vector<double>& signal) const;

private:
    AdaptiveConfig m_config;
};

} // namespace adaptive