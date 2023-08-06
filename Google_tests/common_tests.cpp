//
// Created by sunhang on 2023/8/5.
//
#include <list>
#include "common.h"
#include "gtest/gtest.h"

TEST(CommonTests, Common) {
    EXPECT_EQ("https%3A%2F%2Fwww.163.com", url_encode("https://www.163.com"));

    list<string> strs = {"https://www.163.com", ";/foo/bar/baz:"};
    transform(strs.begin(), strs.end(), strs.begin(), ::url_encode);
    EXPECT_EQ(2, strs.size());
    list<string>::iterator it = strs.begin();
    EXPECT_EQ("https%3A%2F%2Fwww.163.com", *it);
    EXPECT_EQ("%3B%2Ffoo%2Fbar%2Fbaz%3A", *(++it));


    strs = {"foo", "bar", "baz"};
    string result = join(strs, ":");
    EXPECT_EQ("foo:bar:baz", result);
}