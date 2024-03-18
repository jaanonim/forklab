#include "Cli.h"

int main(int argc, char *argv[]) {
    Cli c;
    c.set_args(argc, argv);
    int res = c.run();
    printf("\n");
    return res;
}
