#include <gtest/gtest.h>

#include "smart_data/smart_data_config.hpp"
#include "smart_data/smart_data_generator.hpp"
#include "smart_data/uniform_distribution.hpp"
#include "smart_data/normal_distribution.hpp"
#include "smart_data/distribution.hpp"

#include <cstddef>
#include <cstdint>
#include <random>
#include <stdexcept>


static SmartDataConfig make_uniform_config(
    int min_value,
    int max_value,
    std::size_t length,
    std::uint32_t seed
) {
    SmartDataConfig config;

    config.distribution_type = SmartDataDistributionType::Uniform;
    config.length = Ordinal(length);
    config.seed = seed;
    config.min_value = min_value;
    config.max_value = max_value;

    return config;
}


static SmartDataConfig make_normal_config(
    double mean,
    double standard_deviation,
    int min_value,
    int max_value,
    std::size_t length,
    std::uint32_t seed
) {
    SmartDataConfig config;

    config.distribution_type = SmartDataDistributionType::Normal;
    config.length = Ordinal(length);
    config.seed = seed;
    config.min_value = min_value;
    config.max_value = max_value;
    config.mean = mean;
    config.standard_deviation = standard_deviation;

    return config;
}


static void consume_values(Generator<int>& generator, std::size_t count) {
    for (std::size_t i = 0; i < count; ++i) {
        generator.get_next();
    }
}


static void expect_next_values_equal(
    Generator<int>& left,
    Generator<int>& right,
    std::size_t count
) {
    for (std::size_t i = 0; i < count; ++i) {
        ASSERT_TRUE(left.has_next());
        ASSERT_TRUE(right.has_next());

        const int left_value = left.get_next();
        const int right_value = right.get_next();

        EXPECT_EQ(left_value, right_value);
    }
}


TEST(UniformDistributionTests, ConstructorRejectsInvalidRange) {
    EXPECT_THROW(
        {
            UniformDistribution distribution(10, 5);
        },
        std::invalid_argument
    );
}


TEST(UniformDistributionTests, GeneratedValuesAreInsideRange) {
    UniformDistribution distribution(-10, 10);
    std::mt19937 engine(42);

    for (std::size_t i = 0; i < 1000; ++i) {
        const int value = distribution.next(engine);

        EXPECT_GE(value, -10);
        EXPECT_LE(value, 10);
    }
}


TEST(UniformDistributionTests, SameEngineSeedProducesSameValues) {
    UniformDistribution left_distribution(0, 100);
    UniformDistribution right_distribution(0, 100);

    std::mt19937 left_engine(42);
    std::mt19937 right_engine(42);

    for (std::size_t i = 0; i < 100; ++i) {
        const int left_value = left_distribution.next(left_engine);
        const int right_value = right_distribution.next(right_engine);

        EXPECT_EQ(left_value, right_value);
    }
}


TEST(UniformDistributionTests, CloneProducesEquivalentDistribution) {
    UniformDistribution original(0, 100);

    std::mt19937 original_engine(42);
    std::mt19937 copied_engine(42);

    Distribution<int>* copied = original.clone();

    ASSERT_NE(copied, nullptr);

    for (std::size_t i = 0; i < 100; ++i) {
        const int original_value = original.next(original_engine);
        const int copied_value = copied->next(copied_engine);

        EXPECT_EQ(original_value, copied_value);
    }

    delete copied;
}


TEST(NormalDistributionTests, ConstructorRejectsInvalidStandardDeviation) {
    EXPECT_THROW(
        {
            NormalDistribution distribution(0.0, 0.0, -100, 100);
        },
        std::invalid_argument
    );

    EXPECT_THROW(
        {
            NormalDistribution distribution(0.0, -1.0, -100, 100);
        },
        std::invalid_argument
    );
}


TEST(NormalDistributionTests, ConstructorRejectsInvalidRange) {
    EXPECT_THROW(
        {
            NormalDistribution distribution(0.0, 1.0, 100, -100);
        },
        std::invalid_argument
    );
}


TEST(NormalDistributionTests, GeneratedValuesAreInsideClampedRange) {
    NormalDistribution distribution(50.0, 10.0, 0, 100);
    std::mt19937 engine(42);

    for (std::size_t i = 0; i < 1000; ++i) {
        const int value = distribution.next(engine);

        EXPECT_GE(value, 0);
        EXPECT_LE(value, 100);
    }
}


TEST(NormalDistributionTests, SameEngineSeedProducesSameValues) {
    NormalDistribution left_distribution(50.0, 10.0, 0, 100);
    NormalDistribution right_distribution(50.0, 10.0, 0, 100);

    std::mt19937 left_engine(42);
    std::mt19937 right_engine(42);

    for (std::size_t i = 0; i < 100; ++i) {
        const int left_value = left_distribution.next(left_engine);
        const int right_value = right_distribution.next(right_engine);

        EXPECT_EQ(left_value, right_value);
    }
}


TEST(NormalDistributionTests, CloneCopiesSpareState) {
    NormalDistribution original(50.0, 10.0, 0, 100);
    std::mt19937 original_engine(42);

    original.next(original_engine);

    Distribution<int>* copied = original.clone();

    ASSERT_NE(copied, nullptr);

    std::mt19937 copied_engine = original_engine;

    for (std::size_t i = 0; i < 20; ++i) {
        const int original_value = original.next(original_engine);
        const int copied_value = copied->next(copied_engine);

        EXPECT_EQ(original_value, copied_value);
    }

    delete copied;
}


