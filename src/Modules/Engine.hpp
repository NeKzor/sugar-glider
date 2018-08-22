#pragma once
#include "Interface.hpp"
#include "Module.hpp"
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

    using _IsInEditMode = bool (*)();
    _IsInEditMode IsInEditMode;

    using _IsInCommentaryMode = bool (*)();
    _IsInCommentaryMode IsInCommentaryMode;

    using _IsPlayingBack = bool(__funcc*)(void* thisptr);
    _IsPlayingBack IsPlayingBack;

    Engine();
    bool Init() override;
    void Shutdown() override;

    bool CL_IsRecordingMovie();
};

extern Engine* engine;
