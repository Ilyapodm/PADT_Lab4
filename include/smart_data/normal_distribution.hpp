#pragma once

#include "distribution.hpp"

#include <cmath>
#include <stdexcept>

class NormalDistribution : public Distribution<int> {
public:
    using Engine = Distribution<int>::Engine;

    NormalDistribution(
        double mean,  
        double standard_deviation,
        int min_value,
        int max_value
    ) : 
        mean_{mean},
        standard_deviation_{standard_deviation},
        min_value_{min_value},
        max_value_{max_value},
        has_spare_{false},
        spare_{0.0}
    {
        if (standard_deviation <= 0.0) {
            throw std::invalid_argument("NormalDistribution::NormalDistribution: standard deviation must be positive");
        }

        if (max_value < min_value) {
            throw std::invalid_argument("NormalDistribution::NormalDistribution: min value is greater than max value");
        }
    }

    int next(Engine& engine) override {
        const double z = next_standard_normal(engine);
        const double value = mean_ + standard_deviation_ * z;

        return round_and_clamp(value);
    }

    Distribution<int>* clone() const override {
        return new NormalDistribution(*this);
    }

    double mean() const {
        return mean_;
    }

    double standard_deviation() const {
        return standard_deviation_;
    }

    int min_value() const {
        return min_value_;
    }

    int max_value() const {
        return max_value_;
    }

private:
    double mean_;  // the center
    double standard_deviation_;
    int min_value_;
    int max_value_;

    bool has_spare_;
    double spare_;

    static double pi() {
        return 3.141592653589793238462643383279502884;
    }

    static double uniform_unit(Engine& engine) {
        const double engine_min = static_cast<double>(Engine::min());

        const double engine_range =
            static_cast<double>(Engine::max()) -
            static_cast<double>(Engine::min()) +
            1.0;

        const double raw = static_cast<double>(engine()) - engine_min;

        // maps raw integer output to the open interval (0, 1)
        return (raw + 0.5) / engine_range;
    }

    double next_standard_normal(Engine& engine) {
        if (has_spare_) {
            has_spare_ = false;
            return spare_;
        }

        const double u1 = uniform_unit(engine);
        const double u2 = uniform_unit(engine);

        // box-muller transform
        const double radius = std::sqrt(-2.0 * std::log(u1));
        const double angle = 2.0 * pi() * u2;

        const double z0 = radius * std::cos(angle);
        const double z1 = radius * std::sin(angle);

        spare_ = z1;
        has_spare_ = true;

        return z0;
    }

    int round_and_clamp(double value) const {
        if (value < static_cast<double>(min_value_)) {
            return min_value_;
        }

        if (value > static_cast<double>(max_value_)) {
            return max_value_;
        }

        const long rounded = std::lround(value);

        if (rounded < min_value_) {
            return min_value_;
        }

        if (rounded > max_value_) {
            return max_value_;
        }

        return static_cast<int>(rounded);
    }
};
