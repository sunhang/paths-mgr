//
// Created by sunhang on 2023/8/1.
//

#include "common.h"
#include <pwd.h>
#include <unistd.h>
#include <filesystem>
#include <numeric>

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

unsigned char to_hex(unsigned char x) {
    return x > 9 ? x + 55 : x + 48;
}

string url_encode(const string &str) {
    string strTemp = "";
    size_t length = str.length();
    for (size_t i = 0; i < length; i++) {
        if (isalnum((unsigned char) str[i]) ||
            (str[i] == '-') ||
            (str[i] == '_') ||
            (str[i] == '.') ||
            (str[i] == '~'))
            strTemp += str[i];
        else if (str[i] == ' ')
            strTemp += "+";
        else {
            strTemp += '%';
            strTemp += to_hex((unsigned char) str[i] >> 4);
            strTemp += to_hex((unsigned char) str[i] % 16);
        }
    }
    return strTemp;
}

string join(list<string> &strings, string delim) {
    return accumulate(strings.begin(), strings.end(), string(),
                      [&delim](string &x, string &y) {
                          return x.empty() ? y : x + delim + y;
                      });
}

vector<string> custom_split(string s, string del) {
    vector<string> result;
    // Use find function to find 1st position of delimiter.
    int end = s.find(del);
    while (end != -1) { // Loop until no delimiter is left in the string.
        result.push_back(s.substr(0, end));
        s.erase(s.begin(), s.begin() + end + 1);
        end = s.find(del);
    }
    result.push_back(s.substr(0, end));

    return result;
}