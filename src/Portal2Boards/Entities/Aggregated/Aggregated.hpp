#pragma once
#include <map>

#include "nlohmann/json.hpp"

#include "AggregatedData.hpp"

namespace Portal2Boards {

using json = nlohmann::json;

class Aggregated {
public:
    // Points
    std::map<unsigned long long, AggregatedData> points;
    // Times
    std::map<unsigned long long, AggregatedData> times;

public:
    Aggregated()
        : points()
        , times()
    {
    }
    void Parse(json data)
    {
        json points = data["Points"];
        for (json::iterator it = points.begin(); it != points.end(); ++it) {
            auto data = AggregatedData();
            data.Parse(it.value());
            this->points.insert({ std::stoll(it.key().c_str()), data });
        }

        json times = data["Times"];
        for (json::iterator it = times.begin(); it != times.end(); ++it) {
            auto data = AggregatedData();
            data.Parse(it.value());
            this->times.insert({ std::stoll(it.key().c_str()), data });
        }
    }
};
}
