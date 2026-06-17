#pragma once

namespace adaptive{

struct ObserverConfig{
    double T = 1.0;
    double l2 = 0.1;
    double l1 = 1.0;
    double initial_y = 0.0;
    double initial_v = 0.0;
};

} // namespace adaptive