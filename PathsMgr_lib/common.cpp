//
// Created by sunhang on 2023/8/1.
//

#include "common.h"
#include <pwd.h>
#include <unistd.h>
#include <filesystem>

bool is_number(const string &s) {
    return !s.empty()
           && find_if(s.begin(),
                           s.end(),
                           [](unsigned char c) { return !isdigit(c); }) == s.end();
}

string get_home_dir() {
    struct passwd *pw = getpwuid(getuid());
    const char *homedir = pw->pw_dir;
    return string(homedir);
}

string get_workDir() {
    // todo string format实现
    // todo file path 拼接的高级方法
    string strWorkDir = get_home_dir() + filesystem::path::preferred_separator + ".local/share/paths-mgr";
    return strWorkDir;
}