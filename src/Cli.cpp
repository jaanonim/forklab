//
// Created by jaanonim on 06.03.24.
//
#define FMT_HEADER_ONLY

#include "fmt/core.h"
#include "fmt/color.h"
#include "Cli.h"
#include "Exceptions.h"
#include <iostream>
#include "inquirer.h"
#include "Gitlab.h"
#include <filesystem>
#include "spinners.hpp"

using namespace spinners;
using namespace fmt;
namespace fs = std::filesystem;

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
    } catch (HTTPError &e) {
        print(stderr, fg(fmt::color::red) | fmt::emphasis::bold, "{} {}", e.response_code, e.what());
        print(stderr, fg(fmt::color::red), "\n" + e.message);
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
        case REMOVE:
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
        return Cli::ArgAction::REMOVE;
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
    if (config->get_groups().empty()) {
        throw GenericError("Create group before running this command.\nTo create it run:\n\tforklab c");
    }

    auto *inquirer = new alx::Inquirer("");
    std::vector<std::string> v;
    for (const auto &group: config->get_groups()) v.push_back(group.name);
    inquirer->add_question({"name", "Select group", v});
    inquirer->ask();

    Group group = config->get_group(inquirer->answer("name"));
    Gitlab gitlab(config->getAuthToken().value_or(""));

    start_spinner("Loading ...");
    auto in_data = gitlab.getGroupProjects(group.in_group);
    stop_spinner(true);

    delete inquirer;
    inquirer = new alx::Inquirer("");
    v.clear();
    for (const auto &ele: in_data) v.push_back(ele.name);
    inquirer->add_question({"name", "Select project to fork", v});
    inquirer->ask();
    std::string name = inquirer->answer("name");
    delete inquirer;
    inquirer = nullptr;
    const ProjectData *p_data = std::find_if(in_data.begin(), in_data.end(),
                                             [&](auto ele) { return ele.name == name; }).base();


    start_spinner("Forking ...");
    try {
//        gitlab.createFork(p_data->id, group->out_group);
    }
    catch (HTTPError &e) {
        if (e.response_code == 409) {
            throw GenericError("Fork of this repo already exists in this group.");
        } else {
            throw;
        }
    }
    auto out_data = gitlab.getGroupProjects(group.out_group);
    stop_spinner();

    p_data = std::find_if(out_data.begin(), out_data.end(),
                          [&](auto ele) { return ele.name == name; }).base();

    if (p_data == nullptr) throw GenericError("Failed to fork repo.");


    if (!fs::is_directory(group.folder_path)) {
        throw GenericError(format("Directory `{}` don't exists.", group.folder_path));
    }
    if (fs::is_directory(group.folder_path + p_data->path)) {
        throw GenericError(format("Project directory `{}` already exists.", group.folder_path + p_data->path));
    }

    start_spinner("Cloning ...");
    int res = system(format("cd {} && git clone {} --quiet", group.folder_path, p_data->clone_url).c_str());
    stop_spinner();

    if (res != 0) throw GenericError(format("Cloning failed with code {}", res));

    if (group.command.has_value()) {
        start_spinner("Running command ...");
        res = system(format("cd {} && {}", group.folder_path + p_data->path, group.command.value()).c_str());
        stop_spinner();

        if (res != 0) throw GenericError(format("Command failed with code {}", res));
    }
    print(fg(fmt::color::green) | fmt::emphasis::bold, "DONE!");
}

void Cli::start_spinner(std::string text) {
    spinner = new Spinner();
    ((Spinner *) spinner)->setText(std::move(text));
    ((Spinner *) spinner)->setInterval(100);
    ((Spinner *) spinner)->setSymbols("dots");
    ((Spinner *) spinner)->start();
}

void Cli::stop_spinner(bool silent) {
    ((Spinner *) spinner)->stop();
    if (!silent) {
        print(fg(fmt::color::green), "✓");
        print("\n");
    }
    delete ((Spinner *) spinner);
    spinner = nullptr;
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
    start_spinner("Loading ...");
    Gitlab gitlab(config->getAuthToken().value_or(""));
    auto groups = gitlab.getGroups();
    std::vector<std::string> v;
    v.reserve(groups.size());
    for (const auto &group: groups) v.push_back(group.name);
    stop_spinner(true);

    auto inquirer = alx::Inquirer("Create group.");
    inquirer.add_question({"name", "How to call this group?", ".+"});
    inquirer.add_question({"in_group", "Group ID from witch group fork projects (eg. 1234567)?", "\\d+"});
    inquirer.add_question({"out_group", "To witch group fork to?", v});
    inquirer.add_question({"folder_path", "Where to clone those project (absolute folder_path)?", "^(/[^/ ]*)+/?$"});
    inquirer.add_question({"command", "Execute any command after cloning?:"});
    inquirer.ask();
    auto command = inquirer.answer("command");

    std::string name = inquirer.answer("out_group");
    auto id = std::find_if(groups.begin(), groups.end(),
                           [&](auto ele) { return ele.name == name; })->id;

    config->add_group(Group(
            inquirer.answer("name"),
            inquirer.answer("in_group"),
            id,
            inquirer.answer("folder_path"),
            command.empty() ? std::nullopt : std::make_optional(command)
    ));
}

void Cli::run_delete() {
    if (args.size() == 2) {
        run_delete_interactive();
        return;
    }
    if (args.size() != 3) {
        throw InvalidArgs("Expected to get only name of group after `delete`.");
    }

    if (config->del_group(args[2])) {
        print("Group named {} deleted.", args[2]);
    } else {
        throw GroupError("Cannot delete group named: " + args[2] + "\nProbably group doesn't exist.");
    }
}

void Cli::run_delete_interactive() {
    auto *inquirer = new alx::Inquirer("Delete group");
    std::vector<std::string> v;
    for (const auto &group: config->get_groups()) v.push_back(group.name);
    inquirer->add_question({"name", "Select group to delete", v});
    inquirer->ask();
    std::string s = inquirer->answer("name");
    if (config->del_group(s)) {
        print("Group named {} deleted.", s);
    } else {
        throw GroupError("Cannot delete group named: " + s + "\nProbably group doesn't exist.");
    }
}

void Cli::run_list() {
    print(fmt::emphasis::bold, "{:<16} {:<20} {:<20} {:<30} {:<30}\n", "Name", "In", "Out", "Path", "Command");
    for (const auto &group: config->get_groups()) {
        print("{:<16} {:<20} {:<20} {:<30} {:<30}\n", group.name, group.in_group, group.out_group, group.folder_path,
              group.command.value_or(""));
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

    if (spinner != nullptr) {
        delete ((Spinner *) spinner);
        spinner = nullptr;
    }
}
