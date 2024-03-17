//
// Created by jaanonim on 17.03.24.
//

#include "Gitlab.h"
#include <string>
#include <vector>
#include <json.hpp>
#include "Exceptions.h"

#define CPPHTTPLIB_OPENSSL_SUPPORT

#include "httplib.h"

using json = nlohmann::json;

Gitlab::Gitlab(std::string auth_token) : auth_token(auth_token) {
    http_client = new httplib::Client("https://gitlab.com/api/v4");
}

std::vector<ProjectData> Gitlab::getGroupProjects(std::string) {
    return std::vector<ProjectData>();
}

nlohmann::json Gitlab::get(std::string url) {

    auto res = http_client->Get(url);
    if (res->status != 200) {
        if (res->status == 401) {
            throw Unauthorized();
        }
        json data = json::parse(res->body);
        std::string message = data["message"];
        throw HTTPError(message, res->status);
    }
    return json::parse(res->body);
}

Gitlab::~Gitlab() {
    delete http_client;
    http_client = nullptr;
}