TEST(SmartDataGeneratorTests, UniformGeneratorHasCorrectFiniteLength) {
    SmartDataConfig config = make_uniform_config(0, 100, 5, 42);
    SmartDataGenerator generator(config);

    EXPECT_TRUE(generator.has_next());
    EXPECT_EQ(generator.length(), Ordinal(5));
    EXPECT_EQ(generator.current_index(), 0u);

    for (std::size_t i = 0; i < 5; ++i) {
        EXPECT_TRUE(generator.has_next());

        const int value = generator.get_next();

        EXPECT_GE(value, 0);
        EXPECT_LE(value, 100);
        EXPECT_EQ(generator.current_index(), i + 1);
    }

    EXPECT_FALSE(generator.has_next());
}


TEST(SmartDataGeneratorTests, UniformGeneratorThrowsAfterEnd) {
    SmartDataConfig config = make_uniform_config(0, 100, 3, 42);
    SmartDataGenerator generator(config);

    generator.get_next();
    generator.get_next();
    generator.get_next();

    EXPECT_FALSE(generator.has_next());

    EXPECT_THROW(
        {
            generator.get_next();
        },
        std::out_of_range
    );
}


TEST(SmartDataGeneratorTests, NormalGeneratorHasCorrectFiniteLength) {
    SmartDataConfig config = make_normal_config(50.0, 10.0, 0, 100, 5, 42);
    SmartDataGenerator generator(config);

    EXPECT_TRUE(generator.has_next());
    EXPECT_EQ(generator.length(), Ordinal(5));
    EXPECT_EQ(generator.current_index(), 0u);

    for (std::size_t i = 0; i < 5; ++i) {
        EXPECT_TRUE(generator.has_next());

        const int value = generator.get_next();

        EXPECT_GE(value, 0);
        EXPECT_LE(value, 100);
        EXPECT_EQ(generator.current_index(), i + 1);
    }

    EXPECT_FALSE(generator.has_next());
}


TEST(SmartDataGeneratorTests, SameUniformConfigProducesSameSequence) {
    SmartDataConfig config = make_uniform_config(-100, 100, 50, 42);

    SmartDataGenerator left_generator(config);
    SmartDataGenerator right_generator(config);

    expect_next_values_equal(left_generator, right_generator, 50);

    EXPECT_FALSE(left_generator.has_next());
    EXPECT_FALSE(right_generator.has_next());
}


TEST(SmartDataGeneratorTests, SameNormalConfigProducesSameSequence) {
    SmartDataConfig config = make_normal_config(0.0, 1.0, -10, 10, 50, 42);

    SmartDataGenerator left_generator(config);
    SmartDataGenerator right_generator(config);

    expect_next_values_equal(left_generator, right_generator, 50);

    EXPECT_FALSE(left_generator.has_next());
    EXPECT_FALSE(right_generator.has_next());
}


TEST(SmartDataGeneratorTests, UniformCloneContinuesFromSameState) {
    SmartDataConfig config = make_uniform_config(0, 100, 20, 42);
    SmartDataGenerator original(config);

    consume_values(original, 7);

    Generator<int>* copied = original.clone();

    ASSERT_NE(copied, nullptr);

    EXPECT_EQ(original.current_index(), 7u);
    expect_next_values_equal(original, *copied, 13);

    EXPECT_FALSE(original.has_next());
    EXPECT_FALSE(copied->has_next());

    delete copied;
}


TEST(SmartDataGeneratorTests, NormalCloneContinuesFromSameState) {
    SmartDataConfig config = make_normal_config(50.0, 10.0, 0, 100, 20, 42);
    SmartDataGenerator original(config);

    consume_values(original, 1);

    Generator<int>* copied = original.clone();

    ASSERT_NE(copied, nullptr);

    EXPECT_EQ(original.current_index(), 1u);
    expect_next_values_equal(original, *copied, 19);

    EXPECT_FALSE(original.has_next());
    EXPECT_FALSE(copied->has_next());

    delete copied;
}


TEST(SmartDataGeneratorTests, CopyConstructorCopiesCurrentState) {
    SmartDataConfig config = make_uniform_config(0, 100, 20, 42);
    SmartDataGenerator original(config);

    consume_values(original, 5);

    SmartDataGenerator copied(original);

    EXPECT_EQ(original.current_index(), 5u);
    EXPECT_EQ(copied.current_index(), 5u);

    expect_next_values_equal(original, copied, 15);

    EXPECT_FALSE(original.has_next());
    EXPECT_FALSE(copied.has_next());
}


TEST(SmartDataGeneratorTests, CopyAssignmentCopiesCurrentState) {
    SmartDataConfig source_config = make_uniform_config(0, 100, 20, 42);
    SmartDataConfig target_config = make_uniform_config(-5, 5, 3, 777);

    SmartDataGenerator source(source_config);
    SmartDataGenerator target(target_config);

    consume_values(source, 5);

    target = source;

    EXPECT_EQ(source.current_index(), 5u);
    EXPECT_EQ(target.current_index(), 5u);

    expect_next_values_equal(source, target, 15);

    EXPECT_FALSE(source.has_next());
    EXPECT_FALSE(target.has_next());
}


TEST(SmartDataGeneratorTests, ConstructorRejectsInvalidUniformRange) {
    SmartDataConfig config = make_uniform_config(10, 5, 10, 42);

    EXPECT_THROW(
        {
            SmartDataGenerator generator(config);
        },
        std::invalid_argument
    );
}


TEST(SmartDataGeneratorTests, ConstructorRejectsInvalidNormalStandardDeviation) {
    SmartDataConfig config = make_normal_config(0.0, 0.0, -10, 10, 10, 42);

    EXPECT_THROW(
        {
            SmartDataGenerator generator(config);
        },
        std::invalid_argument
    );
}