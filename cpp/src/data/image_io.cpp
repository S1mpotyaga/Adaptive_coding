#include "image_io.hpp"

#include <opencv2/imgcodecs.hpp>

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <filesystem>
#include <stdexcept>

namespace adaptive{

ColorImageMatrices load_color_image(const std::filesystem::path& image_path) {
    const cv::Mat image = cv::imread(image_path.string(), cv::IMREAD_COLOR);
    if (image.empty()) {
        throw std::runtime_error("Failed to load image: " + image_path.string());
    }
    const std::size_t rows = static_cast<std::size_t>(image.rows);
    const std::size_t columns = static_cast<std::size_t>(image.cols);
    ColorImageMatrices matrices{
        ImageMatrix(rows, std::vector<double>(columns)),
        ImageMatrix(rows, std::vector<double>(columns)),
        ImageMatrix(rows, std::vector<double>(columns))
    };

    for (int row = 0; row < image.rows; ++row) {
        const auto* source_row = image.ptr<cv::Vec3b>(row);
        for (int column = 0; column < image.cols; ++column) {
            const cv::Vec3b& pixel = source_row[column];
            const std::size_t r = static_cast<std::size_t>(row);
            const std::size_t c = static_cast<std::size_t>(column);
            matrices.blue[r][c] = static_cast<double>(pixel[0]) / 255.0;
            matrices.green[r][c] = static_cast<double>(pixel[1]) / 255.0;
            matrices.red[r][c] = static_cast<double>(pixel[2]) / 255.0;
        }
    }
    return matrices;
}

void save_color_image(const ColorImageMatrices& image, const std::filesystem::path& output_path) {
    if (image.blue.empty() ||
        image.green.empty() ||
        image.red.empty() ||
        image.blue.front().empty() ||
        image.green.front().empty() ||
        image.red.front().empty()) {
        throw std::invalid_argument("Image matrices cannot be empty");
    }

    const std::size_t height = image.blue.size();
    const std::size_t width = image.blue.front().size();
    if (image.green.size() != height || image.red.size() != height) {
        throw std::invalid_argument("All color channels must have equal height");
    }
    cv::Mat output(static_cast<int>(height), static_cast<int>(width), CV_8UC3);
    for (std::size_t row = 0; row < height; ++row) {
        if (image.blue[row].size() != width || image.green[row].size() != width || image.red[row].size() != width) {
            throw std::invalid_argument("All color channel rows must have equal width");
        }
        auto* destination_row = output.ptr<cv::Vec3b>(static_cast<int>(row));
        for (std::size_t column = 0; column < width; ++column) {
            const double blue = std::clamp(image.blue[row][column], 0.0, 1.0);
            const double green = std::clamp(image.green[row][column], 0.0, 1.0);
            const double red = std::clamp(image.red[row][column], 0.0, 1.0);
            destination_row[column][0] = static_cast<std::uint8_t>(std::lround(blue * 255.0));
            destination_row[column][1] = static_cast<std::uint8_t>(std::lround(green * 255.0));
            destination_row[column][2] = static_cast<std::uint8_t>(std::lround(red * 255.0));
        }
    }

    if (!cv::imwrite(output_path.string(), output)) {
        throw std::runtime_error("Failed to save image: " + output_path.string());
    }
}

} // namespace adaptive