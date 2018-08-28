#include "Engine.hpp"

#include "Console.hpp"

#include "CDiscordPlugin.hpp"
#include "Command.hpp"
#include "Interface.hpp"
#include "Memory.hpp"
#include "Offsets.hpp"
#include "Platform.hpp"
#include "Variable.hpp"

Variable sv_cheats;
Variable sv_bonus_challenge;

Engine::Engine()
    : Module()
    , hoststate(nullptr)
    , m_szLevelName(nullptr)
    , demoplayer(nullptr)
    , moviename(nullptr)
    , GetMaxClients(nullptr)
    , GetGameDirectory(nullptr)
    , IsInCommentaryMode(nullptr)
    , IsPlayingBack(nullptr)
{
}
bool Engine::Init()
{
    if (auto engine = Interface::Create(MODULE("engine"), "VEngineClient0", false)) {
        this->GetMaxClients = engine->Original<_GetMaxClients>(Offsets::GetMaxClients);
        this->GetGameDirectory = engine->Original<_GetGameDirectory>(Offsets::GetGameDirectory);
        this->IsInCommentaryMode = engine->Original<_IsInCommentaryMode>(Offsets::IsInCommentaryMode);

        typedef void* (*_GetClientState)();
        auto ClientCmd = engine->Original(Offsets::ClientCmd);
        auto GetClientState = Memory::Read<_GetClientState>(ClientCmd + Offsets::GetClientStateFunction);
        auto cl = Interface::Create(GetClientState(), false);
        if (cl) {
            auto SetSignonState = cl->Original(Offsets::SetSignonState);
            auto HostState_OnClientConnected = Memory::Read(SetSignonState + Offsets::HostState_OnClientConnected);
            this->hoststate = Memory::Deref<CHostState*>(HostState_OnClientConnected + Offsets::hoststate);

            auto disconnect = cl->Original(Offsets::Disconnect);
            this->demoplayer = Memory::DerefDeref<void*>(disconnect + Offsets::demoplayer);
            this->IsPlayingBack = Memory::VMT<_IsPlayingBack>(this->demoplayer, Offsets::IsPlayingBack);
        }
    }

    auto endmovie = Command("endmovie");
    if (!!endmovie) {
        auto cc_endmovie_callback = (uintptr_t)endmovie.ThisPtr()->m_pCommandCallback;
        this->moviename = Memory::Deref<char*>(cc_endmovie_callback + Offsets::CL_IsRecordingMovie);
    }

    if (auto tool = Interface::Create(MODULE("engine"), "VENGINETOOL0", false)) {
        auto GetCurrentMap = tool->Original(Offsets::GetCurrentMap);
        this->m_szLevelName = Memory::Deref<char*>(GetCurrentMap + Offsets::m_szLevelName);
    }

    sv_cheats = Variable("sv_cheats");
    sv_bonus_challenge = Variable("sv_bonus_challenge");
    developer = Variable("developer");

    return this->hasLoaded = this->hoststate != nullptr
        && this->demoplayer != nullptr
        && this->moviename != nullptr
        && this->GetMaxClients != nullptr
        && this->IsInCommentaryMode != nullptr
        && this->IsPlayingBack != nullptr
        && this->m_szLevelName != nullptr
        && !!sv_cheats
        && !!sv_bonus_challenge
        && !!developer;
}
void Engine::Shutdown()
{
}
bool Engine::CL_IsRecordingMovie()
{
    return *this->moviename != 0;
}

Engine* engine;
