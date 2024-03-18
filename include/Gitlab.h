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
    std::string clone_url;
    std::string path;
};


class Gitlab {
public:
    Gitlab(std::string);

    ~Gitlab();

    std::vector<ProjectData> getGroupProjects(const std::string &);

    void createFork(const std::string &, const std::string &);

protected:
    nlohmann::json get(const std::string &url);

    nlohmann::json post(const std::string &url);

    void check_errors(httplib::Result &);

private:
    const std::string auth_token;
    httplib::Client *http_client;

};


#endif //FORKLAB_GITLAB_H
