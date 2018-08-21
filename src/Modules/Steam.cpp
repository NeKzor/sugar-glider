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
    auto libsteam_api = Memory::GetModuleHandleByName(MODULE("libsteam_api"));
    if (libsteam_api) {
        auto SteamClient = Memory::GetSymbolAddress<void* (*)()>(libsteam_api, "SteamClient");
        if (SteamClient) {
            auto client = SteamClient();
            auto pipe = Memory::VMT<int (*)(void*)>(client, 0)(client);
            auto handle = Memory::VMT<int (*)(void*, int)>(client, 2)(client, pipe);

            this->user = Memory::VMT<ISteamUser* (*)(void*, int, int, const char*)>(client, 5)(client, handle, pipe, "SteamUser019");
            if (this->user) {
                auto id = this->user->GetSteamID();
                console->Print("Hello %llu!\n", id.ConvertToUint64());
            }
        }
    }
    Memory::CloseModuleHandle(libsteam_api);

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
