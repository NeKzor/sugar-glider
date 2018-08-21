#pragma once
#include <map>

#include "nlohmann/json.hpp"

#include "ChamberEntry.hpp"

namespace Portal2Boards {

using json = nlohmann::json;

class Chamber {
public:
    uint id;
    std::map<unsigned long long, ChamberEntry> entries;

public:
    Chamber()
        : id(0)
        , entries()
    {
    }
    void Parse(uint id, json data)
    {
        this->id = id;
        for (json::iterator it = data.begin(); it != data.end(); ++it) {
            auto entry = ChamberEntry();
            entry.Parse(it.value());
            this->entries.insert({ std::stoll(it.key().c_str()), entry });
        }
    }
};
}
