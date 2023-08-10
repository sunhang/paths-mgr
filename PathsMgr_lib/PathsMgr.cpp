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

using namespace std;

ArgumentsParser &PathsMgr::getArgumentsParser() {
    return mArgumentsParser;
}

DiskDataIO &PathsMgr::getDiskDataIO() {
    return mDiskDataIo;
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

    list<string> &paths = mPaths;
    list<string>::iterator it = find(paths.begin(), paths.end(), strCwd);
    if (it == paths.end()) {
        paths.push_front(strCwd);
        if (paths.size() > mMax) {
            paths.erase(--paths.end());
        }
    } else {
        string str = *it;
        paths.erase(it);
        paths.push_front(str);
    }
}

void PathsMgr::ls() {
    list<string> paths = getPaths();

    string homeDir = get_home_dir();
    // print
    for (int i{0}; auto item: paths) {
        cout << format(i, item, homeDir) << endl;
        ++i;
    }
}

string PathsMgr::format(int number, string &path, string &homeDir) {
    filesystem::path p(path);
    string simpleName = p.stem().c_str();

    // replace homedir with '~'
    string strPath = path;
    if (strPath.find(homeDir, 0) == 0) {
        strPath = "~" + strPath.substr(homeDir.size());
    }
    return to_string(number) + ": " + simpleName + "\t" + strPath;
}

list<string> PathsMgr::getPaths() {
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
    list<string> &paths = mPaths;
    set<string> pathsSet;
    list<string> result;
    for (auto item: paths) {
        filesystem::path p(item);
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
    list<string> &paths = mPaths;
    list<string> result;
    for (auto item: paths) {
        filesystem::path p(item);
        string simpleName = p.stem().c_str();

        if (simpleName.find(dirName, 0) == 0) {
            result.push_back(item);
        }
    }
    return result;
}

void PathsMgr::del() {
    list<string> &paths = mPaths;
    int index = stoi(mArgumentsParser.getArgv()[2]);
    if (index < 0 || index >= paths.size()) {
        cout << "下标越界" << endl;
    } else {
        list<string>::iterator it = paths.begin();
        advance(it, index);
        const string path = *it;
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
    list<string>& paths = mPaths;
    int index = stoi(mArgumentsParser.getArgv()[1]);
    if (index < 0 || index >= paths.size()) {
        cout << "下标越界" << endl;
        return false;
    } else {
        list<string>::iterator it = paths.begin();
        advance(it, index);
        newDir = *it;

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
    }

    return result;
}
