#include "Client.hpp"

#include <cstring>
#include <string>
#include <vector>

#include "nlohmann/json.hpp"
#include "restclient-cpp/connection.h"
#include "restclient-cpp/restclient.h"

#include <iostream>

namespace Portal2Boards {

namespace RC = RestClient;
using json = nlohmann::json;

Client::Client(const char* userAgent)
    : api("https://board.iverb.me")
    , ua("Portal2Boards.cpp/1.0")
{
    if (std::strlen(userAgent) != 0) {
        std::snprintf(this->ua, sizeof(this->ua), "%s %s", userAgent, this->ua);
    }

    RC::init();
    this->client = new RC::Connection(this->api);
    this->client->SetTimeout(5);
    this->client->SetUserAgent(this->ua);
}
Client::~Client()
{
    if (this->client) {
        delete this->client;
        this->client = nullptr;
    }
    RC::disable();
}
bool Client::TryGetAggregated(AggregatedMode mode, Aggregated& result)
{
    std::string url;
    switch (mode) {
    case AggregatedMode::Overall:
        url = std::string("/overall");
        break;
    case AggregatedMode::SinglePlayer:
        url = std::string("/sp");
        break;
    case AggregatedMode::Cooperative:
        url = std::string("/coop");
        break;
    case AggregatedMode::Chapter:
    default:
        return false;
    }

    char get[64];
    std::snprintf(get, sizeof(get), "/aggregated%s/json", url.c_str());

    auto response = this->client->get(get);
    if (response.code == 200) {
        result.Parse(json::parse(response.body));
        return true;
    }
    return false;
}
bool Client::TryGetChamber(uint bestTimeId, Chamber& result)
{
    char get[64];
    std::snprintf(get, sizeof(get), "/chamber/%i/json", bestTimeId);

    auto response = this->client->get(get);
    if (response.code == 200) {
        result.Parse(bestTimeId, json::parse(response.body));
        return true;
    }
    return false;
}
}
