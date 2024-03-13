//
// Created by jaanonim on 12.03.24.
//

#ifndef FORKLAB_CONFIG_H
#define FORKLAB_CONFIG_H

#include <string>
#include <json.hpp>
#include "Group.h"

class Config {
public:
    Config();

    std::optional<std::string> getAuthToken();

    void setAuthToken(const std::string &);

    bool add_group(const Group &g);

    bool del_group(int);

    bool del_group(const std::string &);

    std::vector<Group> get_groups();


private:
    std::optional<std::string> auth_token;
    std::vector<Group> groups;

    std::string path;
    std::string file_name = "forklab.json";

    void save();

    nlohmann::json to_json();

    void from_json(nlohmann::json);
};


#endif //FORKLAB_CONFIG_H
