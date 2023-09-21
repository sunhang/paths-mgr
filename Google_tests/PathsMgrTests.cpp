//
// Created by sunhang on 23-7-30.
//
#include <filesystem>
#include "gtest/gtest.h"
#include "DiskDataIO.h"
#include "ArgumentsParser.h"
#include "PathsMgr.h"
#include "common.h"
#include "base_tests.h"

using namespace std;


class PathsMgrTests : public ::testing::Test {
protected:
    PathsMgrTests() {}

    virtual void SetUp() {
    }

    virtual void TearDown() {
    }

    static void SetUpTestCase() {
        cout << "PathsMgrTests SetUpTestCase" << endl;
        mPathsMgr.init();
    }

    static void TearDownTestCase() {
        cout << "PathsMgrTests TearDownTestCase" << endl;
        mPathsMgr.clearData();
        mPathsMgr.save();
        mPathsMgr.getDiskDataIO().clear();
    }

protected:
    // todo 命名规则是什么？
    static PathsMgr mPathsMgr;
};

static PathsMgr createPathsMgr() {
    const string str = filesystem::current_path().string() + "/paths-mgr";
    cout << str << endl;
    DiskDataIO diskDataIo;
    diskDataIo.setup(str);
    ArgumentsParser parser;
    return PathsMgr(parser, diskDataIo, 3);
}

PathsMgr PathsMgrTests::mPathsMgr = createPathsMgr();

TEST_F(PathsMgrTests, PathsMgr_root) {
    mPathsMgr.clearData();

    mPathsMgr.add("/");
    list<Path> paths = mPathsMgr.getPaths();
    EXPECT_EQ(0, paths.size());

    list<string> fileNames = mPathsMgr.filterByDirName("");
    EXPECT_EQ(0, fileNames.size());

    mPathsMgr.clearData();
}

TEST_F(PathsMgrTests, PathsMgr_Frequency) {
    mPathsMgr.add("foo");
    mPathsMgr.add("bar");
    mPathsMgr.add("baz");
    mPathsMgr.getArgumentsParser().setup(3, ARGUMENTS(PATHS_MGR_CMD, "r", "foo"));
    mPathsMgr.doWork();
    mPathsMgr.add("zhang");
    list<Path> paths = mPathsMgr.getPaths();
    EXPECT_EQ(3, paths.size());
    list<Path>::iterator it = paths.begin();
    EXPECT_PATH_STR_EQ("zhang", *it);
    EXPECT_PATH_STR_EQ("baz", *(++it));
    EXPECT_PATH_STR_EQ("foo", *(++it));

    mPathsMgr.add("wang");
    paths = mPathsMgr.getPaths();
    EXPECT_EQ(3, paths.size());
    it = paths.begin();
    EXPECT_PATH_STR_EQ("wang", *it);
    EXPECT_PATH_STR_EQ("zhang", *(++it));
    EXPECT_PATH_STR_EQ("foo", *(++it));

    mPathsMgr.clearData();
}

TEST_F(PathsMgrTests, PathsMgr_add) {
    mPathsMgr.add("/a/b/c");
    mPathsMgr.add("/d/e/f");
    mPathsMgr.add("/g/h/i");
    mPathsMgr.add("/j/k/l");

    list<Path> paths = mPathsMgr.getPaths();
    EXPECT_EQ(3, paths.size());
    list<Path>::iterator it = paths.begin();
    EXPECT_PATH_STR_EQ("/j/k/l", *it);
    EXPECT_PATH_STR_EQ("/g/h/i", *(++it));
    EXPECT_PATH_STR_EQ("/d/e/f", *(++it));

    mPathsMgr.add("/g/h/i");
    paths = mPathsMgr.getPaths();
    it = paths.begin();
    EXPECT_EQ(3, paths.size());
    EXPECT_PATH_STR_EQ("/g/h/i", *it);
    EXPECT_PATH_STR_EQ("/j/k/l", *(++it));
    EXPECT_PATH_STR_EQ("/d/e/f", *(++it));

    mPathsMgr.getArgumentsParser().setup(2, ARGUMENTS(PATHS_MGR_CMD, "a"));
    mPathsMgr.doWork();
    paths = mPathsMgr.getPaths();
    it = paths.begin();
    const string str = filesystem::current_path().string();
    EXPECT_PATH_STR_EQ(str, *it);
}

