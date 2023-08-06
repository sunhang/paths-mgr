//
// Created by sunhang on 23-7-26.
//

#ifndef PATH_MGR_DISKDATAIO_H
#define PATH_MGR_DISKDATAIO_H

#include <list>
#include <string>

using namespace std;

// todo 请阅读 https://www.learncpp.com/cpp-tutorial/basic-file-io/
// todo 进程间互斥需要了解下
// todo 目前非线程安全非进程安全
class DiskDataIO {
public:
    DiskDataIO() {};

    // todo 析构函数的正确写法是什么
    ~DiskDataIO() {};

    void setup(const string strWorkDir);

    list <string> loadFromDisk();

    void saveToDisk(list <string> paths);

    void writeToLog(string log);

    int getLogLineCount();

    void clear();

private:
    string getDataPath();

    string getLogPath();

private:
    string mStrWorkDir;
    const string mData = "data";
    const string mLog = "log";
};

#endif // PATH_MGR_DISKDATAIO_H
