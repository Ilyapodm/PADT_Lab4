#include <gtest/gtest.h>
#include <stdexcept>

#include "../src/ordinal.hpp"

//TODO rename names of tests to PascalCase

// just for less letters
static Ordinal w(std::size_t k, std::size_t n = 0) {
    return Ordinal(k, n);
}

TEST(OrdinalTest, FiniteConstruction) {
    Ordinal a(5);

    EXPECT_TRUE(a.is_finite());
    EXPECT_EQ(a.get_omega_coeff(), 0);
    EXPECT_EQ(a.get_finite_part(), 5);
}

TEST(OrdinalTest, OmegaConstruction) {
    Ordinal omega = Ordinal::omega();

    EXPECT_FALSE(omega.is_finite());
    EXPECT_EQ(omega.get_omega_coeff(), 1);
    EXPECT_EQ(omega.get_finite_part(), 0);
}

TEST(OrdinalTest, Equality) {
    EXPECT_EQ(w(1, 5), w(1, 5));
    EXPECT_NE(w(1, 5), w(1, 6));
    EXPECT_NE(w(1, 5), w(2, 5));
}

TEST(OrdinalTest, Comparison) {
    EXPECT_LT(Ordinal(3), Ordinal(5));
    EXPECT_LT(Ordinal(100), Ordinal::omega());
    EXPECT_LT(w(1, 3), w(1, 5));
    EXPECT_LT(w(1, 100), w(2, 0));
}

TEST(OrdinalTest, FiniteAddition) {
    EXPECT_EQ(Ordinal(2) + Ordinal(3), Ordinal(5));
}

TEST(OrdinalTest, FinitePlusOmegaAbsorbsFinitePart) {
    EXPECT_EQ(Ordinal(5) + Ordinal::omega(), w(1, 0));
}

TEST(OrdinalTest, omega_plus_finite_keeps_finite_tail) {
    EXPECT_EQ(Ordinal::omega() + Ordinal(5), w(1, 5));
}

TEST(OrdinalTest, omega_plus_omega) {
    EXPECT_EQ(Ordinal::omega() + Ordinal::omega(), w(2, 0));
}

TEST(OrdinalTest, omega_with_tail_plus_omega_with_tail) {
    EXPECT_EQ(w(1, 5) + w(1, 3), w(2, 3));  // only second tail is kept
}

TEST(OrdinalTest, omega_with_tail_plus_finite) {
    EXPECT_EQ(w(2, 4) + Ordinal(7), w(2, 11));
}

TEST(OrdinalTest, addition_is_not_commutative) {
    EXPECT_NE(Ordinal(5) + Ordinal::omega(), Ordinal::omega() + Ordinal(5));

    EXPECT_EQ(Ordinal(5) + Ordinal::omega(), w(1, 0));
    EXPECT_EQ(Ordinal::omega() + Ordinal(5), w(1, 5));
}

TEST(OrdinalTest, subtract_prefix_finite) {
    EXPECT_EQ(Ordinal(7).subtract_prefix(Ordinal(3)), Ordinal(4));
}

TEST(OrdinalTest, subtract_prefix_equal_finite) {
    EXPECT_EQ(Ordinal(7).subtract_prefix(Ordinal(7)), Ordinal(0));
}

TEST(OrdinalTest, subtract_prefix_too_large_finite_throws) {
    EXPECT_THROW(
        Ordinal(3).subtract_prefix(Ordinal(7)),
        std::invalid_argument
    );
}

TEST(OrdinalTest, subtract_prefix_omega_from_omega) {
    EXPECT_EQ(w(1, 0).subtract_prefix(w(1, 0)), Ordinal(0));
}

TEST(OrdinalTest, subtract_prefix_omega_from_omega_with_tail) {
    EXPECT_EQ(w(1, 5).subtract_prefix(w(1, 0)), Ordinal(5));
}

TEST(OrdinalTest, subtract_prefix_same_omega_coeff_subtracts_finite_parts) {
    EXPECT_EQ(w(1, 7).subtract_prefix(w(1, 5)), Ordinal(2));
}

TEST(OrdinalTest, subtract_prefix_same_omega_coeff_but_bigger_tail_throws) {
    EXPECT_THROW(
        w(1, 3).subtract_prefix(w(1, 5)),
        std::invalid_argument
    );
}

TEST(OrdinalTest, subtract_prefix_smaller_omega_coeff_does_not_subtract_finite_tail) {
    EXPECT_EQ(w(2, 4).subtract_prefix(w(1, 7)), w(1, 4));
}

TEST(OrdinalTest, subtract_prefix_multiple_omega_blocks) {
    EXPECT_EQ(w(3, 2).subtract_prefix(w(2, 9)), w(1, 2));
}

TEST(OrdinalTest, subtract_prefix_too_large_omega_coeff_throws) {
    EXPECT_THROW(
        w(1, 0).subtract_prefix(w(2, 0)),
        std::invalid_argument
    );
}

TEST(OrdinalTest, subtract_prefix_result_satisfies_prefix_plus_result_equals_current) {
    Ordinal current = w(2, 4);
    Ordinal prefix = w(1, 7);

    Ordinal local = current.subtract_prefix(prefix);

    EXPECT_EQ(prefix + local, current);
}