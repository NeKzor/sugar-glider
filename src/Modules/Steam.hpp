#pragma once
#include <cstdint>

#include "Module.hpp"

#include "SDK.hpp"

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
