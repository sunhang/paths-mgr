//
// Created by sunhang on 23-7-30.
//
#include <filesystem>
#include "gtest/gtest.h"
#include "DiskDataIO.h"
#include "ArgumentsParser.h"
#include "PathsMgr.h"
#include "common.h"

using namespace std;

class PathsMgrTests : public ::testing::Test {
protected:
    PathsMgrTests() : mPathsMgr(ArgumentsParser(), 3) {}

    virtual void SetUp() {
    }

    virtual void TearDown() {
    }

    static void SetUpTestCase() {
        cout << "PathsMgrTests SetUpTestCase" << endl;
        const string str = filesystem::current_path().string() + "/paths-mgr";
        cout << str << endl;
        DiskDataIO::getInstance().setup(str);
    }

    static void TearDownTestCase() {
        cout << "PathsMgrTests TearDownTestCase" << endl;
        DiskDataIO::getInstance().clear();
    }

protected:
    PathsMgr mPathsMgr;
};


TEST_F(PathsMgrTests, PathsMgr_add) {
    mPathsMgr.add("/a/b/c");
    mPathsMgr.add("/d/e/f");
    mPathsMgr.add("/g/h/i");
    mPathsMgr.add("/j/k/l");

    list<string> paths = mPathsMgr.getPaths();
    EXPECT_EQ(3, paths.size());
    list<string>::iterator it = paths.begin();
    EXPECT_EQ("/j/k/l", *it);
    EXPECT_EQ("/g/h/i", *(++it));
    EXPECT_EQ("/d/e/f", *(++it));

    mPathsMgr.add("/g/h/i");
    paths = mPathsMgr.getPaths();
    it = paths.begin();
    EXPECT_EQ(3, paths.size());
    EXPECT_EQ("/g/h/i", *it);
    EXPECT_EQ("/j/k/l", *(++it));
    EXPECT_EQ("/d/e/f", *(++it));

    char *add[] = {"_", "a"};
    mPathsMgr.getArgumentsParser().setup(2, add);
    mPathsMgr.doWork();
    paths = mPathsMgr.getPaths();
    it = paths.begin();
    const string str = filesystem::current_path().string();
    EXPECT_EQ(str, *it);
}

TEST_F(PathsMgrTests, PathsMgr_del) {
    list<string> origin = mPathsMgr.getPaths();
    EXPECT_EQ(3, origin.size());
    char *delIndex4[] = {"_", "d", "4"};
    mPathsMgr.getArgumentsParser().setup(3, delIndex4);
    mPathsMgr.doWork();
    list<string> deledList = mPathsMgr.getPaths();
    EXPECT_EQ(3, deledList.size());
    EXPECT_EQ(origin, deledList);

    char *delIndex1[] = {"_", "d", "1"};
    mPathsMgr.getArgumentsParser().setup(3, delIndex1);
    mPathsMgr.doWork();
    deledList = mPathsMgr.getPaths();
    EXPECT_EQ(2, deledList.size());
    list<string>::iterator it = deledList.begin();
    const string str = filesystem::current_path().string();
    EXPECT_EQ(str, *it);
    EXPECT_EQ("/j/k/l", *(++it));
}

TEST_F(PathsMgrTests, PathsMgr_cd) {
    list<string> origin = mPathsMgr.getPaths();
    char *argsCd3[] = {"_", "3"};
    mPathsMgr.getArgumentsParser().setup(2, argsCd3);
    string newDir;
    EXPECT_FALSE(mPathsMgr.cd(newDir));
    char *argsCd1[] = {"_", "1"};
    mPathsMgr.getArgumentsParser().setup(2, argsCd1);
    EXPECT_TRUE(mPathsMgr.cd(newDir));
    EXPECT_EQ("/j/k/l", newDir);
}

TEST_F(PathsMgrTests, PathsMgr_path_format) {
    string homeDir = get_home_dir();
    string path = homeDir + filesystem::path::preferred_separator + "a/b/c";
    string formatted = mPathsMgr.format(1, path, homeDir);
    EXPECT_EQ("1: c\t~/a/b/c", formatted);

    path = "/e/f/g";
    formatted = mPathsMgr.format(2, path, homeDir);
    EXPECT_EQ("2: g\t/e/f/g", formatted);
}
