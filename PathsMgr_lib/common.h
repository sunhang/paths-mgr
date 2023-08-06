//
// Created by sunhang on 2023/8/1.
//

#ifndef PATH_MGR_COMMON_H
#define PATH_MGR_COMMON_H

#include <string>
#include <algorithm>
#include <list>

using namespace std;

bool is_number(const string &s);

string get_workDir();

string get_home_dir();

string url_encode(const string &str);

string join(list<string> &strings, string delim);

#endif //PATH_MGR_COMMON_H
