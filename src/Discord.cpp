#include "Discord.hpp"

#include <cstring>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#ifndef _WIN32
#include "discord-rpc/linux/discord_rpc.h"
#endif

#include "Modules/Console.hpp"
#include "Modules/Engine.hpp"
#include "Modules/Steam.hpp"

#include "Portal2Boards/Portal2Boards.hpp"

using namespace Portal2Boards;

Discord::Discord()
    : level("unknown")
    , globalRank("")
    , levelRank("")
    , large()
    , small()
{
    this->isCooping = engine->GetMaxClients() >= 2;
    this->isRouting = sv_cheats.GetBool();
    this->isGrinding = sv_bonus_challenge.GetBool();
    this->isViewing = engine->IsPlayingBack(engine->demoplayer);
    this->isRendering = engine->CL_IsRecordingMovie();
    this->isMapping = false;
    this->isActive = engine->hoststate->m_activeGame;
    this->iverb = new Client("portal2-discord-plugin/1.0");
    this->large.shouldShow = true;
}
Discord::~Discord()
{
    if (this->iverb) {
        delete this->iverb;
        this->iverb = nullptr;
    }
    this->Shutdown();
}
void Discord::Init()
{
    Aggregated global;
    if (this->iverb->TryGetAggregated(AggregatedMode::Overall, global)) {
        auto steamId = steam->GetLocalSteamId();
        for (const auto& entry : global.points) {
            if (entry.first == steamId) {
                auto rank = std::string("Global Rank: ") + std::to_string(entry.second.scoreData.playerRank);
                console->Print("SGP: Global rank of %llu -> %i\n", steamId, entry.second.scoreData.playerRank);
                std::strcpy(this->globalRank, rank.c_str());
            }
        }

        if (std::strlen(this->globalRank) == 0) {
            console->Warning("SGP: Player with id %llu does not have board.iverb.me profile!\n", steamId);
        }
    } else {
        console->Warning("SGP: Failed to fetch aggregated leaderboard!\n");
    }

    DiscordEventHandlers handlers;
    std::memset(&handlers, 0, sizeof(handlers));
    handlers.ready = Discord::OnDiscordReady;
    handlers.disconnected = Discord::OnDiscordDisconnected;
    handlers.errored = Discord::OnDiscordErrored;
    Discord_Initialize(APP_ID, &handlers, 1, STEAM_APP_ID);
    Discord_RunCallbacks();
}
const char* Discord::GetDetails()
{
    if (this->isRendering || this->isViewing) {
        return "Demo Player";
    }
    if (this->isMapping) {
        return "Puzzle Maker";
    }
    if (!this->isActive) {
        return "Main Menu";
    }
    if (this->isGrinding) {
        return "Challenge Mode";
    }
    if (this->isCooping) {
        return "Cooperative";
    }

    Leaderboard lb;
    if (Leaderboard::TryFindByLevelName(this->level, lb)) {
        if (lb.type == MapType::SinglePlayer)
            return "Single Player";
        if (lb.type == MapType::Extras)
            return "Extras";
    }

    return "Workshop";
}
const char* Discord::GetState()
{
    if (this->isRendering) {
        return "Rendering";
    }
    if (this->isViewing) {
        return "Viewing";
    }
    if (this->isMapping) {
        return "Mapping";
    }
    if (!this->isActive) {
        return "Menuing";
    }
    if (this->isRouting) {
        return "Routing";
    }
    if (this->isGrinding) {
        return "Grinding";
    }

    Leaderboard lb;
    if (Leaderboard::TryFindByLevelName(this->level, lb)) {
        return "Running";
    }

    return "Playing";
}
void Discord::ResolveLevel()
{
    Leaderboard lb;
    if (this->isActive && Leaderboard::TryFindByLevelName(this->level, lb)) {
        std::strcpy(this->large.key, lb.levelName);
        std::strcpy(this->large.text, lb.chamberName);
    } else {
        std::strcpy(this->large.key, (!this->isActive) ? "main_menu" : "puzzlemaker_newchamber");
        std::strcpy(this->large.text, (!this->isActive) ? "Main Menu" : "Workshop");
    }
}
void Discord::ResolveRank()
{
    this->small.shouldShow = false;

    if (std::strlen(this->globalRank) != 0) {
        std::strcpy(this->small.key, "iverb");

        Leaderboard lb;
        Chamber chamber;
        if (this->isGrinding
            && Leaderboard::TryFindByLevelName(this->level, lb)
            && lb.DoesExist()
            && this->iverb->TryGetChamber(lb.bestTimeId, chamber)) {
            auto steamId = steam->GetLocalSteamId();
            for (const auto& entry : chamber.entries) {
                if (entry.first == steamId) {
                    auto rank = std::string("Rank: ") + std::to_string(entry.second.score.playerRank);
                    console->Print("Level rank of %llu for %s -> %i\n", steamId, lb.chamberName, entry.second.score.playerRank);
                    std::strcpy(this->small.text, rank.c_str());
                    this->small.shouldShow = true;
                }
            }

            if (!this->small.shouldShow) {
                console->Print("Unable to find rank of %llu for %s\n", steamId, lb.chamberName);
            }
        } else {
            std::strcpy(this->small.text, this->globalRank);
            this->small.shouldShow = true;
        }
    }
}
void Discord::SendPresence()
{
    DiscordRichPresence presence;
    std::memset(&presence, 0, sizeof(presence));
    presence.state = this->GetState();
    presence.details = this->GetDetails();
    presence.startTimestamp = time(0);

    this->ResolveLevel();
    this->ResolveRank();

    if (this->large.shouldShow) {
        presence.largeImageKey = this->large.key;
        presence.largeImageText = this->large.text;
    }
    if (this->small.shouldShow) {
        presence.smallImageKey = this->small.key;
        presence.smallImageText = this->small.text;
    }

    Discord_UpdatePresence(&presence);
}
void Discord::Update()
{
    auto change = false;

    // Detect changes
    if (std::strcmp(this->level, engine->hoststate->m_levelName) != 0) {
        std::strcpy(this->level, engine->hoststate->m_levelName);
        this->isMapping = !std::strcmp(engine->hoststate->m_levelName, "puzzlemaker/newchamber");
        console->Print("this->level = %s\n", this->level);
        console->Print("this->isMapping = %i\n", this->isMapping);
        change = true;
    }
    if (this->isCooping != engine->GetMaxClients() >= 2) {
        this->isCooping = engine->GetMaxClients() >= 2;
        console->Print("this->isCooping = %i\n", this->isCooping);
        change = true;
    }
    if (this->isGrinding != sv_bonus_challenge.GetBool()) {
        this->isGrinding = sv_bonus_challenge.GetBool();
        console->Print("this->isGrinding = %i\n", this->isGrinding);
        change = true;
    }
    if (this->isRouting != sv_cheats.GetBool()) {
        this->isRouting = sv_cheats.GetBool();
        console->Print("this->isRouting = %i\n", this->isRouting);
        change = true;
    }
    if (this->isViewing != engine->IsPlayingBack(engine->demoplayer)) {
        this->isViewing = engine->IsPlayingBack(engine->demoplayer);
        console->Print("this->isViewing = %i\n", this->isViewing);
        change = true;
    }
    if (this->isRendering != engine->CL_IsRecordingMovie()) {
        this->isRendering = engine->CL_IsRecordingMovie();
        console->Print("this->isRendering = %i\n", this->isRendering);
        change = true;
    }
    if (this->isActive != engine->hoststate->m_activeGame) {
        this->isActive = engine->hoststate->m_activeGame;
        console->Print("this->isActive = %i\n", this->isActive);
        change = true;
    }

    if (change) {
        console->Print("Discord -> Change!\n");
        this->SendPresence();
    }

    Discord_RunCallbacks();
}
void Discord::Shutdown()
{
    Discord_Shutdown();
}

// Callbacks
void Discord::OnDiscordReady(const DiscordUser* request)
{
    console->Print("Discord Ready -> %s#%s\n", request->username, request->discriminator);
}
void Discord::OnDiscordDisconnected(int errorCode, const char* message)
{
    console->Print("Discord Disconnected -> %i\n%s\n", errorCode, message);
}
void Discord::OnDiscordErrored(int errorCode, const char* message)
{
    console->Print("Discord Errored -> %i\n%s\n", errorCode, message);
}
