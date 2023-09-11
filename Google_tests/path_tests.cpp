//
// Created by sunhang on 2023/9/11.
//
#include "gtest/gtest.h"
#include "Path.h"
#include <list>
#include <cmath>

TEST(PathTests, Equal) {
    Path p0(0.9f, "foo");
    Path p1(0.9f, "foo");
    EXPECT_TRUE(p0 == p1);
}

TEST(PathTests, Min) {
    std::list<Path> paths;
    paths.push_back(Path(60, "zhang"));
    paths.push_back(Path(53.1, "wang"));
    paths.push_back(Path(53.1, "li"));
    paths.push_back(Path(80.1, "zhao"));

    list<Path>::iterator it = min_element(paths.begin(), paths.end(), [](const auto &a, const auto &b) {
        return a.getFrequency() < b.getFrequency()
               || fabs(a.getFrequency() - b.getFrequency()) < FLT_EPSILON;
    });

    EXPECT_TRUE(*it == Path(53.1, "li"));
}