#pragma once
#include <atomic>
#include <thread>

#include "Modules/Module.hpp"

#include "Discord.hpp"
#include "Game.hpp"
#include "Plugin.hpp"
#include "SDK.hpp"

#define SGP_VERSION "1.1"
#define SGP_BUILD __TIME__ " " __DATE__
#define SGP_WEB "https://nekzor.github.io/sugar-glider"

#define SAFE_UNLOAD(ptr) \
    if (ptr) {           \
        delete ptr;      \
        ptr = nullptr;   \
    }

class CDiscordPlugin : public IServerPluginCallbacks {
public:
    Game* game;
    Plugin* plugin;
    Modules* modules;
    Discord* discord;

private:
    std::thread pluginThread;
    std::thread mainThread;
    std::atomic<bool> isRunning;

public:
    virtual bool Load(CreateInterfaceFn interfaceFactory, CreateInterfaceFn gameServerFactory);
    virtual void Unload();
    virtual void Pause();
    virtual void UnPause();
    virtual const char* GetPluginDescription();
    virtual void LevelInit(char const* pMapName);
    virtual void ServerActivate(void* pEdictList, int edictCount, int clientMax);
    virtual void GameFrame(bool simulating);
    virtual void LevelShutdown();
    virtual void ClientFullyConnect(void* pEdict);
    virtual void ClientActive(void* pEntity);
    virtual void ClientDisconnect(void* pEntity);
    virtual void ClientPutInServer(void* pEntity, char const* playername);
    virtual void SetCommandClient(int index);
    virtual void ClientSettingsChanged(void* pEdict);
    virtual int ClientConnect(bool* bAllowConnect, void* pEntity, const char* pszName, const char* pszAddress, char* reject, int maxrejectlen);
    virtual int ClientCommand(void* pEntity, const void*& args);
    virtual int NetworkIDValidated(const char* pszUserName, const char* pszNetworkID);
    virtual void OnQueryCvarValueFinished(int iCookie, void* pPlayerEntity, int eStatus, const char* pCvarName, const char* pCvarValue);
    virtual void OnEdictAllocated(void* edict);
    virtual void OnEdictFreed(const void* edict);

    const char* Version() { return SGP_VERSION; }
    const char* Build() { return SGP_BUILD; }
    const char* Website() { return SGP_WEB; }

    bool TryGetPlugin();
    void Cleanup();

private:
    void StartPluginThread();
    void StartMainThread();
};

extern CDiscordPlugin g_DiscordPlugin;
