//
// Created by jaanonim on 06.03.24.
//

#ifndef FORKLAB_CLI_H
#define FORKLAB_CLI_H

#include <vector>
#include <string>

class Cli {
public:
    Cli();

    void set_args(int argc, char *argv[]);

    int run();

private:
    std::vector<std::string> args;


    enum ArgAction {
        INVALID = 0,
        CREATE = 1,
        DELETE = 2,
        LIST = 3,
        AUTH = 4,
        HELP = 5,
    };

    ArgAction match_first_arg();

    int run_interactive();

    void print_help();
};


#endif //FORKLAB_CLI_H
