//
// Created by jaanonim on 17.03.24.
//

#define CPPHTTPLIB_OPENSSL_SUPPORT;

#include "Gitlab.h"
#include <string>
#include <utility>
#include <vector>
#include <json.hpp>
#include "Exceptions.h"
#include "httplib.h"

using json = nlohmann::json;

Gitlab::Gitlab(std::string auth_token) : auth_token(std::move(auth_token)) {
    http_client = new httplib::Client("https://gitlab.com");
    http_client->enable_server_certificate_verification(true);
}

Gitlab::~Gitlab() {
    delete http_client;
    http_client = nullptr;
}

std::vector<ProjectData> Gitlab::getGroupProjects(const std::string &group_id) {
    json data = get("/api/v4/groups/" + group_id + "/projects");
    std::vector<ProjectData> res;
    for (auto ele: data) {
        res.push_back({ele["name"], to_string(ele["id"]), ele["ssh_url_to_repo"], ele["path"]});
    }
    return res;
}


std::vector<GroupData> Gitlab::getGroups() {
    json data = get("/api/v4/groups");
    std::vector<GroupData> res;
    for (auto ele: data) {
        res.push_back({ele["full_name"], to_string(ele["id"])});
    }
    return res;
}


nlohmann::json Gitlab::get(const std::string &url) {
    httplib::Headers headers = {
            {"Authorization", "Bearer " + auth_token}
    };
    auto res = http_client->Get(url, headers);
    check_errors(res);
    return json::parse(res->body);
}

nlohmann::json Gitlab::post(const std::string &url) {
    httplib::Headers headers = {
            {"Authorization", "Bearer " + auth_token}
    };
    auto res = http_client->Post(url, headers);
    check_errors(res);
    return json::parse(res->body);
}

void Gitlab::createFork(const std::string &project_id, const std::string &group_id) {
    try {
        post("/api/v4/projects/" + project_id + "/fork?namespace_id=" + group_id);
    }
    catch (EmptyResponse &e) {}
}

void Gitlab::check_errors(httplib::Result &res) {
    if (res == nullptr) {
        throw HTTPError("Null ptr", 0);
    }
    if (res->status < 200 || res->status >= 300) {
        if (res->status == 401) {
            throw Unauthorized();
        }
        if (!(res) || res->body.empty()) {
            throw HTTPError("Empty body", res->status);
        } else {
            json data = json::parse(res->body);
            if (data["message"].is_array()) {
                std::string s;
                for (const auto &ele: data["message"]) s += ele;
                throw HTTPError(s, res->status);
            } else {
                throw HTTPError(data["message"], res->status);
            }
        }
    }
    if (!(res) || res->body.empty()) {
        throw EmptyResponse();
    }

}
