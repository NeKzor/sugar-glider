#pragma once
#include "Portal2Boards/Portal2Boards.hpp"

#define P2_APP_ID "478256997314330634"
#define AT_APP_ID "482831216165257216"
#define PS_APP_ID "482833874406211584"

#define P2_STEAM_APP_ID "620"
#define AT_STEAM_APP_ID "280740"
#define PS_STEAM_APP_ID "317400"

#ifdef _WIN32
#define WORKSHOP_STRING "workshop\\"
#define PUZZLEMAKER_STRING "puzzlemaker\\"
#else
#define WORKSHOP_STRING "workshop/"
#define PUZZLEMAKER_STRING "puzzlemaker/"
#endif

#define DETECT_CHANGE_S(prevState, curState)             \
    if (std::strcmp(prevState, curState) != 0) {         \
        std::strcpy(prevState, curState);                \
        console->Debug(#prevState " = %s\n", prevState); \
        change = true;                                   \
    }
#define DETECT_CHANGE_B(prevState, curState)             \
    if (prevState != curState) {                         \
        prevState = curState;                            \
        console->Debug(#prevState " = %i\n", prevState); \
        change = true;                                   \
    }

struct DiscordRichPresence;
struct DiscordUser;

struct DiscordAsset {
    char key[256];
    char text[256];
    bool isActive;

    DiscordAsset()
        : key("")
        , text("")
        , isActive(false)
    {
    }
};

class Discord {
private:
    // States
    char level[256];
    bool isCooping;
    bool isRouting;
    bool isGrinding;
    bool isViewing;
    bool isRendering;
    bool isMenuing;
    bool isListening;
    // Connection
    bool hasChallengeMode;
    Portal2Boards::Client* iverb;
    char globalRank[32];
    char levelRank[32];
    std::vector<Portal2Boards::Map>* campaign;
    // Assets
    DiscordAsset largeAsset;
    DiscordAsset smallAsset;
    // Cache
    char details[256];
    char state[256];
    time_t timestamp;

public:
    Discord();
    ~Discord();
    bool Init();
    void SendPresence();
    void Update();
    void Shutdown();

private:
    const char* GetDetails();
    const char* GetState();
    void ResolveLevel();
    void ResolveRank();

public:
    static void OnDiscordReady(const DiscordUser* request);
    static void OnDiscordDisconnected(int errorCode, const char* message);
    static void OnDiscordErrored(int errorCode, const char* message);
};
