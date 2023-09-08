//
// Created by sunhang on 2023/9/8.
//

#include "Path.h"

int Path::getFrequency() const {
    return mFrequency;
}

string Path::getStr() const {
    return mPath;
}

void Path::increaseFrequency() {
    mFrequency++;
}

void Path::decreaseFrequency() {
    mFrequency--;
}

bool Path::operator==(const Path &p) const {
    return (*this).mFrequency == p.mFrequency && (*this).mPath == p.mPath;
}