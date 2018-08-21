#pragma once
#include "Entities/Aggregated/Aggregated.hpp"
#include "Entities/Chamber/Chamber.hpp"

#include "restclient-cpp/connection.h"
#include "restclient-cpp/restclient.h"

namespace Portal2Boards {

namespace RC = RestClient;

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
    RC::Connection* client;

public:
    Client(const char* userAgent = "");
    ~Client();
    bool TryGetAggregated(AggregatedMode chapter, Aggregated& result);
    bool TryGetChamber(unsigned bestTimeId, Chamber& result);
};
}
