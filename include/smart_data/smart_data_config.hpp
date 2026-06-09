#pragma once

#include "core/ordinal.hpp"

#include <cstdint>

enum class SmartDataDistributionType {
    Uniform,
    Normal
};

struct SmartDataConfig {
    SmartDataDistributionType distribution_type = SmartDataDistributionType::Uniform;

    Ordinal length = Ordinal(0);

    std::uint32_t seed = 0;

    int min_value = 0;
    int max_value = 100;

    double mean = 0.0;
    double standard_deviation = 1.0;
};
