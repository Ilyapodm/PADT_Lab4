#pragma once

#include <cstddef>
#include <stdexcept>

#include "generator.hpp"
#include "lab_2/sequence.hpp"
#include "ordinal.hpp"

template <typename T>
class SequenceGenerator : public Generator<T> {
public:
    explicit SequenceGenerator(const Sequence<T>* source)
        : source_(source) {
            if (source_ == nullptr) {
                throw std::invalid_argument("SequenceGenerator source is null");
        }
    }

    bool has_next() const override {
        return index_ < source_->get_size();
    }

    T get_next() override {
        if (!has_next()) {
            throw std::out_of_range("Generator is exhausted");
        }

        return source_->get(index_++);
    }

    Ordinal length() const override {
        return Ordinal(source_->get_size());
    }

    SequenceGenerator<T>* clone() const override {
        return new SequenceGenerator<T>(*this);
    }

private:
    const Sequence<T>* source_;
    std::size_t index_ = 0;
};
