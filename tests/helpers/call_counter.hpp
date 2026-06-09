#pragma once

#include <cstddef>
#include <functional>
#include <stdexcept>

struct CountingFunction{
public:
    CountingFunction(int& call_count, std::function<int(std::size_t)> func) :
        call_count_(call_count),
        function_{func}
    {
        if (!function_)
            throw std::invalid_argument("CountingFunction's function is null");
    }

    int operator()(std::size_t index) {
        ++call_count_;
        return function_(index);
    }
private:
    int& call_count_;
    std::function<int(std::size_t)> function_;
};