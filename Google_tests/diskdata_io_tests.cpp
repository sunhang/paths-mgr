//
// Created by sunhang on 2023/8/1.
//

#include <filesystem>
#include <fstream>
#include "gtest/gtest.h"
#include "DiskDataIO.h"

using namespace std;

/**
 * 测试文件夹的创建
 */
TEST(DiskDataIOTest, DiskDataIOSetup) {
    DiskDataIO diskDataIO;
    const string str = filesystem::current_path().string() + "/paths-mgr-test";
    remove(str.c_str());

    // todo static_cast是什么
    // 测试：假如存在同名文件，则删除它并且创建文件夹
    bool ok = static_cast<bool>(ofstream(str).put('a'));
    EXPECT_TRUE(ok);
    diskDataIO.setup(str);
    EXPECT_TRUE(filesystem::is_directory(str));

    // 测试：假如不存在同名文件或文件夹，则创建文件夹
    remove(str.c_str());
    diskDataIO.setup(str);
    EXPECT_TRUE(filesystem::is_directory(str));

    remove(str.c_str());
}

TEST(DiskDataIOTest, DiskDataIO) {
    DiskDataIO diskDataIO;
    const string str = filesystem::current_path().string() + "/paths-mgr-test";
    diskDataIO.setup(str);

    list<string> to;
    to.push_front("b");
    to.push_front("a");
    diskDataIO.saveToDisk(to);

    list<string> from = diskDataIO.loadFromDisk();
    list<string>::iterator it = from.begin();
    EXPECT_EQ(2, from.size());
    EXPECT_EQ("a", *it);
    EXPECT_EQ("b", *(++it));

    diskDataIO.clear();
}

TEST(DiskDataIOTest, DiskLog) {
    DiskDataIO diskDataIO;
    const string str = filesystem::current_path().string() + "/paths-mgr-test1";
    diskDataIO.setup(str);

    diskDataIO.writeToLog("my log0");
    diskDataIO.writeToLog("my log1");

    EXPECT_EQ(2, diskDataIO.getLogLineCount());

    diskDataIO.clear();
}