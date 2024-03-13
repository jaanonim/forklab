//
// Created by jaanonim on 12.03.24.
//

#include "Group.h"

Group::Group(nlohmann::json data) {
    name = data["name"];
    in_group = data["in_group"];
    out_group = data["out_group"];
    path = data["path"];
    if (data.contains("command")) {
        command = data["command"];
    }
}

Group::Group(std::string name, std::string in_group, std::string out_group, std::string path,
             std::optional<std::string> command) {
    this->name = name;
    this->in_group = in_group;
    this->out_group = out_group;
    this->path = path;
    this->command = command;
}

nlohmann::json Group::to_json() {
    nlohmann::json data;
    data["name"] = name;
    data["in_group"] = in_group;
    data["out_group"] = out_group;
    data["path"] = path;
    if (command.has_value()) {
        data["command"] = command.value();
    }
    return data;
}
