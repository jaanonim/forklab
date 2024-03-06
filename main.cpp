#include "Cli.h"

int main(int argc, char* argv[]) {
    Cli c;
    c.set_args(argc,argv);
    return c.run();
}
