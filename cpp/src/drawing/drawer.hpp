#pragma once

#include "drawing/drawer.hpp"
#include "utils/decoder/decoder.hpp"
#include "utils/coder/coder.hpp"
#include "generators/sampler.hpp"

#include <filesystem>
#include <vector>
#include <matplot/matplot.h>
#include <string>

namespace adaptive{

class Drawer{
public:
    void draw(const SampledSignal& original_values, const std::string& type);

private:
    void save_signal_comparison(
        const std::filesystem::path& filename,
        const std::string& graph_title,
        const std::vector<double>& time,
        const std::vector<double>& original_signal,
        const std::vector<double>& decoded_signal
    );

    const std::filesystem::path m_output_directory =
        std::filesystem::path(PROJECT_ROOT) /
        "images" /
        "one-demensial";
};

} // namespace adaptive