#pragma once

#include "distribution.hpp"

#include <cstdint>
#include <stdexcept>

class UniformDistribution : public Distribution<int> {
public:
    using Engine = Distribution<int>::Engine;

    UniformDistribution(int min_value, int max_value) : 
        min_value_{min_value},
        max_value_{max_value}
    {
        if (max_value < min_value) 
            throw std::invalid_argument("UniformDistribution<T>::UniformDistribution: uniform distribution min value is greater than max value");
        
    }

    int next(Engine& engine) override {
        const std::uint64_t offset = next_offset(engine);

        const std::int64_t value =
            static_cast<std::int64_t>(min_value_) +
            static_cast<std::int64_t>(offset);

        return static_cast<int>(value);
    }

    Distribution<int>* clone() const override {
        return new UniformDistribution(*this);
    }

    int min_value() const {
        return min_value_;
    }

    int max_value() const {
        return max_value_;
    }

private:
    int min_value_;
    int max_value_;

    // just max_val - min_val + 1 - amount of available elements
    std::uint64_t range_size() const {
        return static_cast<std::uint64_t>(
            static_cast<std::int64_t>(max_value_) -
            static_cast<std::int64_t>(min_value_)
        ) + 1ULL;
    }

    // return the next raw number from engine [0, ...]
    std::uint64_t next_raw(Engine& engine) const {
        return static_cast<std::uint64_t>(engine()) -
               static_cast<std::uint64_t>(Engine::min());
    }

    // generates offset without modulo bias
    std::uint64_t next_offset(Engine& engine) const {
        const std::uint64_t target_range = range_size();

        // amount of elements, that could be given by engine
        const std::uint64_t engine_range =
            static_cast<std::uint64_t>(Engine::max()) -
            static_cast<std::uint64_t>(Engine::min()) +
            1ULL;

        // range without probabilistic bias (amounts of every remainder are equal)
        const std::uint64_t accepted_range =
            engine_range - engine_range % target_range;

        std::uint64_t raw = 0;

        // rejection sampling removes modulo bias
        do {
            raw = next_raw(engine);
        } while (raw >= accepted_range);

        return raw % target_range;
    }
};

