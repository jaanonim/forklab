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

    ~Cli();

    void set_args(int argc, char *argv[]);

    int run();

private:
    std::vector<std::string> args;

    void _run();

    enum ArgAction {
        INVALID = 0,
        CREATE = 1,
        DELETE = 2,
        LIST = 3,
        AUTH = 4,
        HELP = 5,
    };

    ArgAction match_first_arg();

    void run_interactive();

    void run_create();

    void run_delete();

    void run_list();

    void run_auth();

    void run_help();


    void print_help();

    Config *config;

    void run_create_interactive();
};


#endif //FORKLAB_CLI_H
