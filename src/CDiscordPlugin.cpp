#include "CDiscordPlugin.hpp"

#include "Modules/Console.hpp"
#include "Modules/Engine.hpp"
#include "Modules/Module.hpp"
#include "Modules/Steam.hpp"
#include "Modules/Tier1.hpp"

#include "Game.hpp"

#include "SDK.hpp"

CDiscordPlugin g_DiscordPlugin;
EXPOSE_SINGLE_INTERFACE_GLOBALVAR(CDiscordPlugin, IServerPluginCallbacks, INTERFACEVERSION_ISERVERPLUGINCALLBACKS, g_DiscordPlugin);

bool CDiscordPlugin::Load(CreateInterfaceFn interfaceFactory, CreateInterfaceFn gameServerFactory)
{
    console = new Console();
    if (!console->Init())
        return false;

    this->game = Game::CreateGame();
    if (game) {
        this->game->LoadOffsets();

        this->modules = new Modules();
        this->modules->AddModule<Tier1>(&tier1);
        this->modules->InitAll();

        if (tier1->hasLoaded) {
            this->modules->AddModule<Engine>(&engine);
            this->modules->AddModule<Steam>(&steam);
            this->modules->InitAll();

            if (engine->hasLoaded && steam->hasLoaded) {
                this->plugin = new Plugin();
                this->StartPluginThread();

                this->discord = new Discord();
                this->discord->Init();
                this->StartMainThread();

                console->PrintActive("Loaded Discord Plugin, Version %s (by NeKz)\n", this->Version());
                return true;
            } else {
                console->Warning("SGP: Could not load stuff from the engine!\n");
            }
        } else {
            console->Warning("SGP: Could not register any commands!\n");
        }
    } else {
        console->Warning("SGP: Game not supported!\n");
    }

    console->Warning("SGP: Discord Plugin failed to load!\n");
    return false;
}
void CDiscordPlugin::Unload()
{
    console->Print("SGP: Cya!\n");

    this->isRunning = false;
    if (this->mainThread.joinable())
        mainThread.detach();

    SAFE_UNLOAD(this->discord);
    SAFE_UNLOAD(this->game);
    SAFE_UNLOAD(this->plugin);
    SAFE_UNLOAD(this->modules);
    SAFE_UNLOAD(console);
}
const char* CDiscordPlugin::GetPluginDescription()
{
    return SGP_SIGNATURE;
}

#pragma region Unused callbacks
void CDiscordPlugin::Pause()
{
}
void CDiscordPlugin::UnPause()
{
}
void CDiscordPlugin::LevelInit(char const* pMapName)
{
}
void CDiscordPlugin::ServerActivate(void* pEdictList, int edictCount, int clientMax)
{
}
void CDiscordPlugin::GameFrame(bool simulating)
{
}
void CDiscordPlugin::LevelShutdown()
{
}
void CDiscordPlugin::ClientFullyConnect(void* pEdict)
{
}
void CDiscordPlugin::ClientActive(void* pEntity)
{
}
void CDiscordPlugin::ClientDisconnect(void* pEntity)
{
}
void CDiscordPlugin::ClientPutInServer(void* pEntity, char const* playername)
{
}
void CDiscordPlugin::SetCommandClient(int index)
{
}
void CDiscordPlugin::ClientSettingsChanged(void* pEdict)
{
}
int CDiscordPlugin::ClientConnect(bool* bAllowConnect, void* pEntity, const char* pszName, const char* pszAddress, char* reject, int maxrejectlen)
{
    return 0;
}
int CDiscordPlugin::ClientCommand(void* pEntity, const void*& args)
{
    return 0;
}
int CDiscordPlugin::NetworkIDValidated(const char* pszUserName, const char* pszNetworkID)
{
    return 0;
}
void CDiscordPlugin::OnQueryCvarValueFinished(int iCookie, void* pPlayerEntity, int eStatus, const char* pCvarName, const char* pCvarValue)
{
}
void CDiscordPlugin::OnEdictAllocated(void* edict)
{
}
void CDiscordPlugin::OnEdictFreed(const void* edict)
{
}
#pragma endregion

// We don't use any callbacks
bool CDiscordPlugin::PluginFound()
{
    static Interface* s_ServerPlugin = Interface::Create(MODULE("engine"), "ISERVERPLUGINHELPERS0", false);
    if (!this->plugin->found && s_ServerPlugin) {
        auto m_Size = *reinterpret_cast<int*>((uintptr_t)s_ServerPlugin->ThisPtr() + CServerPlugin_m_Size);
        if (m_Size > 0) {
            auto m_Plugins = *reinterpret_cast<uintptr_t*>((uintptr_t)s_ServerPlugin->ThisPtr() + CServerPlugin_m_Plugins);
            for (int i = 0; i < m_Size; i++) {
                auto ptr = *reinterpret_cast<CPlugin**>(m_Plugins + sizeof(uintptr_t) * i);
                if (!std::strcmp(ptr->m_szName, SGP_SIGNATURE)) {
                    this->plugin->index = i;
                    this->plugin->ptr = ptr;
                    this->plugin->found = true;
                    break;
                }
            }
        }
    }
    return this->plugin->found;
}
void CDiscordPlugin::StartPluginThread()
{
    pluginThread = std::thread([this]() {
        GO_THE_FUCK_TO_SLEEP(1000);
        if (!this->PluginFound()) {
            console->DevWarning("SGP: Failed to find Discord Plugin in the plugin list!\n");
        } else {
            this->plugin->ptr->m_bDisable = true;
        }
    });
    pluginThread.detach();
}

// Main loop
void CDiscordPlugin::StartMainThread()
{
    this->isRunning = true;
    mainThread = std::thread([this]() {
        while (this->isRunning) {
            this->discord->Update();
            GO_THE_FUCK_TO_SLEEP(333);
        }
    });
}
