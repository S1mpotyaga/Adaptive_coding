#include "drawer.hpp"
#include "metrics/metrics.hpp"

namespace adaptive{

cv::Mat Drawer::color_matrices_to_cv_mat(
        const ColorImageMatrices& image
) {
    const ImageDimensions dimensions = validate_color_image(image, "Image");
    cv::Mat result(static_cast<int>(dimensions.height), static_cast<int>(dimensions.width), CV_8UC3);

    const auto to_byte = [](double value) {
        return static_cast<std::uint8_t>(
            std::lround(std::clamp(value, 0.0, 1.0) * 255.0)
        );
    };

    for (std::size_t row = 0; row < dimensions.height; ++row) {
        auto* destination = result.ptr<cv::Vec3b>(static_cast<int>(row));

        for (std::size_t column = 0; column < dimensions.width; ++column) {

            destination[column] = cv::Vec3b{
                to_byte(image.blue[row][column]),
                to_byte(image.green[row][column]),
                to_byte(image.red[row][column])
            };
        }
    }
    return result;
}

void Drawer::save_signal_comparison(
        const std::filesystem::path& filename,
        const std::string& graph_title,
        const std::vector<double>& time,
        const std::vector<double>& original_signal,
        const std::vector<double>& decoded_signal
    ) {
    if (time.size() != original_signal.size()) {
        throw std::invalid_argument(
            "Размеры time и original_signal не совпадают."
        );
    }

    if (original_signal.size() != decoded_signal.size()) {
        throw std::invalid_argument(
            "Размеры original_signal и decoded_signal не совпадают"
        );
    }

    auto fig = matplot::figure(true);
    fig->size(1200, 800);

    auto ax = fig->current_axes();

    auto original_line = matplot::plot(ax, time, original_signal);

    original_line->line_width(2).display_name("Исходный сигнал");

    matplot::hold(ax, matplot::on);

    auto decoded_line = matplot::plot(ax, time, decoded_signal, "--");

    decoded_line->line_width(2).display_name("Раскодированный сигнал");

    matplot::hold(ax, matplot::off);

    matplot::title(ax, graph_title);
    matplot::xlabel(ax, "t");
    matplot::ylabel(ax, "x(t)");
    matplot::grid(ax, true);

    auto graph_legend = matplot::legend(ax, {});

    graph_legend->location(
        matplot::legend::general_alignment::topright
    );

    if (!filename.parent_path().empty()) {
        std::filesystem::create_directories(
            filename.parent_path()
        );
    }

    fig->save(filename.string());
}

void Drawer::save_image_comparison(
    const std::filesystem::path& filename,
    const std::string& graph_title,
    const ColorImageMatrices& original_image,
    const ColorImageMatrices& decoded_image
) {
    const ImageDimensions original_dimensions = validate_color_image(original_image, "Original image");
    const ImageDimensions decoded_dimensions = validate_color_image(decoded_image, "Decoded image");
    if (original_dimensions.height != decoded_dimensions.height ||
        original_dimensions.width != decoded_dimensions.width) {
        throw std::invalid_argument("Original and decoded images must have equal dimensions");
    }
    if (!filename.parent_path().empty()) {
        std::filesystem::create_directories(filename.parent_path());
    }
    cv::Mat original = color_matrices_to_cv_mat(original_image);
    cv::Mat decoded = color_matrices_to_cv_mat(decoded_image);
    constexpr int max_panel_width = 800;
    constexpr int header_height = 70;
    constexpr int gap = 30;
    constexpr int margin = 30;
    if (original.cols > max_panel_width) {
        const double scale = static_cast<double>(max_panel_width) / static_cast<double>(original.cols);
        cv::resize(
            original,
            original,
            cv::Size{},
            scale,
            scale,
            cv::INTER_AREA
        );
        cv::resize(
            decoded,
            decoded,
            original.size(),
            0.0,
            0.0,
            cv::INTER_AREA
        );
    }

    const int canvas_width = margin + original.cols + gap + decoded.cols + margin;
    const int canvas_height = header_height + original.rows + margin;
    cv::Mat canvas(canvas_height, canvas_width, CV_8UC3, cv::Scalar(245, 245, 245)    );
    const cv::Rect original_area(margin, header_height, original.cols, original.rows);
    const cv::Rect decoded_area(margin + original.cols + gap, header_height, decoded.cols, decoded.rows);
    original.copyTo(canvas(original_area));
    decoded.copyTo(canvas(decoded_area));
    cv::putText(
        canvas,
        "Original image",
        cv::Point(margin, 45),
        cv::FONT_HERSHEY_SIMPLEX,
        1.0,
        cv::Scalar(30, 30, 30),
        2,
        cv::LINE_AA
    );
    cv::putText(
        canvas,
        "Decoded image",
        cv::Point(margin + original.cols + gap, 45),
        cv::FONT_HERSHEY_SIMPLEX,
        1.0,
        cv::Scalar(30, 30, 30),
        2,
        cv::LINE_AA
    );

    std::filesystem::path output_path = filename;
    output_path.replace_extension(".png");
    if (!cv::imwrite(output_path.string(), canvas)) {
        throw std::runtime_error("Failed to save comparison image: " + output_path.string());
    }
    std::cout << "Comparison saved: " << output_path << '\n';
}

void Drawer::draw_grow_factor(const SampledSignal& original_values, const std::string& type) {
    AdaptiveConfig config;
    config.initial_step = 1.0;
    config.min_step = 0.1;
    config.max_step = 100.0;
    config.grow_factor = 1.2;
    config.decay_factor = 0.8;

    std::filesystem::path output_directory = m_output_directory_one_demensial / "grow_factor";

    std::filesystem::create_directories(output_directory);

    Coder coder(config);
    Decoder decoder(config);

    const auto encoded_values =
        coder.encode_grow_factor(original_values.x);

    const auto decoded_values =
        decoder.decode_grow_factor(encoded_values);

    const std::string filename =
        type + "_comparison.jpg";

    const std::string title =
        "Сравнение исходного и раскодированного сигнала: " + type;

    save_signal_comparison(
        output_directory / filename,
        title,
        original_values.t,
        original_values.x,
        decoded_values
    );
}

void Drawer::draw_fixed(const SampledSignal& original_values, const std::string& type) {
    AdaptiveConfig config;
    config.initial_step = 1.0;
    config.min_step = 0.1;
    config.max_step = 100.0;
    config.grow_factor = 1.2;
    config.decay_factor = 0.8;

    std::filesystem::path output_directory = m_output_directory_one_demensial / "fixed";

    std::filesystem::create_directories(output_directory);

    Coder coder(config);
    Decoder decoder(config);

    const auto encoded_values =
        coder.encode_fixed(original_values.x);

    const auto decoded_values =
        decoder.decode_fixed(encoded_values);

    const std::string filename =
        type + "_comparison.jpg";

    const std::string title =
        "Сравнение исходного и раскодированного сигнала: " + type;

    save_signal_comparison(
        output_directory / filename,
        title,
        original_values.t,
        original_values.x,
        decoded_values
    );
}

void Drawer::draw_zooming(const SampledSignal& original_values, const std::string& type, const double fs) {
    AdaptiveConfig adaptive_config;
    adaptive_config.initial_step = 0.05;
    adaptive_config.min_step = 0.005;
    adaptive_config.max_step = 0.3;
    adaptive_config.grow_factor = 1.2;
    adaptive_config.decay_factor = 0.8;
    adaptive_config.zoom_offset = 0.002;

    ObserverConfig observer_config;
    observer_config.T = 1.0 / fs;
    observer_config.l1 = 0.15;
    observer_config.l2 = 0.01;
    observer_config.initial_y = original_values.x.front();
    observer_config.initial_v = 0.0;

    std::filesystem::path output_directory = m_output_directory_one_demensial / "zooming";

    std::filesystem::create_directories(output_directory);

    Coder coder(adaptive_config, observer_config);
    Decoder decoder(adaptive_config, observer_config);

    const auto encoded_values =
        coder.encode_zooming(original_values.x);

    const auto decoded_values =
        decoder.decode_zooming(encoded_values);

    const std::string filename =
        type + "_comparison.jpg";

    const std::string title =
        "Сравнение исходного и раскодированного сигнала: " + type;

    save_signal_comparison(
        output_directory / filename,
        title,
        original_values.t,
        original_values.x,
        decoded_values
    );
}

void Drawer::draw_image_zooming(const ColorImageMatrices& original_values){
    std::cerr << "\n\nADAPTIVE CODING WITH ZOOMING.\n";
    std::filesystem::path output_directry = m_output_directory_image / "zooming";

    std::filesystem::create_directories(output_directry);

    AdaptiveConfig adaptive_config;
    adaptive_config.initial_step = 0.05;
    adaptive_config.min_step = 0.005;
    adaptive_config.max_step = 0.3;
    adaptive_config.grow_factor = 1.2;
    adaptive_config.decay_factor = 0.8;
    adaptive_config.zoom_offset = 0.002;

    ObserverConfig observer_config;
    observer_config.T = 1.0;
    observer_config.l1 = 0.15;
    observer_config.l2 = 0.01;
    observer_config.initial_v = 0.0;

    ImageCoder coder(adaptive_config, observer_config);
    ImageDecoder decoder(adaptive_config, observer_config);

    const auto encoded_values =
        coder.encode_zooming(original_values);

    std::size_t total_encoded_size = Metrics::encoded_color_image_size_bytes(encoded_values);
    std::cerr << "Total encoded size is : " << total_encoded_size << "\n";

    const auto decoded_values =
        decoder.decode_zooming(encoded_values);

    long double mse = Metrics::calc_mse(original_values, decoded_values);
    std::cerr << "MSE: " << mse << "\n\n";

    const std::string filename =
        "comparison.png";

    const std::string title =
        "Сравнение исходного и раскодированного сигнала";

    save_image_comparison(
        output_directry / filename,
        title,
        original_values,
        decoded_values
    );
}

void Drawer::draw_image_fixed(const ColorImageMatrices& original_values){
    std::cerr << "\nADAPTIVE CODING WITH FIXED STEP.\n";
    std::filesystem::path output_directry = m_output_directory_image / "fixed";

    std::filesystem::create_directories(output_directry);

    AdaptiveConfig adaptive_config;
    adaptive_config.initial_step = 0.05;
    adaptive_config.min_step = 0.005;
    adaptive_config.max_step = 0.3;
    adaptive_config.grow_factor = 1.2;
    adaptive_config.decay_factor = 0.8;
    adaptive_config.zoom_offset = 0.002;

    ObserverConfig observer_config;
    observer_config.T = 1.0;
    observer_config.l1 = 0.15;
    observer_config.l2 = 0.01;
    observer_config.initial_v = 0.0;

    ImageCoder coder(adaptive_config, observer_config);
    ImageDecoder decoder(adaptive_config, observer_config);

    const auto encoded_values =
        coder.encode_fixed(original_values);

    std::size_t total_encoded_size = Metrics::encoded_color_image_size_bytes(encoded_values);
    std::cerr << "Total encoded size is : " << total_encoded_size << "\n";

    const auto decoded_values =
        decoder.decode_fixed(encoded_values);

    long double mse = Metrics::calc_mse(original_values, decoded_values);
    std::cerr << "MSE: " << mse << "\n\n";

    const std::string filename =
        "comparison.png";

    const std::string title =
        "Сравнение исходного и раскодированного сигнала";

    save_image_comparison(
        output_directry / filename,
        title,
        original_values,
        decoded_values
    );
}

void Drawer::draw_image_grow_factor(const ColorImageMatrices& original_values){
    std::cerr << "\nADAPTIVE CODING WITH GROW FACTOR.\n";
    std::filesystem::path output_directry = m_output_directory_image / "grow_factor";

    std::filesystem::create_directories(output_directry);

    AdaptiveConfig adaptive_config;
    adaptive_config.initial_step = 0.05;
    adaptive_config.min_step = 0.005;
    adaptive_config.max_step = 0.3;
    adaptive_config.grow_factor = 1.2;
    adaptive_config.decay_factor = 0.8;
    adaptive_config.zoom_offset = 0.002;

    ObserverConfig observer_config;
    observer_config.T = 1.0;
    observer_config.l1 = 0.15;
    observer_config.l2 = 0.01;
    observer_config.initial_v = 0.0;

    ImageCoder coder(adaptive_config, observer_config);
    ImageDecoder decoder(adaptive_config, observer_config);

    const auto encoded_values =
        coder.encode_grow_factor(original_values);

    std::size_t total_encoded_size = Metrics::encoded_color_image_size_bytes(encoded_values);
    std::cerr << "Total encoded size is : " << total_encoded_size << "\n";

    const auto decoded_values =
        decoder.decode_grow_factor(encoded_values);
    
    long double mse = Metrics::calc_mse(original_values, decoded_values);
    std::cerr << "MSE: " << mse << "\n\n";

    const std::string filename =
        "comparison.png";

    const std::string title =
        "Сравнение исходного и раскодированного сигнала";

    save_image_comparison(
        output_directry / filename,
        title,
        original_values,
        decoded_values
    );
}

ImageDimensions Drawer::validate_color_image(const ColorImageMatrices& image, const std::string& image_name) {
    if (image.blue.empty() ||
        image.green.empty() ||
        image.red.empty()) {
        throw std::invalid_argument(image_name + ": color channels cannot be empty");
    }

    const std::size_t height = image.blue.size();
    if (image.green.size() != height ||
        image.red.size() != height) {
        throw std::invalid_argument(image_name + ": all channels must have equal height");
    }
    if (image.blue.front().empty()) {
        throw std::invalid_argument(image_name + ": image rows cannot be empty");
    }
    const std::size_t width = image.blue.front().size();
    for (std::size_t row = 0; row < height; ++row) {
        if (image.blue[row].size() != width ||
            image.green[row].size() != width ||
            image.red[row].size() != width) {
            throw std::invalid_argument(image_name + ": all channel rows must have equal width");
        }
    }
    return {height, width};
}

std::uint8_t Drawer::normalized_to_byte(double value) {
    if (!std::isfinite(value)) {
        return 0;
    }
    const double normalized = std::clamp(value, 0.0, 1.0);
    return static_cast<std::uint8_t>(std::lround(normalized * 255.0));
}

matplot::image_channels_t Drawer::to_matplot_rgb(const ColorImageMatrices& image) {
    const ImageDimensions dimensions = validate_color_image(image, "Image");
    matplot::image_channels_t rgb_image(3, matplot::image_channel_t(dimensions.height, matplot::image_row_t(dimensions.width)));
    for (std::size_t row = 0; row < dimensions.height; ++row) {
        for (std::size_t column = 0; column < dimensions.width; ++column) {
            rgb_image[0][row][column] = normalized_to_byte(image.red[row][column]);
            rgb_image[1][row][column] = normalized_to_byte(image.green[row][column]);
            rgb_image[2][row][column] = normalized_to_byte(image.blue[row][column]);
        }
    }
    return rgb_image;
}

} // namespace adaptive