//
// Created by sunhang on 2023/8/1.
//

#include "ArgumentsParser.h"
#include "common.h"

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
    if (isLs() || isAdd() || isDel() || isCd()) {
        return true;
    }
    return false;
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

bool ArgumentsParser::isCd() {
    return mArgv.size() == 2 && is_number(mArgv[1]);
}

const vector<string> &ArgumentsParser::getArgv() const {
    return mArgv;
}
