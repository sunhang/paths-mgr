//
// Created by sunhang on 23-7-26.
//

#ifndef PATH_MGR_DISKDATAIO_H
#define PATH_MGR_DISKDATAIO_H

#include <list>
#include <string>

using namespace std;

/**
 * 此处的磁盘读写不耗时，同时不太可能发生多进程竞争文件资源。此处目前不实现互斥机制。
 */
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
