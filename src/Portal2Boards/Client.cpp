#include "Client.hpp"

#include <cstring>
#include <string>
#include <vector>

#include "curl/curl.h"
#include "nlohmann/json.hpp"

// Note: Using console module for debugging
#include "Modules/Console.hpp"

namespace Portal2Boards {

using json = nlohmann::json;

Client::Client(const char* userAgent)
    : api("https://board.iverb.me")
    , ua("Portal2Boards.cpp/1.0")
{
    if (std::strlen(userAgent) != 0) {
        std::snprintf(this->ua, sizeof(this->ua), "%s %s", userAgent, this->ua);
    }

    this->client = curl_easy_init();
    curl_easy_setopt(this->client, CURLOPT_USERAGENT, this->ua);
}
Client::~Client()
{
    if (this->client) {
        curl_easy_cleanup(this->client);
        this->client = nullptr;
    }
}
// Curl needs this function :>
size_t WriteFunction(void* ptr, size_t size, size_t nmemb, std::string* data)
{
    data->append((char*)ptr, size * nmemb);
    return size * nmemb;
}
bool Client::TryGetAggregated(const AggregatedMode mode, Aggregated& result)
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
    std::snprintf(get, sizeof(get), "%s/aggregated%s/json", this->api, url.c_str());

    std::string responseHeader;
    std::string responseBody;
    curl_easy_setopt(this->client, CURLOPT_URL, get);
    curl_easy_setopt(this->client, CURLOPT_HEADERDATA, &responseHeader);
    curl_easy_setopt(this->client, CURLOPT_WRITEDATA, &responseBody);
    curl_easy_setopt(this->client, CURLOPT_WRITEFUNCTION, WriteFunction);

    auto code = curl_easy_perform(this->client);
    if (code == CURLcode::CURLE_OK) {
        long response;
        curl_easy_getinfo(this->client, CURLINFO_RESPONSE_CODE, &response);
        console->Debug("SGP: CURL -> %s (%d)\n", get, response);
        if (response == 200) {
            result.Parse(json::parse(responseBody));
            return true;
        }
    } else {
        console->Debug("SGP: CURL Error -> %i\n", code);
    }
    return false;
}
bool Client::TryGetChamber(unsigned int bestTimeId, Chamber& result)
{
    char get[64];
    std::snprintf(get, sizeof(get), "%s/chamber/%i/json", this->api, bestTimeId);

    std::string responseHeader;
    std::string responseBody;
    curl_easy_setopt(this->client, CURLOPT_URL, get);
    curl_easy_setopt(this->client, CURLOPT_HEADERDATA, &responseHeader);
    curl_easy_setopt(this->client, CURLOPT_WRITEDATA, &responseBody);
    curl_easy_setopt(this->client, CURLOPT_WRITEFUNCTION, WriteFunction);

    auto code = curl_easy_perform(this->client);
    if (code == CURLcode::CURLE_OK) {
        long response;
        curl_easy_getinfo(this->client, CURLINFO_RESPONSE_CODE, &response);
        console->Debug("SGP: CURL -> %s (%d)\n", get, response);
        if (response == 200) {
            result.Parse(bestTimeId, json::parse(responseBody));
            return true;
        }
    } else {
        console->Debug("SGP: CURL Error -> %i\n", code);
    }
    return false;
}
}
