#pragma once

#include <vector>
#include <cstdint>

namespace adaptive{

struct EncodedData{
    double initial_value = 0.0;
    std::size_t code_count = 0;
    std::vector<std::int8_t> codes;
};

} // namespace adaptive