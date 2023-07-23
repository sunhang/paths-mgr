//
// Created by sunhang on 23-7-26.
//

#ifndef PATH_MGR_PATHSMGR_H
#define PATH_MGR_PATHSMGR_H

#include <list>
#include <string>
#include "ArgumentsParser.h"
#include "gtest/gtest_prod.h"

class PathsMgr {
    friend class PathsMgrTests;

public:
    PathsMgr(ArgumentsParser parser, int max = 10)
            : mArgumentsParser(parser), mMax(max) {}

    bool doWork();

    ArgumentsParser &getArgumentsParser();

private:
    void ls();

    void add();

    void del();

    bool cd();

    string format(int number, string &path, string &homeDir);

    void add(string strCwd);

    list<string> getPaths();

    bool cd(string &newDir);

    FRIEND_TEST(PathsMgrTests, PathsMgr_add);

    FRIEND_TEST(PathsMgrTests, PathsMgr_del);

    FRIEND_TEST(PathsMgrTests, PathsMgr_cd);

    FRIEND_TEST(PathsMgrTests, PathsMgr_path_format);

private:
    list<string> mPaths;
    ArgumentsParser mArgumentsParser;
    const int mMax;
};


#endif //PATH_MGR_PATHSMGR_H
