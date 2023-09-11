//
// Created by sunhang on 2023/9/8.
//

#include "Path.h"
#include <cfloat>
#include <cmath>

float Path::getFrequency() const {
    return mFrequency;
}

string Path::getStr() const {
    return mPath;
}

void Path::liftFrequency() {
    mFrequency += 1.0f;
}

void Path::reduceFrequency() {
    mFrequency -= 0.1f;
}

bool Path::operator==(const Path &p) const {
    return fabs((*this).mFrequency - p.mFrequency) < FLT_EPSILON && (*this).mPath == p.mPath;
}