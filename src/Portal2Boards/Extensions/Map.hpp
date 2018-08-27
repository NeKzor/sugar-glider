#pragma once
#include <vector>

#include "MapType.hpp"

namespace Portal2Boards {

class Map {
public:
    const char* levelName;
    const char* chamberName;
    MapType type;
    unsigned bestTimeId;
    unsigned bestPortalsId;
    int chapterId;

    static std::vector<class Map> portal2;
    static std::vector<class Map> aperturetag;
    static std::vector<class Map> portal_stories;

public:
    bool HasLeaderboard();
    static bool GetMapByName(const char* levelName, Map& map, const std::vector<class Map>* campaign = &Map::portal2);
};
}
