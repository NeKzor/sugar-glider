#pragma once
#include <string>

#include "nlohmann/json.hpp"

namespace Portal2Boards {

using json = nlohmann::json;

class EntryUserData {
public:
    // boardname
    std::string boardName;
    // avatar
    std::string avatar;

public:
    void Parse(json data)
    {
        this->boardName = (!data["boardname"].is_null()) ? data["boardname"].get<std::string>() : std::string("");
        this->avatar = (!data["avatar"].is_null()) ? data["avatar"].get<std::string>() : std::string("");
    }
};
}
