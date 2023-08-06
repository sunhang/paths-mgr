//
// Created by sunhang on 2023/8/1.
//

#include "ArgumentsParser.h"
#include "common.h"
#include <iostream>

ArgumentsParser::ArgumentsParser() {}

ArgumentsParser::ArgumentsParser(int argc, char **argv) {
    setup(argc, argv);
}

void ArgumentsParser::setup(int argc, char **argv) {
    mArgv.clear();
    for (int i = 0; i < argc; i++) {
        mArgv.push_back(string(argv[i]));
    }
}

bool ArgumentsParser::isArgumentsCorrect() {
    return isLs() || isAdd() || isDel()
           || isNumber() || isPredict() || isCd()
           || isRequestSubCommandsInfo();
}

bool ArgumentsParser::isLs() {
    return mArgv.size() == 1 || mArgv.size() == 2 && mArgv[1] == "ls";
}

bool ArgumentsParser::isAdd() {
    return mArgv.size() == 2 && mArgv[1] == "a";
}

bool ArgumentsParser::isDel() {
    return mArgv.size() == 3 && mArgv[1] == "d" && is_number(mArgv[2]);
}

bool ArgumentsParser::isNumber() {
    return mArgv.size() == 2 && is_number(mArgv[1]);
}

bool ArgumentsParser::isPredict() {
    return mArgv.size() == 3 && mArgv[1] == "predict";
}

bool ArgumentsParser::isCd() {
    return mArgv.size() == 3 && mArgv[1] == "cd";
}

bool ArgumentsParser::isRequestSubCommandsInfo() {
    return mArgv.size() == 2 && mArgv[1] == "subcommands";
}

const vector<string> &ArgumentsParser::getArgv() const {
    return mArgv;
}
