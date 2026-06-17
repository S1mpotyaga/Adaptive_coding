#include "drawer.hpp"

namespace adaptive{

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

void Drawer::draw_grow_factor(const SampledSignal& original_values, const std::string& type) {
    AdaptiveConfig config;
    config.initial_step = 1.0;
    config.min_step = 0.1;
    config.max_step = 100.0;
    config.grow_factor = 1.2;
    config.decay_factor = 0.8;

    std::filesystem::create_directories(m_output_directory_grow_factor);

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
        m_output_directory_grow_factor / filename,
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

    std::filesystem::create_directories(m_output_directory_fixed);

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
        m_output_directory_fixed / filename,
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

    std::filesystem::create_directories(m_output_directory_zooming);

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
        m_output_directory_zooming / filename,
        title,
        original_values.t,
        original_values.x,
        decoded_values
    );
}


} // namespace adaptive