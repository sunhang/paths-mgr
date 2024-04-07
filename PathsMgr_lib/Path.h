//
// Created by sunhang on 2023/9/8.
//

#ifndef PATH_MGR_PATHINFO_H
#define PATH_MGR_PATHINFO_H

#include <string>
#include <utility>

using namespace std;

// todo 这个可以做成动态配置项
#define DEFAULT_FREQUENCY 16

class Path {
public:
    explicit Path(string path) : Path(DEFAULT_FREQUENCY, std::move(path)) {}

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

    bool operator==(const Path &rhs) const;

private:
    float mFrequency;
    string mPath;
};

#endif //PATH_MGR_PATHINFO_H
