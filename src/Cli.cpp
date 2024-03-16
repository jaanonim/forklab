//
// Created by jaanonim on 06.03.24.
//
#define FMT_HEADER_ONLY

#include "fmt/core.h"
#include "fmt/color.h"
#include "Cli.h"
#include "Exceptions.h"
#include <iostream>


using namespace fmt;

int Cli::run() {
    try {
        _run();
    }
    catch (InvalidArgs &e) {
        print(stderr, fg(fmt::color::red) | fmt::emphasis::bold, e.what());
        print(stderr, fg(fmt::color::red), "\n" + e.message);
        if (e.print_help) {
            print(fmt::emphasis::bold, "\nHere is help for you:\n");
            print_help();
            print("\n");
        }
        return e.code;
    }
    catch (MessageException &e) {
        print(stderr, fg(fmt::color::red) | fmt::emphasis::bold, e.what());
        print(stderr, fg(fmt::color::red), "\n" + e.message);
        return e.code;
    }
    catch (CodeException &e) {
        print(stderr, fg(fmt::color::red) | fmt::emphasis::bold, e.what());
        return e.code;
    }
    catch (std::exception &e) {
        print(stderr, fg(fmt::color::red) | fmt::emphasis::bold, e.what());
        return 255;
    }
    return 0;
}

void Cli::_run() {
    config = new Config();

    if (args.size() == 1) {
        return run_interactive();
    }

    auto action = match_first_arg();

    switch (action) {
        case CREATE:
            return run_create();
        case DELETE:
            return run_delete();
        case LIST:
            return run_list();
        case AUTH:
            return run_auth();
        case HELP:
            return run_help();
        case INVALID:
            throw InvalidArgs("Invalid argument: " + args[1], true);
    }

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

void Cli::run_interactive() {

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

void Cli::run_create() {
    switch (args.size()) {
        case 2:
            run_create_interactive();
            break;
        case 6:
            if (config->add_group(Group(args[2], args[3], args[4], args[5]))) {
                print("Group named {} created.", args[2]);
            } else {
                throw GroupError("Cannot create group named: " + args[2] + "\nProbably group already exist.");
            }
            break;
        case 7:
            if (config->add_group(Group(args[2], args[3], args[4], args[5], args[6]))) {
                print("Group named {} created.", args[2]);
            } else {
                throw GroupError("Cannot create group named: " + args[2] + "\nProbably group already exist.");
            }
            break;
        default:
            throw InvalidArgs("Command create expects 0, 4 or 7 arguments.");
    }
}

void Cli::run_create_interactive() {

}

void Cli::run_delete() {
    if (args.size() != 3) {
        throw InvalidArgs("Expected to get only name of group after `create`.");
    }
    if (config->del_group(args[2])) {
        print("Group named {} deleted.", args[2]);
    } else {
        throw GroupError("Cannot delete group named: " + args[2] + "\nProbably group doesn't exist.");
    }
}

void Cli::run_list() {
    print(fmt::emphasis::bold,"{:<16} {:<20} {:<20} {:<30} {:<30}\n", "Name","In","Out","Path","Command");
    for (const auto& group : config->get_groups()) {
        print("{:<16} {:<20} {:<20} {:<30} {:<30}\n", group.name, group.in_group,group.out_group,group.path,group.command.value_or(""));
    }
}

void Cli::run_auth() {
    if (args.size() != 3) {
        throw InvalidArgs("Expected to get only token after `auth`.");
    }
    config->setAuthToken(args[2]);
    print("Auth token saved.");
}

void Cli::run_help() {
    print_help();
}

Cli::~Cli() {
    delete config;
    config = nullptr;
}
