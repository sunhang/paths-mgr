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

using namespace std;

ArgumentsParser &PathsMgr::getArgumentsParser() {
    return mArgumentsParser;
}

void PathsMgr::add() {
    char *cwd = get_current_dir_name();
    add(string(cwd));
    free(cwd);
}

void PathsMgr::add(string strCwd) {
    cout << "添加到列表 :" << strCwd << endl;

    list<string> paths = DiskDataIO::getInstance().loadFromDisk();
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

    DiskDataIO::getInstance().saveToDisk(paths);
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
    list<string> paths = DiskDataIO::getInstance().loadFromDisk();
    return paths;
}

void PathsMgr::del() {
    list<string> paths = DiskDataIO::getInstance().loadFromDisk();
    int index = stoi(mArgumentsParser.getArgv()[2]);
    if (index < 0 || index >= paths.size()) {
        cout << "下标越界" << endl;
    } else {
        list<string>::iterator it = paths.begin();
        advance(it, index);
        const string path = *it;
        paths.erase(it);
        DiskDataIO::getInstance().saveToDisk(paths);
        cout << "删除成功：" << path << endl;
    }
}

bool PathsMgr::cd() {
    string newDir;
    bool result = cd(newDir);
    // 约定最后一行是输出的路径，以便让shell解析它
    cout << newDir << endl;
    return result;
}

bool PathsMgr::cd(string &newDir) {
    list<string> paths = DiskDataIO::getInstance().loadFromDisk();
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
    } else if (mArgumentsParser.isCd()) {
        result = cd();
    }

    return result;
}
