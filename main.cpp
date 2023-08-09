#include <string>
#include "DiskDataIO.h"
#include "common.h"
#include "ArgumentsParser.h"
#include "PathsMgr.h"

// todo 研究一下switch case
// todo https://liam.page/2018/11/05/apply-C-style-string-in-switch-case-clause/
int main(int argc, char **argv) {
    DiskDataIO dataIo;
    dataIo.setup(get_workDir());

    ArgumentsParser parser(argc, argv);
    PathsMgr pathsMgr(parser, dataIo);
    pathsMgr.init();

    bool workResult = pathsMgr.doWork();
    pathsMgr.save();

    return workResult ? EXIT_SUCCESS : EXIT_FAILURE;
}
