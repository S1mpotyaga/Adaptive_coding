#pragma once

#include "encoded_image.hpp"

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <filesystem>
#include <stdexcept>

namespace adaptive{

ColorImageMatrices load_color_image(const std::filesystem::path& image_path);

void save_color_image(const ColorImageMatrices& image, const std::filesystem::path& output_path);

} // namespace adaptive