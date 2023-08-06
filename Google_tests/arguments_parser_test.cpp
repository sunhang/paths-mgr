#include "ArgumentsParser.h"
#include "gtest/gtest.h"
#include "base_tests.h"


//
// Created by sunhang on 2023/8/1.
//
// todo 阅读：https://www.cnblogs.com/jiangling500/p/8367629.html
TEST(ArgumentsParserTest, ArgumentsParser) {
    ArgumentsParser parser(2, ARGUMENTS(PATHS_MGR_CMD, "ls"));
    EXPECT_TRUE(parser.isArgumentsCorrect());
    EXPECT_TRUE(parser.isLs());

    parser.setup(1, ARGUMENTS(PATHS_MGR_CMD));
    EXPECT_TRUE(parser.isArgumentsCorrect());
    EXPECT_TRUE(parser.isLs());

    parser.setup(3, ARGUMENTS(PATHS_MGR_CMD, "ls", "a"));
    EXPECT_FALSE(parser.isArgumentsCorrect());

    parser.setup(2, ARGUMENTS(PATHS_MGR_CMD, "a"));
    EXPECT_TRUE(parser.isArgumentsCorrect());
    EXPECT_TRUE(parser.isAdd());

    parser.setup(3, ARGUMENTS(PATHS_MGR_CMD, "d", "2"));
    EXPECT_TRUE(parser.isArgumentsCorrect());
    EXPECT_TRUE(parser.isDel());

    parser.setup(2, ARGUMENTS(PATHS_MGR_CMD, "2"));
    EXPECT_TRUE(parser.isArgumentsCorrect());
    EXPECT_TRUE(parser.isNumber());

    parser.setup(2, ARGUMENTS(PATHS_MGR_CMD, "predict"));
    EXPECT_FALSE(parser.isArgumentsCorrect());
    EXPECT_FALSE(parser.isPredict());

    parser.setup(3, ARGUMENTS(PATHS_MGR_CMD, "predict", "n"));
    EXPECT_TRUE(parser.isArgumentsCorrect());
    EXPECT_TRUE(parser.isPredict());

    parser.setup(3, ARGUMENTS(PATHS_MGR_CMD, "cd", "xxx"));
    EXPECT_TRUE(parser.isArgumentsCorrect());
    EXPECT_TRUE(parser.isCd());

    parser.setup(2, ARGUMENTS(PATHS_MGR_CMD, "cd"));
    EXPECT_FALSE(parser.isArgumentsCorrect());
    EXPECT_FALSE(parser.isCd());

    parser.setup(2, ARGUMENTS(PATHS_MGR_CMD, "subcommands"));
    EXPECT_TRUE(parser.isArgumentsCorrect());
    EXPECT_TRUE(parser.isRequestSubCommandsInfo());
}