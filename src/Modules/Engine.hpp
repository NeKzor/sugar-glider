#pragma once
#include <string>

#include "Interface.hpp"
#include "Module.hpp"
#include "Platform.hpp"
#include "Variable.hpp"

#include "SDK.hpp"

extern Variable sv_cheats;
extern Variable sv_bonus_challenge;

class Engine : public Module {
public:
    CHostState* hoststate;
    void* demoplayer;
    char* moviename;

    using _GetMaxClients = int (*)();
    _GetMaxClients GetMaxClients;

    using _GetGameDirectory = char*(__cdecl*)();
    _GetGameDirectory GetGameDirectory;

    using _IsInCommentaryMode = bool (*)();
    _IsInCommentaryMode IsInCommentaryMode;

    using _IsPlayingBack = bool(__funcc*)(void* thisptr);
    _IsPlayingBack IsPlayingBack;

    Engine();
    bool Init() override;
    void Shutdown() override;

    bool CL_IsRecordingMovie();

    inline bool IsInMenu()
    {
        return !this->hoststate->m_activeGame
            && this->hoststate->m_currentState == HOSTSTATES::HS_RUN;
    }
    std::string GetModDir()
    {
        auto dir = std::string(this->GetGameDirectory());
        auto index = dir.find_last_of("\\/");
        return dir.substr(index + 1, dir.length() - index);
    }
};

extern Engine* engine;
