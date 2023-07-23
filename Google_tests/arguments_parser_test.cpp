#include "ArgumentsParser.h"
#include "gtest/gtest.h"


//
// Created by sunhang on 2023/8/1.
//
// todo 阅读：https://www.cnblogs.com/jiangling500/p/8367629.html
TEST(ArgumentsParserTest, ArgumentsParser) {
    char *ls[] = {"_", "ls"};
    ArgumentsParser parser(2, ls);
    EXPECT_TRUE(parser.isArgumentsCorrect());
    EXPECT_TRUE(parser.isLs());

    char *ls0[] = {"_"};
    parser.setup(1, ls0);
    EXPECT_TRUE(parser.isArgumentsCorrect());
    EXPECT_TRUE(parser.isLs());

    char *wrong0[] = {"_", "ls", "a"};
    parser.setup(3, wrong0);
    EXPECT_FALSE(parser.isArgumentsCorrect());

    char *add[] = {"_", "a"};
    parser.setup(2, add);
    EXPECT_TRUE(parser.isArgumentsCorrect());
    EXPECT_TRUE(parser.isAdd());

    char *del[] = {"_", "d", "2"};
    parser.setup(3, del);
    EXPECT_TRUE(parser.isArgumentsCorrect());
    EXPECT_TRUE(parser.isDel());

    char *cd[] = {"_", "2"};
    parser.setup(2, cd);
    EXPECT_TRUE(parser.isArgumentsCorrect());
    EXPECT_TRUE(parser.isCd());
}