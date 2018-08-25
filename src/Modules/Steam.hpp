#pragma once
#include <cstdint>

#include "Module.hpp"

#include "SDK.hpp"

#ifdef _WIN32
#define STEAM_API "steam_api"
#else
#define STEAM_API "libsteam_api"
#endif

#define ISteamClient_Symbol "SteamClient"

class Steam : public Module {
public:
    ISteamUser* user;

public:
    Steam();
    bool Init() override;
    void Shutdown() override;

    uint64_t GetLocalSteamId();
};

extern Steam* steam;
