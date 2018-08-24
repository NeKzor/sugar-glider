#include "Steam.hpp"

#include <cstdint>

#include "Console.hpp"

#include "Memory.hpp"
#include "Offsets.hpp"
#include "Platform.hpp"

Steam::Steam()
    : Module()
{
}
bool Steam::Init()
{
    auto steam_api = Memory::GetModuleHandleByName(MODULE(STEAM_API));
    if (steam_api) {
        auto SteamClient = Memory::GetSymbolAddress<void* (*)()>(steam_api, ISteamClient_Symbol);
        if (SteamClient) {
            auto client = SteamClient();
            auto CreateSteamPipe = Memory::VMT<int(__funcc*)(void*)>(client, ISteamClient_CreateSteamPipe);
            auto ConnectToGlobalUser = Memory::VMT<int(__funcc*)(void*, int)>(client, ISteamClient_ConnectToGlobalUser);
            auto GetISteamUser = Memory::VMT<ISteamUser*(__funcc*)(void*, int, int, const char*)>(client, ISteamClient_GetISteamUser);

            auto pipe = CreateSteamPipe(client);
            auto handle = ConnectToGlobalUser(client, pipe);
            this->user = GetISteamUser(client, handle, pipe, STEAMUSER_INTERFACE_VERSION);
            if (this->user) {
                console->Debug("SGP: Hello %llu!\n", this->user->GetSteamID().ConvertToUint64());
            }
        }
    }
    Memory::CloseModuleHandle(steam_api);

    return this->hasLoaded = this->user != nullptr;
}
void Steam::Shutdown()
{
}
uint64_t Steam::GetLocalSteamId()
{
    return this->user->GetSteamID().ConvertToUint64();
}

Steam* steam;
