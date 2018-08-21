#pragma once
#include "nlohmann/json.hpp"

namespace Portal2Boards {

using json = nlohmann::json;

class EntryScoreData {
public:
    // score
    int score;
    // playerRank
    int playerRank;
    // scoreRank
    int scoreRank;

public:
    void Parse(json data)
    {
        this->score = (!data["score"].is_null()) ? data["score"].get<int>() : 0;
        this->playerRank = (!data["playerRank"].is_null()) ? data["playerRank"].get<int>() : 0;
        this->scoreRank = (!data["scoreRank"].is_null()) ? data["scoreRank"].get<int>() : 0;
    }
};
}
