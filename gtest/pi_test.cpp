#include "pi.hpp"
#include <gtest/gtest.h>
#include <cmath>

TEST(Input, AcceptsPositiveIntegers) {
    EXPECT_EQ(pi_demo::parse_n("1"), 1);
    EXPECT_EQ(pi_demo::parse_n("1000000"), 1000000);
    EXPECT_EQ(pi_demo::parse_n("3000000000"), 3000000000LL);
}

TEST(Input, RejectsInvalidValues) {
    for (const auto text : {"", "0", "-1", "abc", "1.5", "10abc", " 10", "+10"}) {
        EXPECT_EQ(pi_demo::parse_n(text), 0) << text;
    }
}

TEST(Input, RejectsOverflow) {
    EXPECT_EQ(pi_demo::parse_n("9223372036854775808"), 0);
}

TEST(Integration, ApproximatesPi) {
    EXPECT_NEAR(pi_demo::partial_pi(10000, 0, 1), std::acos(-1.0), 1e-8);
}

TEST(Integration, UnevenPartitionsMatchSerial) {
    double total = 0.0;
    for (int rank = 0; rank < 4; ++rank) {
        total += pi_demo::partial_pi(1003, rank, 4);
    }
    EXPECT_NEAR(total, pi_demo::partial_pi(1003, 0, 1), 1e-12);
}

TEST(Integration, MoreRanksThanRectangles) {
    double total = 0.0;
    for (int rank = 0; rank < 8; ++rank) {
        total += pi_demo::partial_pi(3, rank, 8);
    }
    EXPECT_DOUBLE_EQ(pi_demo::partial_pi(3, 7, 8), 0.0);
    EXPECT_NEAR(total, pi_demo::partial_pi(3, 0, 1), 1e-12);
}
