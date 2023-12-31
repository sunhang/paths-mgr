//
// Created by sunhang on 2023/9/8.
//

#ifndef PATH_MGR_PATHINFO_H
#define PATH_MGR_PATHINFO_H

#include <string>

using namespace std;

// todo 这个可以做成动态配置项
#define DEFAULT_FREQUENCY 16

// todo C++中struct的使用方式需要了解一下
class Path {
public:
    Path(string path) {
        // todo 如何调用 Path(DEFAULT_FREQUENCY, string path)
        this->mFrequency = DEFAULT_FREQUENCY;
        this->mPath = path;
    }

    Path(float frequency, string path) {
        this->mFrequency = frequency;
        this->mPath = path;
    }

    Path &operator=(const Path &pi) {
        this->mFrequency = pi.mFrequency;
        this->mPath = pi.mPath;
        return *this;
    }

    Path(const Path &pi) {
        this->mFrequency = pi.mFrequency;
        this->mPath = pi.mPath;
    }

    float getFrequency() const;

    string getStr() const;

    void liftFrequency();

    void reduceFrequency();

    // todo 需要研究下重载equal
    bool operator==(const Path &rhs) const;

private:
    float mFrequency;
    string mPath;
};

#endif //PATH_MGR_PATHINFO_H
