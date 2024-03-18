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

Config::Config() {
    char *p = getenv("FORKLAB_CONFIG_PATH");
    if (p == nullptr) {
        p = getenv("HOME");
        if (p == nullptr) {
            throw MissingHomeEnv();
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
            json data = json::parse(file);
            from_json(data);
        }
        catch (json::parse_error &ex) {
            throw CannotReadConfig("Parse error when loading config at: " + path + "\n" + ex.what());
        }
    }
    file.close();
}

void Config::save() {
    std::ofstream o(path + file_name);
    if (!o.good()) {
        std::cerr << "Cannot save file to: " << path << std::endl;
        throw CannotSaveConfig();
    }
    o << std::setw(4) << to_json() << std::endl;
    o.close();
}

bool Config::add_group(const Group &g) {
    if (any_of(groups.begin(), groups.end(), [&](const Group &elem) { return elem.name == g.name; })) {
        return false;
    }
    groups.push_back(g);
    save();
    return true;
}

bool Config::del_group(int id) {
    if (id < 0 || id >= groups.size()) return false;
    groups.erase(groups.begin() + id);
    save();
    return true;
}

bool Config::del_group(const std::string &name) {
    for (int i = 0; i < groups.size(); ++i) {
        if (groups[i].name == name) {
            return del_group(i);
        }
    }
    return false;
}

std::vector<Group> Config::get_groups() {
    return groups;
}

nlohmann::json Config::to_json() {
    json data;
    if (auth_token.has_value())
        data["auth"] = auth_token.value();

    data["groups"] = json::array();
    for (auto group: groups) {
        data["groups"].push_back(group.to_json());
    }

    return data;
}

void Config::from_json(nlohmann::json data) {
    if (data.contains("auth")) {
        auth_token = data["auth"];
    }
    if (data.contains("groups") && data["groups"].is_array()) {
        groups.clear();
        for (const auto &j: data["groups"]) {
            groups.emplace_back(j);
        }
    }
}

Group Config::get_group(const std::string &name) {
    for (auto g: groups) {
        if (g.name == name) {
            return g;
        }
    }
}
