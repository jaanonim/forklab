//
// Created by jaanonim on 17.03.24.
//

#ifndef FORKLAB_GITLAB_H
#define FORKLAB_GITLAB_H

#define CPPHTTPLIB_OPENSSL_SUPPORT

#include "httplib.h"
#include <string>
#include <vector>
#include <json.hpp>

struct ProjectData {
    std::string name;
    std::string id;
};


class Gitlab {
public:
    Gitlab(std::string);

    ~Gitlab();

    std::vector<ProjectData> getGroupProjects(std::string);

protected:
    nlohmann::json get(std::string url);

private:
    const std::string auth_token;
    httplib::Client *http_client;
};


#endif //FORKLAB_GITLAB_H
