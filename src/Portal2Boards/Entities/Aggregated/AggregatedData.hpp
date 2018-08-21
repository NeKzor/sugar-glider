#pragma once
#include "nlohmann/json.hpp"

#include "../EntryScoreData.hpp"
#include "../EntryUserData.hpp"

namespace Portal2Boards {

using json = nlohmann::json;

class AggregatedData {
public:
    // userData
    EntryUserData userData;
    // scoreData
    EntryScoreData scoreData;

public:
    void Parse(json data)
    {
        this->userData.Parse(data["userData"]);
        this->scoreData.Parse(data["scoreData"]);
    }
};
}
