//
// Created by sunhang on 2023/8/1.
//

#ifndef PATH_MGR_ARGUMENTSPARSER_H
#define PATH_MGR_ARGUMENTSPARSER_H

#include <string>
#include <vector>

using namespace std;

class ArgumentsParser {
public:
    ArgumentsParser();
    ArgumentsParser(int argc, char **argv);

    void setup(int argc, char **argv);

    bool isLs();

    bool isAdd();

    bool isDel();

    bool isCd();

    bool isArgumentsCorrect();

    const vector<string> &getArgv() const;

private:
    vector<string> mArgv;
};


#endif //PATH_MGR_ARGUMENTSPARSER_H
