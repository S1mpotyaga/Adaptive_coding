#pragma once

#include "drawing/drawer.hpp"
#include "utils/decoder/decoder.hpp"
#include "utils/coder/coder.hpp"
#include "generators/sampler.hpp"
#include "data/encoded_image.hpp"
#include "utils/coder/image_coder.hpp"
#include "utils/decoder/image_decoder.hpp"

#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

#include <filesystem>
#include <vector>
#include <matplot/matplot.h>
#include <string>

namespace adaptive{

struct ImageDimensions {
    std::size_t height = 0;
    std::size_t width = 0;
};

class Drawer{
public:
    void draw_grow_factor(const SampledSignal& original_values, const std::string& type);

    void draw_fixed(const SampledSignal& original_values, const std::string& type);

    void draw_zooming(const SampledSignal& original_values, const std::string& type, const double fs);

    void draw_image(const ColorImageMatrices& original_values);

private:
    void save_signal_comparison(
        const std::filesystem::path& filename,
        const std::string& graph_title,
        const std::vector<double>& time,
        const std::vector<double>& original_signal,
        const std::vector<double>& decoded_signal
    );

    void save_image_comparison(
        const std::filesystem::path& filename,
        const std::string& graph_title,
        const ColorImageMatrices& original_image,
        const ColorImageMatrices& decoded_image        
    );

    cv::Mat color_matrices_to_cv_mat(const ColorImageMatrices& image);

    void draw_image_channel(const ImageMatrix& channel, const double fs);

    ImageDimensions validate_color_image(const ColorImageMatrices& image, const std::string& image_name);

    std::uint8_t normalized_to_byte(double value);

    matplot::image_channels_t to_matplot_rgb(const ColorImageMatrices& image);

    const std::filesystem::path m_output_directory_one_demensial =
        std::filesystem::path(PROJECT_ROOT) /
        "images" /
        "one-demensial";
    
    const std::filesystem::path m_output_directory_image = 
        std::filesystem::path(PROJECT_ROOT) /
        "images" / 
        "two-demensial";
};

} // namespace adaptive