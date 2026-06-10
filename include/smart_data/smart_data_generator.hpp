#pragma once

#include "generators/generator.hpp"
#include "smart_data/smart_data_config.hpp"
#include "smart_data/distribution.hpp"
#include "smart_data/uniform_distribution.hpp"
#include "smart_data/normal_distribution.hpp"

#include <cstddef>
#include <random>
#include <stdexcept>

class SmartDataGenerator : public Generator<int> {
public:
    using Engine = std::mt19937;

    explicit SmartDataGenerator(const SmartDataConfig& config) : 
        config_{config},
        engine_{config.seed},
        index_{0},
        distribution_{create_distribution(config)}
    {
        validate_config(config_);
    }

    SmartDataGenerator(const SmartDataGenerator& other) : 
        config_{other.config_},
        engine_{other.engine_},
        index_{other.index_},
        distribution_{other.distribution_->clone()}
    {}

    SmartDataGenerator& operator=(const SmartDataGenerator& other) {
        if (this == &other) {
            return *this;
        }

        Distribution<int>* copied_distribution = other.distribution_->clone();

        delete distribution_;

        config_ = other.config_;
        engine_ = other.engine_;
        index_ = other.index_;
        distribution_ = copied_distribution;

        return *this;
    }

    ~SmartDataGenerator() override {
        delete distribution_;
    }

    bool has_next() const override {
        if (is_omega_length()) 
            return true;
        
        return index_ < config_.length.get_finite_part();
    }

    int get_next() override {
        if (!has_next()) 
            throw std::out_of_range("SmartDataGenerator::get_next: Generator reached end");
        

        const int value = distribution_->next(engine_);
        ++index_;

        return value;
    }

    Ordinal length() const override {
        return config_.length;
    }

    Generator<int>* clone() const override {
        return new SmartDataGenerator(*this);
    }

    std::size_t current_index() const {
        return index_;
    }

    const SmartDataConfig& config() const {
        return config_;
    }

private:
    SmartDataConfig config_;
    Engine engine_;
    std::size_t index_;
    Distribution<int>* distribution_;

    // helpers
    bool is_omega_length() const {
        return config_.length == Ordinal::omega();
    }

    static void validate_config(const SmartDataConfig& config) {
        if (config.length.get_omega_coeff() > 1 || (config.length.get_omega_coeff() == 1 && config.length.get_finite_part() != 0)) 
            throw std::invalid_argument("SmartDataGenerator::SmartDataGenerator: length must be finite or omega");
        
        if (config.max_value < config.min_value) 
            throw std::invalid_argument("SmartDataGenerator::SmartDataGenerator: min value is greater than max value");
        
        if (config.distribution_type == SmartDataDistributionType::Normal && config.standard_deviation <= 0.0) 
            throw std::invalid_argument("SmartDataGenerator::SmartDataGenerator: standard deviation must be positive");
    }

    static Distribution<int>* create_distribution(const SmartDataConfig& config) {
        // creates a concrete distribution from config
        switch (config.distribution_type) {
            case SmartDataDistributionType::Uniform:
                return new UniformDistribution(
                    config.min_value,
                    config.max_value
                );

            case SmartDataDistributionType::Normal:
                return new NormalDistribution(
                    config.mean,
                    config.standard_deviation,
                    config.min_value,
                    config.max_value
                );

            default:
                throw std::invalid_argument("SmartDataGenerator<int>::create_distribution: unknown smart data distribution type");
        }
    }
};