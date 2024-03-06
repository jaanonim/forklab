//
// Created by jaanonim on 06.03.24.
//
#define FMT_HEADER_ONLY

#include "fmt/core.h"
#include "fmt/color.h"
#include "Cli.h"

using namespace fmt;

Cli::Cli() {}

int Cli::run() {
    print("Hello, world!\n");


    if (args.size() == 1) {
        return run_interactive();
    }

    auto action = match_first_arg();
    if (action == Cli::ArgAction::INVALID) {
        print(fg(fmt::color::red) | fmt::emphasis::bold, "Invalid argument: ");
        print(fmt::emphasis::italic, args[1]);
        print(fmt::emphasis::bold, "\nHere is help for you:\n");
        print_help();
        return 100;
    }

    return 0;
}

Cli::ArgAction Cli::match_first_arg() {
    if (args[1] == "c" || args[1] == "create") {
        return Cli::ArgAction::CREATE;
    } else if (args[1] == "d" || args[1] == "delete") {
        return Cli::ArgAction::DELETE;
    } else if (args[1] == "l" || args[1] == "list") {
        return Cli::ArgAction::LIST;
    } else if (args[1] == "h" || args[1] == "help") {
        return Cli::ArgAction::HELP;
    } else if (args[1] == "auth") {
        return Cli::ArgAction::AUTH;
    }
    return Cli::ArgAction::INVALID;
}

void Cli::set_args(int argc, char **argv) {
    for (int i = 0; i < argc; ++i) {
        args.emplace_back(argv[i]);
    }
}

int Cli::run_interactive() {
    
    return 0;
}

void Cli::print_help() {
    print(
            fg(fmt::color::cyan) | fmt::emphasis::bold,
            "\n"
            "   __           _    _       _     \n"
            "  / _|         | |  | |     | |    \n"
            " | |_ ___  _ __| | _| | __ _| |__  \n"
            " |  _/ _ \\| '__| |/ / |/ _` | '_ \\ \n"
            " | || (_) | |  |   <| | (_| | |_) |\n"
            " |_| \\___/|_|  |_|\\_\\_|\\__,_|_.__/ \n"
            "                                   \n"
    );
    print("Tool for automated forking and cloning repos on gitlab.\n\n");
    print(fmt::emphasis::bold, "Usage: ");
    print("forklab [COMMAND]\n\n");
    print(fmt::emphasis::bold, "Options:\n");
    print("\t\tauth\t\tsets auth token for gitlab\n");
    print("\tc,\tcreate\t\tcreate new group interactively\n");
    print("\td,\tdelete\t\tdelete group of given index\n");
    print("\th,\thelp\t\tshows this help\n");
    print("\tl,\tlist\t\tlists groups\n");
}
