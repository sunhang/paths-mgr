#include <string>
#include "DiskDataIO.h"
#include "common.h"
#include "ArgumentsParser.h"
#include "PathsMgr.h"

// todo 如何用最新迭代器以及迭代器原理
// todo 研究一下switch case
// todo https://liam.page/2018/11/05/apply-C-style-string-in-switch-case-clause/
// todo 项目文件结构得弄一弄
int main(int argc, char **argv) {
    DiskDataIO::getInstance().setup(get_workDir());
    ArgumentsParser parser(argc, argv);
    PathsMgr pathsMgr(parser);
    bool workResult = pathsMgr.doWork();
    return workResult ? EXIT_SUCCESS : EXIT_FAILURE;
}
