#include "Cli.h"
#include "fmt/core.h"

using namespace fmt;

int main(int argc, char *argv[]) {
    Cli c;
    c.set_args(argc, argv);
    int res = c.run();
    print("\n");
    return res;
}
