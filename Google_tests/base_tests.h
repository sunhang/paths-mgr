//
// Created by sunhang on 2023/8/6.
//

#ifndef PATH_MGR_BASE_TESTS_H
#define PATH_MGR_BASE_TESTS_H

// todo 需要把个数也放进去
#define ARGUMENTS(...) const_cast<char **>(std::begin({__VA_ARGS__}))
#define PATHS_MGR_CMD "pa"
#define EXPECT_PATH_STR_EQ(expected, actual) EXPECT_EQ(expected, (actual).getStr())

#endif //PATH_MGR_BASE_TESTS_H
