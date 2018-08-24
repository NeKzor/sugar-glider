#include "Discord.hpp"

#include <cstring>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#ifdef _WIN32
#include "discord-rpc/win32/discord_rpc.h"
#else
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
    , largeAsset()
    , smallAsset()
    , details("")
    , state("")
    , timestamp()
{
    this->isCooping = engine->GetMaxClients() >= 2;
    this->isRouting = sv_cheats.GetBool();
    this->isGrinding = sv_bonus_challenge.GetBool();
    this->isViewing = engine->IsPlayingBack(engine->demoplayer);
    this->isRendering = engine->CL_IsRecordingMovie();
    this->isMenuing = engine->IsInMenu();
    this->isListening = engine->IsInCommentaryMode();
    this->iverb = new Client("portal2-discord-plugin/1.0");
    this->largeAsset.isActive = true;
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
                auto rank = std::string("Global Rank ") + std::to_string(entry.second.scoreData.playerRank);
                console->Debug("SGP: Global rank of %llu -> %i\n", steamId, entry.second.scoreData.playerRank);
                std::strcpy(this->globalRank, rank.c_str());
            }
        }

        if (std::strlen(this->globalRank) == 0) {
            console->DevWarning("SGP: Player with id %llu does not have board.iverb.me profile!\n", steamId);
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
    if (this->isMenuing) {
        return "Main Menu";
    }
    if (std::strstr(this->level, PUZZLEMAKER_STRING)) {
        return "Puzzle Maker";
    }
    if (this->isListening) {
        return "Developer Commentary";
    }
    if (this->isGrinding) {
        return "Challenge Mode";
    }
    if (this->isCooping) {
        return "Cooperative";
    }
    if (std::strstr(this->level, WORKSHOP_STRING)) {
        return "Workshop";
    }

    Leaderboard lb;
    if (Leaderboard::TryFindByLevelName(this->level, lb)) {
        if (lb.type == MapType::SinglePlayer)
            return "Single Player";
        if (lb.type == MapType::Cooperative)
            return "Cooperative";
        if (lb.type == MapType::Extras)
            return "Extras";
    }

    return "Custom Map";
}
const char* Discord::GetState()
{
    if (this->isRendering) {
        return "Rendering";
    }
    if (this->isViewing) {
        return "Viewing";
    }
    if (this->isMenuing) {
        return "Menuing";
    }
    if (std::strstr(this->level, PUZZLEMAKER_STRING)) {
        return "Mapping";
    }
    if (this->isRouting) {
        return "Routing";
    }
    if (this->isListening) {
        return "Listening";
    }
    if (this->isGrinding) {
        return "Grinding";
    }
    if (std::strlen(this->globalRank) != 0) {
        return "Running";
    }

    return "Playing";
}
void Discord::ResolveLevel()
{
    if (!this->isMenuing) {
        Leaderboard lb;
        if (Leaderboard::TryFindByLevelName(this->level, lb)) {
            std::strcpy(this->largeAsset.key, lb.levelName);
            std::strcpy(this->largeAsset.text, lb.chamberName);
        } else {
            std::strcpy(this->largeAsset.key, "puzzlemaker_newchamber");

            if (std::strstr(this->level, PUZZLEMAKER_STRING)) {
                std::strcpy(this->largeAsset.text, "Puzzler Maker");
            } else if (std::strstr(this->level, WORKSHOP_STRING)) {
                std::strcpy(this->largeAsset.text, "Workshop");
            } else {
                std::strcpy(this->largeAsset.text, "Custom Map");
            }
        }
    } else {
        std::strcpy(this->largeAsset.key, "main_menu");
        std::strcpy(this->largeAsset.text, "Main Menu");
    }
}
void Discord::ResolveRank()
{
    this->smallAsset.isActive = false;

    if (std::strlen(this->globalRank) != 0) {
        std::strcpy(this->smallAsset.key, "iverb");

        Leaderboard lb;
        Chamber chamber;
        if (!this->isMenuing
            && this->isGrinding
            && Leaderboard::TryFindByLevelName(this->level, lb)
            && lb.DoesExist()
            && this->iverb->TryGetChamber(lb.bestTimeId, chamber)) {
            auto steamId = steam->GetLocalSteamId();
            for (const auto& entry : chamber.entries) {
                if (entry.first == steamId) {
                    auto rank = std::string("Rank ") + std::to_string(entry.second.score.playerRank);
                    console->Debug("Level rank of %llu for %s -> %i\n", steamId, lb.chamberName, entry.second.score.playerRank);
                    std::strcpy(this->smallAsset.text, rank.c_str());
                    this->smallAsset.isActive = true;
                }
            }

            if (!this->smallAsset.isActive) {
                console->Debug("Unable to find rank of %llu for %s!\n", steamId, lb.chamberName);
            }
        } else {
            std::strcpy(this->smallAsset.text, this->globalRank);
            this->smallAsset.isActive = true;
        }
    }
}
void Discord::SendPresence()
{
    DiscordRichPresence presence;
    std::memset(&presence, 0, sizeof(presence));

    auto details = this->GetDetails();
    auto state = this->GetState();

    if (std::strcmp(this->details, details) != 0 || std::strcmp(this->state, state) != 0) {
        std::strcpy(this->details, details);
        std::strcpy(this->state, state);

        presence.startTimestamp = this->timestamp = time(0);
        console->Debug("New timestamp!\n");
    } else {
        presence.startTimestamp = this->timestamp;
    }

    presence.details = details;
    presence.state = state;

    this->ResolveLevel();
    this->ResolveRank();

    if (this->largeAsset.isActive) {
        presence.largeImageKey = this->largeAsset.key;
        presence.largeImageText = this->largeAsset.text;
    }
    if (this->smallAsset.isActive) {
        presence.smallImageKey = this->smallAsset.key;
        presence.smallImageText = this->smallAsset.text;
    }

    Discord_UpdatePresence(&presence);
}
void Discord::Update()
{
    auto change = false;

    // Detect changes
    DETECT_CHANGE_S(this->level, engine->hoststate->m_levelName)
    DETECT_CHANGE_B(this->isCooping, engine->GetMaxClients() >= 2)
    DETECT_CHANGE_B(this->isGrinding, sv_bonus_challenge.GetBool())
    DETECT_CHANGE_B(this->isRouting, sv_cheats.GetBool())
    DETECT_CHANGE_B(this->isViewing, engine->IsPlayingBack(engine->demoplayer))
    DETECT_CHANGE_B(this->isRendering, engine->CL_IsRecordingMovie())
    DETECT_CHANGE_B(this->isMenuing, engine->IsInMenu())
    DETECT_CHANGE_B(this->isListening, engine->IsInCommentaryMode())

    if (change) {
        console->Debug("Change!\n");
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
    console->Debug("Discord Ready -> %s#%s\n", request->username, request->discriminator);
}
void Discord::OnDiscordDisconnected(int errorCode, const char* message)
{
    console->Debug("Discord Disconnected -> %i\n%s\n", errorCode, message);
}
void Discord::OnDiscordErrored(int errorCode, const char* message)
{
    console->Debug("Discord Errored -> %i\n%s\n", errorCode, message);
}
