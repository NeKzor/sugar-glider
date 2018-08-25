#include "Steam.hpp"

#include <cstdint>

#include "Console.hpp"

#include "Memory.hpp"
#include "Offsets.hpp"
#include "Platform.hpp"

#include "SDK.hpp"

Steam::Steam()
    : Module()
{
}
bool Steam::Init()
{
    auto steam_api = Memory::GetModuleHandleByName(MODULE(STEAM_API));
    if (steam_api) {
        auto SteamClient = Memory::GetSymbolAddress<ISteamClient* (*)()>(steam_api, ISteamClient_Symbol);
        if (SteamClient) {
            auto client = SteamClient();
            auto pipe = client->CreateSteamPipe();
            auto handle = client->ConnectToGlobalUser(pipe);
            this->user = client->GetISteamUser(handle, pipe, STEAMUSER_INTERFACE_VERSION);
            if (this->user) {
                console->Debug("SGP: Hello test subject %llu!\n", this->user->GetSteamID().ConvertToUint64());
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
