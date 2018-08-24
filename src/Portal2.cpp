#include "Portal2.hpp"

#include "Game.hpp"
#include "Offsets.hpp"

Portal2::Portal2()
{
    this->version = SourceGame::Portal2;
}
void Portal2::LoadOffsets()
{
    using namespace Offsets;

#ifdef _WIN32
    // engine.dll

    InternalSetValue = 12; // ConVar
    InternalSetFloatValue = 13; // ConVar
    InternalSetIntValue = 14; // ConVar
    ClientCmd = 7; // CEngineClient
    GetClientStateFunction = 4; // CEngineClient::ClientCmd
    GetMaxClients = 20; // CEngineClient
    IsInCommentaryMode = 171; // CEngineClient
    HostState_OnClientConnected = 684; // CClientState::SetSignonState
    hoststate = 1; // HostState_OnClientConnected
    Disconnect = 16; //  CClientState
    demoplayer = 74; // CClientState::Disconnect
    IsPlayingBack = 6; // CDemoPlayer
    AutoCompletionFunc = 66; // listdemo_CompletionFunc
    SetSignonState = 15; // CClientState
    CL_IsRecordingMovie = 2; // endmovie

    // vstdlib.dll

    RegisterConCommand = 9; // CCVar
    UnregisterConCommand = 10; // CCvar
    FindCommandBase = 13; // CCVar
    m_pConCommandList = 48; // CCvar
#else
    // engine.so

    InternalSetValue = 19; // ConVar
    InternalSetFloatValue = 20; // ConVar
    InternalSetIntValue = 21; // ConVar
    ClientCmd = 7; // CEngineClient
    GetClientStateFunction = 11; // CEngineClient::ClientCmd
    GetMaxClients = 20; // CEngineClient
    IsInCommentaryMode = 172; // CEngineClient
    HostState_OnClientConnected = 735; // CClientState::SetSignonState
    hoststate = 9; // HostState_OnClientConnected
    Disconnect = 37; //  CClientState
    demoplayer = 93; // CClientState::Disconnect
    IsPlayingBack = 7; // CDemoPlayer
    AutoCompletionFunc = 37; // listdemo_CompletionFunc
    SetSignonState = 36; // CClientState
    CL_IsRecordingMovie = 8; // endmovie

    // libvstdlib.so

    RegisterConCommand = 9; // CCVar
    UnregisterConCommand = 10; // CCvar
    FindCommandBase = 13; // CCvar
    m_pConCommandList = 48; // CCvar
#endif
}
const char* Portal2::Version()
{
    return "Portal 2 (7054)";
}
const char* Portal2::Process()
{
#ifdef _WIN32
    return "portal2.exe";
#else
    return "portal2_linux";
#endif
}
