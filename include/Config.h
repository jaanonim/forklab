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
    int setup();

    std::optional<std::string> getAuthToken();

    void setAuthToken(const std::string &);

    void add_group(const Group &g);

    bool del_group(int);

    std::vector<Group> get_groups();


private:
    std::optional<std::string> auth_token;
    std::vector<Group> groups;

    std::string path;
    std::string file_name = "forklab.json";
    nlohmann::json data;

    void save();
};


#endif //FORKLAB_CONFIG_H
