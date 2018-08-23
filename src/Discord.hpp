#pragma once
#include "Portal2Boards/Portal2Boards.hpp"

#define APP_ID "478256997314330634"
#define STEAM_APP_ID "620"

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
    bool isMapping;
    bool isListening;
    // Connection
    Portal2Boards::Client* iverb;
    char globalRank[32];
    char levelRank[32];
    // Assets
    DiscordAsset large;
    DiscordAsset small;
    // Cache
    char details[256];
    char state[256];
    time_t timestamp;

public:
    Discord();
    ~Discord();
    void Init();
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
