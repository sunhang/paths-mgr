//
// Created by sunhang on 23-7-26.
//

#ifndef PATH_MGR_PATHSMGR_H
#define PATH_MGR_PATHSMGR_H

#include <list>
#include <string>
#include "ArgumentsParser.h"
#include "gtest/gtest_prod.h"
#include "DiskDataIO.h"
#include <iostream>

// todo 思考什么时候用引用什么时候用指针
class PathsMgr {
    friend class PathsMgrTests;

public:
    // todo 这块有可优化的空间吗？能不调用那么多构造函数吗？
    PathsMgr(const ArgumentsParser &parser, const DiskDataIO &diskDataIo, int max = 10)
            : mArgumentsParser(parser),
              mDiskDataIo(diskDataIo),
              mMax(max) {
    }

    bool doWork();

    void clearData();

    void init();

    void save();

    ArgumentsParser &getArgumentsParser();

    DiskDataIO &getDiskDataIO();

private:
    void ls();

    void add();

    void del();

    bool cd();

    void outFilteredDirNames();

    list<string> filterByDirName(string namePrefix);

    string format(int number, string &path, string &homeDir);

    void add(string strCwd);

    list<string> getPaths();

    bool cd(string &newDir);

    bool outCdByDirName();

    list<string> cdByDirName(string dirName);

    void outSubCommandsInfo();

    FRIEND_TEST(PathsMgrTests, PathsMgr_root);

    FRIEND_TEST(PathsMgrTests, PathsMgr_add);

    FRIEND_TEST(PathsMgrTests, PathsMgr_del);

    FRIEND_TEST(PathsMgrTests, PathsMgr_cd);

    FRIEND_TEST(PathsMgrTests, PathsMgr_path_format);

    FRIEND_TEST(PathsMgrTests, PathsMgr_path_others);

private:
    list<string> mPaths;
    ArgumentsParser mArgumentsParser;
    DiskDataIO mDiskDataIo;
    const int mMax;
};


#endif //PATH_MGR_PATHSMGR_H
