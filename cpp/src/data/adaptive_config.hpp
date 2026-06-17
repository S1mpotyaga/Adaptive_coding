#pragma once

namespace adaptive{

struct AdaptiveConfig{
    double initial_step = 1.0;
    double min_step = 0.1;
    double max_step = 255.0;
    double grow_factor = 1.2;
    double decay_factor = 0.8;
    double zoom_offset = 0.01;
};

} // namespace Adaptive