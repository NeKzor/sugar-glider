#pragma once
#include <string>

#include "nlohmann/json.hpp"

#include "../EntryScoreData.hpp"

namespace Portal2Boards {

using json = nlohmann::json;

class ChamberEntryScoreData : public EntryScoreData {
public:
    // note
    std::string note; // can be null
    // submission
    std::string submission;
    // changelogId
    uint changelogId;
    // date
    std::string date; // can be null
    // hasDemo
    std::string hasDemo;
    // youtubeID
    std::string youTubeId; // can be null

public:
    void Parse(json data)
    {
        this->note = (!data["note"].is_null()) ? data["note"].get<std::string>() : std::string("");
        this->submission = (!data["submission"].is_null()) ? data["submission"].get<std::string>() : std::string("");
        this->changelogId = (!data["changelogId"].is_null()) ? std::atoi(data["changelogId"].get<std::string>().c_str()) : 0;
        this->playerRank = (!data["playerRank"].is_null()) ? std::atoi(data["playerRank"].get<std::string>().c_str()) : 0;
        this->scoreRank = (!data["scoreRank"].is_null()) ? std::atoi(data["scoreRank"].get<std::string>().c_str()) : 0;
        this->score = (!data["score"].is_null()) ? std::atoi(data["score"].get<std::string>().c_str()) : 0;
        this->date = (!data["date"].is_null()) ? data["date"].get<std::string>() : std::string("");
        this->hasDemo = (!data["hasDemo"].is_null()) ? data["hasDemo"].get<std::string>() : std::string("");
        this->youTubeId = (!data["youtubeID"].is_null()) ? data["youtubeID"].get<std::string>() : std::string("");
    }
};
}
