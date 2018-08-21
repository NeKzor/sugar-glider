#pragma once
#include <map>
#include <string>
#include <vector>

#include "nlohmann/json.hpp"

#include "../EntryUserData.hpp"
#include "ChamberEntryScoreData.hpp"

namespace Portal2Boards {

using json = nlohmann::json;

class ChamberEntry {
public:
    // scoreData
    ChamberEntryScoreData score;
    // userData
    EntryUserData user;

public:
    void Parse(json data)
    {
        this->score.Parse(data["scoreData"]);
        this->user.Parse(data["userData"]);
    }
};
}
