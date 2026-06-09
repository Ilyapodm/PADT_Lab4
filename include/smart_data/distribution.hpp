#pragma once

#include <random>

template <typename T>
class Distribution {
public:
    using Engine = std::mt19937;

    virtual ~Distribution() = default;

    // converts engine state into one useful value
    virtual T next(Engine& engine) = 0;

    // copies the distribution, including its internal state
    virtual Distribution<T>* clone() const = 0;
};
