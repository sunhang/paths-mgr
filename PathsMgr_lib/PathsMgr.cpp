//
// Created by sunhang on 23-7-26.
//

#include "PathsMgr.h"
#include "DiskDataIO.h"
#include "common.h"
#include <iostream>
#include <unistd.h>
#include <algorithm>
#include <filesystem>
#include <set>
#include <cmath>
#include <cfloat>

using namespace std;

ArgumentsParser &PathsMgr::getArgumentsParser() {
    return mArgumentsParser;
}

DiskDataIO &PathsMgr::getDiskDataIO() {
    return mDiskDataIo;
}

void PathsMgr::refreshFrequency() {
    string target = mArgumentsParser.getArgv()[2];

    auto &paths = getPaths();
    for (auto &item: paths) {
        if (item.getStr() == target) {
            item.liftFrequency();
        } else {
            item.reduceFrequency();
        }
    }
}

void PathsMgr::add() {
    char *cwd = get_current_dir_name();
    add(string(cwd));
    free(cwd);
}

void PathsMgr::add(string strCwd) {
    if (strCwd == "/") {
        cout << "根目录没有必要添加到列表中" << endl;
        return;
    }

    cout << "添加到列表 :" << strCwd << endl;

    list<Path> &paths = mPaths;
    list<Path>::iterator it = find_if(paths.begin(), paths.end(), [strCwd](const Path &p) {
        return p.getStr() == strCwd;
    });

    if (it == paths.end()) {
        while (paths.size() >= mMax) {
            list<Path>::iterator it = min_element(paths.begin(), paths.end(), [](const auto &a, const auto &b) {
                return a.getFrequency() < b.getFrequency()
                       || fabs(a.getFrequency() - b.getFrequency()) < FLT_EPSILON;
            });
            paths.erase(it);
        }
        paths.push_front(Path(strCwd));
    } else {
        Path p = *it;
        paths.erase(it);
        paths.push_front(p);
    }
}

void PathsMgr::ls() {
    list<Path> paths = getPaths();

    string homeDir = get_home_dir();
    int columns = query_terminal_columns();
    // print
    for (int i{0}; auto item: paths) {
        string strPath = item.getStr();
        string outstr = format(i, strPath, homeDir, item.getFrequency(), columns);
        cout << outstr << endl;
        ++i;
    }
}

string PathsMgr::format(int number, string &path, string &homeDir, float freq, int columns) {
    filesystem::path p(path);
    string simpleName = p.filename().c_str();

    // replace homedir with '~'
    string strPath = path;
    if (strPath.find(homeDir, 0) == 0) {
        strPath = "~" + strPath.substr(homeDir.size());
    }

    ostringstream oss;
    oss << to_string(number) << ") " << simpleName << "  " << strPath << ":" << freq;

    string str = oss.str();

    if (str.size() > columns && columns > 3) {
        str = str.erase(columns - 3) + "...";
    }
    return str;
}

list<Path> &PathsMgr::getPaths() {
    return mPaths;
}

void PathsMgr::outFilteredDirNames() {
    string namePrefix = mArgumentsParser.getArgv()[2];

    list<string> paths = filterByDirName(namePrefix);
    transform(paths.begin(), paths.end(), paths.begin(), ::url_encode);
    string str{";"};
    str += join(paths, ":");
    cout << str << endl;
}

list<string> PathsMgr::filterByDirName(string namePrefix) {
    list<Path> &paths = mPaths;
    set<string> pathsSet;
    list<string> result;
    for (auto item: paths) {
        filesystem::path p(item.getStr());
        string simpleName = p.stem().c_str();

        if (simpleName.find(namePrefix, 0) == 0 && pathsSet.find(simpleName) == pathsSet.end()) {
            result.push_back(simpleName);
            pathsSet.insert(simpleName);
        }
    }
    return result;
}

bool PathsMgr::outCdByDirName() {
    string dirName = mArgumentsParser.getArgv()[2];

    list<string> newDirs = cdByDirName(dirName);
    if (newDirs.size() == 0) {
        return false;
    } else {
        transform(newDirs.begin(), newDirs.end(), newDirs.begin(), ::url_encode);
        string str{";"};
        str += join(newDirs, ":");
        cout << str << endl;
        return true;
    }
}

// todo 应该不是文件名字而是目录名字，检查整个工程有没有相同的命名错误
list<string> PathsMgr::cdByDirName(string dirName) {
    list<Path> &paths = mPaths;
    list<string> result;
    for (auto item: paths) {
        filesystem::path p(item.getStr());
        string simpleName = p.stem().c_str();

        if (simpleName.find(dirName, 0) == 0) {
            result.push_back(item.getStr());
        }
    }
    return result;
}

void PathsMgr::del() {
    list<Path> &paths = mPaths;
    int index = stoi(mArgumentsParser.getArgv()[2]);
    if (index < 0 || index >= paths.size()) {
        cout << "下标越界" << endl;
    } else {
        list<Path>::iterator it = paths.begin();
        advance(it, index);
        const string path = it->getStr();
        paths.erase(it);
        cout << "删除成功：" << path << endl;
    }
}

bool PathsMgr::cdByNumber() {
    string newDir;
    bool result = cdByNumber(newDir);
    // 约定最后一行是输出的路径，以便让shell解析它
    if (result) {
        string str{";"};
        str += url_encode(newDir);
        cout << str << endl;
    }
    return result;
}

bool PathsMgr::cdByNumber(string &newDir) {
    list<Path> &paths = mPaths;
    int index = stoi(mArgumentsParser.getArgv()[1]);
    if (index < 0 || index >= paths.size()) {
        cout << "下标越界" << endl;
        return false;
    } else {
        list<Path>::iterator it = paths.begin();
        advance(it, index);
        newDir = it->getStr();

        return true;
    }
}

void PathsMgr::outSubCommandsInfo() {
    list<string> subCommands = {"ls", "a", "d", "cd"};
    string str = ";" + join(subCommands, ":");
    cout << str << endl;
}

void PathsMgr::clearData() {
    mPaths.clear();
}

void PathsMgr::init() {
    // todo 这样写感觉有问题，过多调用构造函数
    mPaths = mDiskDataIo.loadFromDisk();
}

void PathsMgr::save() {
    mDiskDataIo.saveToDisk(mPaths);
}

bool PathsMgr::doWork() {
    if (!mArgumentsParser.isArgumentsCorrect()) {
        cout << "不支持的参数" << endl;
        return false;
    }

    bool result = true;
    if (mArgumentsParser.isLs()) {
        ls();
    } else if (mArgumentsParser.isAdd()) {
        add();
    } else if (mArgumentsParser.isDel()) {
        del();
    } else if (mArgumentsParser.isNumber()) {
        result = cdByNumber();
    } else if (mArgumentsParser.isPredict()) {
        outFilteredDirNames();
    } else if (mArgumentsParser.isCd()) {
        result = outCdByDirName();
    } else if (mArgumentsParser.isRequestSubCommandsInfo()) {
        outSubCommandsInfo();
    } else if (mArgumentsParser.isRefreshFrequency()) {
        refreshFrequency();
    }

    return result;
}
