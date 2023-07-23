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
    const string str = filesystem::current_path().string() + "/paths-mgr-test";
    remove(str.c_str());

    // todo static_cast是什么
    // 测试：假如存在同名文件，则删除它并且创建文件夹
    bool ok = static_cast<bool>(ofstream(str).put('a'));
    EXPECT_TRUE(ok);
    DiskDataIO::getInstance().setup(str);
    EXPECT_TRUE(filesystem::is_directory(str));

    // 测试：假如不存在同名文件或文件夹，则创建文件夹
    remove(str.c_str());
    DiskDataIO::getInstance().setup(str);
    EXPECT_TRUE(filesystem::is_directory(str));

    remove(str.c_str());
}

TEST(DiskDataIOTest, DiskDataIO) {
    const string str = filesystem::current_path().string() + "/paths-mgr-test";
    DiskDataIO::getInstance().setup(str);

    list<string> to;
    to.push_front("b");
    to.push_front("a");
    DiskDataIO::getInstance().saveToDisk(to);

    list<string> from = DiskDataIO::getInstance().loadFromDisk();
    list<string>::iterator it = from.begin();
    EXPECT_EQ(2, from.size());
    EXPECT_EQ("a", *it);
    EXPECT_EQ("b", *(++it));

    DiskDataIO::getInstance().clear();
}

TEST(DiskDataIOTest, DiskLog) {
    const string str = filesystem::current_path().string() + "/paths-mgr-test1";
    DiskDataIO::getInstance().setup(str);

    DiskDataIO::getInstance().writeToLog("my log0");
    DiskDataIO::getInstance().writeToLog("my log1");

    EXPECT_EQ(2, DiskDataIO::getInstance().getLogLineCount());

    DiskDataIO::getInstance().clear();
}