TEST_F(PathsMgrTests, PathsMgr_del) {
    list<Path> origin = mPathsMgr.getPaths();
    EXPECT_EQ(3, origin.size());
    mPathsMgr.getArgumentsParser().setup(3, ARGUMENTS(PATHS_MGR_CMD, "d", "4"));
    mPathsMgr.doWork();
    list<Path> deledList = mPathsMgr.getPaths();
    EXPECT_EQ(3, deledList.size());
    EXPECT_EQ(origin, deledList);

    mPathsMgr.getArgumentsParser().setup(3, ARGUMENTS(PATHS_MGR_CMD, "d", "1"));
    mPathsMgr.doWork();
    deledList = mPathsMgr.getPaths();
    list<Path>::iterator it = deledList.begin();
    const string str = filesystem::current_path().string();
    EXPECT_PATH_STR_EQ(str, *it);
    EXPECT_PATH_STR_EQ("/j/k/l", *(++it));
    EXPECT_EQ(2, deledList.size());
}

TEST_F(PathsMgrTests, PathsMgr_cd) {
    mPathsMgr.getArgumentsParser().setup(2, ARGUMENTS(PATHS_MGR_CMD, "3"));
    string newDir;
    EXPECT_FALSE(mPathsMgr.cdByNumber(newDir));
    mPathsMgr.getArgumentsParser().setup(2, ARGUMENTS(PATHS_MGR_CMD, "1"));
    EXPECT_TRUE(mPathsMgr.cdByNumber(newDir));
    EXPECT_EQ("/j/k/l", newDir);
    list<Path> origin = mPathsMgr.getPaths();
    EXPECT_EQ(2, origin.size());

    mPathsMgr.clearData();
    mPathsMgr.add("/a/foo");
    mPathsMgr.add("/a/f");
    list<string> dirs;
    dirs = mPathsMgr.cdByDirName("f");
    list<string>::iterator it = dirs.begin();
    EXPECT_EQ(1, dirs.size());
    EXPECT_EQ("/a/f", *it);

    mPathsMgr.add("/c/foo");
    dirs = mPathsMgr.cdByDirName("foo");
    it = dirs.begin();
    EXPECT_EQ(2, dirs.size());
    EXPECT_EQ("/c/foo", *it);
    EXPECT_EQ("/a/foo", *(++it));
}

TEST_F(PathsMgrTests, PathsMgr_path_format) {
    string homeDir = get_home_dir();
    string path = homeDir + filesystem::path::preferred_separator + "a/b/c";
    string formatted = mPathsMgr.format(1, path, homeDir, 1.2f, 32);
    EXPECT_EQ("1) c  ~/a/b/c:1.2", formatted);

    path = "/e/f/g";
    formatted = mPathsMgr.format(2, path, homeDir, 24, 32);
    EXPECT_EQ("2) g  /e/f/g:24", formatted);

    path = homeDir + filesystem::path::preferred_separator + "Documents/linux-debug/source/Linux-0.11code";
    formatted = mPathsMgr.format(3, path, homeDir, 2.4f, 30);
    EXPECT_EQ("3) Linux-0.11code  ~/Docume...", formatted);
}

TEST_F(PathsMgrTests, PathsMgr_path_others) {
    mPathsMgr.clearData();

    mPathsMgr.add("/a/b/never");
    mPathsMgr.add("/d/e/now");
    mPathsMgr.add("/g/h/tomorrow");

    list<string> fileDirs = mPathsMgr.filterByDirName("n");
    list<string>::iterator it = fileDirs.begin();
    EXPECT_EQ("now", *(it));
    EXPECT_EQ("never", *(++it));
    EXPECT_EQ(2, fileDirs.size());

    mPathsMgr.add("/h/h/tomorrow");
    fileDirs = mPathsMgr.filterByDirName("tom");
    it = fileDirs.begin();
    ASSERT_EQ(1, fileDirs.size());
    if (fileDirs.size() == 1) {
        EXPECT_EQ("tomorrow", *it);
    }
}