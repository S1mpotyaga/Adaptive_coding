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

    auto original_line = matplot::plot(
        ax,
        time,
        original_signal
    );

    original_line
        ->line_width(2)
        .display_name("Исходный сигнал");

    matplot::hold(ax, matplot::on);

    auto decoded_line = matplot::plot(
        ax,
        time,
        decoded_signal,
        "--"
    );

    decoded_line
        ->line_width(2)
        .display_name("Раскодированный сигнал");

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

void Drawer::draw(const SampledSignal& original_values, const std::string& type) {
    AdaptiveConfig config;
    config.initial_step = 1.0;
    config.min_step = 0.1;
    config.max_step = 100.0;
    config.grow_factor = 1.2;
    config.decay_factor = 0.8;

    std::filesystem::create_directories(m_output_directory);

    Coder coder(config);
    Decoder decoder(config);

    const auto encoded_values =
        coder.encode(original_values.x);

    const auto decoded_values =
        decoder.decode(encoded_values);

    const std::string filename =
        type + "_comparison.jpg";

    const std::string title =
        "Сравнение исходного и раскодированного сигнала: " + type;

    save_signal_comparison(
        m_output_directory / filename,
        title,
        original_values.t,
        original_values.x,
        decoded_values
    );
}

} // namespace adaptive