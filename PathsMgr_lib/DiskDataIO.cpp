//
// Created by sunhang on 23-7-26.
//

#include "DiskDataIO.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sys/stat.h>
#include <cstdio>
#include <algorithm>

using namespace std;

string DiskDataIO::getDataPath() {
    return mStrWorkDir + filesystem::path::preferred_separator + mData;
}

string DiskDataIO::getLogPath() {
    return mStrWorkDir + filesystem::path::preferred_separator + mLog;
}

void DiskDataIO::setup(const string strWorkDir) {
    bool shouldCreateDir = true;
    struct stat sb;
    if (stat(strWorkDir.c_str(), &sb) == 0) { // 如果存在
        if (sb.st_mode & S_IFDIR) { // 如果是文件夹
            shouldCreateDir = false;
        } else {
            cout << "存在同名文件" << endl;
            remove(strWorkDir.c_str());
        }
    }

    if (shouldCreateDir) {
        try {
            error_code ec;
            auto createDirResult = filesystem::create_directories(strWorkDir, ec);
            string str = "create_dir_result:" + to_string(createDirResult) + " error_code:" + ec.message();
            writeToLog(str);
        } catch (exception &e) {
            cerr << e.what() << endl;
            writeToLog(e.what());
        }
    }
    this->mStrWorkDir = strWorkDir;
}

// todo 如果没有类，文件的命名是否小写？
list<string> DiskDataIO::loadFromDisk() {
    ifstream inf{getDataPath(), ifstream::in};

    if (!inf) {
        writeToLog("Uh oh, file could not be opened for reading!");
        return {};
    }

    list<string> paths;
    string line;
    while (getline(inf, line)) {
        paths.push_back(line);
    }

    return paths;
}

void DiskDataIO::saveToDisk(list<string> paths) {
    ofstream out{getDataPath()};
    list<string>::iterator it;
    for (it = paths.begin(); it != paths.end(); it++) {
        string str = *it;
        out << str << endl;
    }
    out.close();
}

void DiskDataIO::writeToLog(string log) {
    tm tm;
    time_t now = time(0);
    localtime_r(&now, &tm);
    char tmp[32] = {0};
    strftime(tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S", &tm);

    ofstream out{getLogPath(), ios::app};
    out << tmp << "\t" << log << endl;
    out.close();
}

int DiskDataIO::getLogLineCount() {
    ifstream inFile(getLogPath());
    int result = count(istreambuf_iterator<char>(inFile),
                       istreambuf_iterator<char>(), '\n');
    return result;
}

void DiskDataIO::clear() {
    remove(getDataPath().c_str());
    remove(getLogPath().c_str());
    remove(mStrWorkDir.c_str());
}