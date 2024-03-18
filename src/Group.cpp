//
// Created by jaanonim on 12.03.24.
//

#include "Group.h"

Group::Group(nlohmann::json data) {
    name = data["name"];
    in_group = data["in_group"];
    out_group = data["out_group"];
    folder_path = data["folder_path"];
    if (!folder_path.ends_with("/")) folder_path += "/";
    if (data.contains("command")) {
        command = data["command"];
    }
}

Group::Group(std::string name, std::string in_group, std::string out_group, std::string folder_path,
             std::optional<std::string> command) {
    this->name = name;
    this->in_group = in_group;
    this->out_group = out_group;
    if (!folder_path.ends_with("/")) folder_path += "/";
    this->folder_path = folder_path;
    this->command = command;
}

nlohmann::json Group::to_json() {
    nlohmann::json data;
    data["name"] = name;
    data["in_group"] = in_group;
    data["out_group"] = out_group;
    data["folder_path"] = folder_path;
    if (command.has_value()) {
        data["command"] = command.value();
    }
    return data;
}
