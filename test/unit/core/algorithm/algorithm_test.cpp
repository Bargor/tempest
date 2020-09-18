// This file is part of Tempest-core project
// Author: Karol Kontny

#include "gtest/gtest.h"
#include <algorithm/algorithm.h>

TEST(includes, empty_range) {
    std::vector<int> v1{};
    std::vector<int> v2{};
    EXPECT_TRUE(
        tst::includes(v1.begin(), v1.end(), v2.begin(), v2.end(), [](const int rhs, const int lhs) { return rhs == lhs; }));
}

TEST(includes, empty_second_range) {
    std::vector<int> v1{1, 2};
    std::vector<int> v2{};
    EXPECT_TRUE(
        tst::includes(v1.begin(), v1.end(), v2.begin(), v2.end(), [](const int rhs, const int lhs) { return rhs == lhs; }));
}

TEST(includes, empty_first_range) {
    std::vector<int> v1{};
    std::vector<int> v2{1, 2};
    EXPECT_FALSE(tst::includes(
        v1.begin(), v1.end(), v2.begin(), v2.end(), [](const int rhs, const int lhs) { return rhs == lhs; }));
}

TEST(includes, includes) {
    std::vector<int> v1{1, 2};
    std::vector<int> v2{1};
    EXPECT_TRUE(tst::includes(
        v1.begin(), v1.end(), v2.begin(), v2.end(), [](const int rhs, const int lhs) { return rhs == lhs; }));
}

TEST(includes, not_includes) {
    std::vector<int> v1{1, 2};
    std::vector<int> v2{3};
    EXPECT_FALSE(tst::includes(
        v1.begin(), v1.end(), v2.begin(), v2.end(), [](const int rhs, const int lhs) { return rhs == lhs; }));
}

TEST(for_each_adjacent, range_empty) {
    std::vector<int> v1{};
    tst::for_each_adjacent(v1.begin(), v1.end(), [](const int rhs, int lhs) { lhs += rhs; });
}

TEST(for_each_adjacent, range_one_element) {
    std::vector<int> v1{0};
    tst::for_each_adjacent(v1.begin(), v1.end(), [](const int rhs, int lhs) { lhs += rhs; });
    EXPECT_EQ(v1[0], 0);
}

TEST(for_each_adjacent, range_two_elements) {
    std::vector<int> v1{1, 0};
    tst::for_each_adjacent(v1.begin(), v1.end(), [](const int rhs, int& lhs) { lhs += rhs; });
    EXPECT_EQ(v1[0], 1);
    EXPECT_EQ(v1[1], 1);
}