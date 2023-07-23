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
class DiskDataIO {
public:
    /**
     * todo 单例能移动到cpp里吗
     */
    static DiskDataIO &getInstance() {
        static DiskDataIO inst;
        return inst;
    }

    void setup(const string strWorkDir);

    list <string> loadFromDisk();

    void saveToDisk(list <string> paths);

    void writeToLog(string log);

    int getLogLineCount();

    void clear();

private:
    // todo 以下有没有遗漏
    DiskDataIO() {};

    ~DiskDataIO() {};

    string getDataPath();

    string getLogPath();

    DiskDataIO(const DiskDataIO &);

    DiskDataIO &operator=(const DiskDataIO &);

private:
    string mStrWorkDir;
    const string mData = "data";
    const string mLog = "log";
};

#endif // PATH_MGR_DISKDATAIO_H
