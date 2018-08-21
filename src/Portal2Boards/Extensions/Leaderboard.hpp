#pragma once
#include <vector>

#include "MapType.hpp"

namespace Portal2Boards {

class Leaderboard {
public:
    const char* levelName;
    const char* chamberName;
    MapType type;
    unsigned bestTimeId;
    unsigned bestPortalsId;
    int chapterId;
    static std::vector<class Leaderboard> list;

public:
    bool DoesExist();
    static bool TryFindByLevelName(const char* levelName, Leaderboard& leaderboard);
};
}
