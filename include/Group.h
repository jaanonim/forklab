//
// Created by jaanonim on 12.03.24.
//

#ifndef FORKLAB_GROUP_H
#define FORKLAB_GROUP_H

#include <json.hpp>
#include <string>


class Group {
public:
    Group(nlohmann::json data);

    Group(std::string name,
          std::string in_group,
          std::string out_group,
          std::string folder_path,
          std::optional<std::string> command = {});

    std::string name;
    std::string in_group;
    std::string out_group;
    std::string folder_path;
    std::optional<std::string> command;

    nlohmann::json to_json();
};


#endif //FORKLAB_GROUP_H
