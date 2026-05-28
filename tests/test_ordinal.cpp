#include <gtest/gtest.h>
#include <stdexcept>

#include "ordinal.hpp"


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

TEST(OrdinalTest, OmegaPlusFiniteKeepsFiniteTail) {
    EXPECT_EQ(Ordinal::omega() + Ordinal(5), w(1, 5));
}

TEST(OrdinalTest, OmegaPlusOmega) {
    EXPECT_EQ(Ordinal::omega() + Ordinal::omega(), w(2, 0));
}

TEST(OrdinalTest, OmegaWithTailPlusOmegaWithTail) {
    EXPECT_EQ(w(1, 5) + w(1, 3), w(2, 3));  // only second tail is kept
}

TEST(OrdinalTest, OmegaWithTailPlusFinite) {
    EXPECT_EQ(w(2, 4) + Ordinal(7), w(2, 11));
}

TEST(OrdinalTest, AdditionIsNotCommutative) {
    EXPECT_NE(Ordinal(5) + Ordinal::omega(), Ordinal::omega() + Ordinal(5));

    EXPECT_EQ(Ordinal(5) + Ordinal::omega(), w(1, 0));
    EXPECT_EQ(Ordinal::omega() + Ordinal(5), w(1, 5));
}

TEST(OrdinalTest, SubtractPrefixFinite) {
    EXPECT_EQ(Ordinal(7).subtract_prefix(Ordinal(3)), Ordinal(4));
}

TEST(OrdinalTest, SubtractPrefixEqualFinite) {
    EXPECT_EQ(Ordinal(7).subtract_prefix(Ordinal(7)), Ordinal(0));
}

TEST(OrdinalTest, SubtractPrefixTooLargeFiniteThrows) {
    EXPECT_THROW(
        Ordinal(3).subtract_prefix(Ordinal(7)),
        std::invalid_argument
    );
}

TEST(OrdinalTest, SubtractPrefixOmegaFromOmega) {
    EXPECT_EQ(w(1, 0).subtract_prefix(w(1, 0)), Ordinal(0));
}

TEST(OrdinalTest, SubtractPrefixOmegaFromOmegaWithTail) {
    EXPECT_EQ(w(1, 5).subtract_prefix(w(1, 0)), Ordinal(5));
}

TEST(OrdinalTest, SubtractPrefixSameOmegaCoeffSubtractsFiniteParts) {
    EXPECT_EQ(w(1, 7).subtract_prefix(w(1, 5)), Ordinal(2));
}

TEST(OrdinalTest, SubtractPrefixSameOmegaCoeffButBiggerTailThrows) {
    EXPECT_THROW(
        w(1, 3).subtract_prefix(w(1, 5)),
        std::invalid_argument
    );
}

TEST(OrdinalTest, SubtractPrefixSmallerOmegaCoeffDoesNotSubtractFiniteTail) {
    EXPECT_EQ(w(2, 4).subtract_prefix(w(1, 7)), w(1, 4));
}

TEST(OrdinalTest, SubtractPrefix_multiple_omega_blocks) {
    EXPECT_EQ(w(3, 2).subtract_prefix(w(2, 9)), w(1, 2));
}

TEST(OrdinalTest, SubtractPrefixTooLargeOmegaCoeffThrows) {
    EXPECT_THROW(
        w(1, 0).subtract_prefix(w(2, 0)),
        std::invalid_argument
    );
}

TEST(OrdinalTest, SubtractPrefixResultSatisfiesPrefixPlusResultEqualsCurrent) {
    Ordinal current = w(2, 4);
    Ordinal prefix = w(1, 7);

    Ordinal local = current.subtract_prefix(prefix);

    EXPECT_EQ(prefix + local, current);
}