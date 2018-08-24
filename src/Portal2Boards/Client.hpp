#pragma once
#include "Entities/Aggregated/Aggregated.hpp"
#include "Entities/Chamber/Chamber.hpp"

#include "curl/curl.h"

namespace Portal2Boards {

enum class AggregatedMode {
    Overall,
    SinglePlayer,
    Cooperative,
    Chapter
};

class Client {
public:
    const char api[32];
    char ua[64];
    CURL* client;

public:
    Client(const char* userAgent = "");
    ~Client();
    bool TryGetAggregated(const AggregatedMode chapter, Aggregated& result);
    bool TryGetChamber(unsigned int bestTimeId, Chamber& result);
};
}
