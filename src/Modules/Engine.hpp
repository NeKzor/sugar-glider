#pragma once
#include <string>

#include "Interface.hpp"
#include "Module.hpp"
#include "Platform.hpp"
#include "Variable.hpp"

extern Variable sv_cheats;
extern Variable sv_bonus_challenge;

class Engine : public Module {
public:
    CHostState* hoststate;
    char* m_szLevelName;
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
};

extern Engine* engine;
