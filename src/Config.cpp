//
// Created by jaanonim on 12.03.24.
//

#include "Config.h"
#include <json.hpp>
#include <fstream>
#include <iostream>
#include "Exceptions.h"
#include <filesystem>

using json = nlohmann::json;
namespace fs = std::filesystem;


std::optional<std::string> Config::getAuthToken() {
    return auth_token;
}

void Config::setAuthToken(const std::string &t) {
    auth_token = t;
    save();
}

int Config::setup() {
    char *p = getenv("FORKLAB_CONFIG_PATH");
    if (p == nullptr) {
        p = getenv("HOME");
        if (p == nullptr) {
            return 10;
        } else {
            path = (std::string) p + "/.config/";
        }

    } else {
        path = p;
    }

    if (!path.ends_with("/")) path += "/";

    if (!fs::exists(path)) {
        fs::create_directory(path);
    }

    std::ifstream file(path + file_name);
    if (file) {
        try {
            data = json::parse(file);
        }
        catch (json::parse_error &ex) {
            std::cerr << "Parse error when loading config at: " << path << std::endl;
            std::cerr << ex.what() << std::endl;
            return 11;
        }
    }
    file.close();

    return 0;
}

void Config::save() {
    std::cout << path << std::endl;
    std::cout << data << std::endl;

    std::ofstream o(path + file_name);
    if (!o.good()) {
        std::cerr << "Cannot save file to: " << path << std::endl;
        throw CannotSave();
    }
    o << std::setw(4) << data << std::endl;
    o.close();
}

void Config::add_group(const Group &g) {
    groups.push_back(g);
}

bool Config::del_group(int id) {
    if (id < 0 || id >= groups.size()) return false;
    groups.erase(groups.begin() + id);
    return true;
}

std::vector<Group> Config::get_groups() {
    return groups;
}

