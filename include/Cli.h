//
// Created by jaanonim on 06.03.24.
//

#ifndef FORKLAB_CLI_H
#define FORKLAB_CLI_H

#include <vector>
#include <string>
#include "Config.h"

class Cli {
public:

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

    int run_create();

    int run_delete();

    int run_list();

    int run_auth();

    int run_help();


    void print_help();

    Config config;
};


#endif //FORKLAB_CLI_H
