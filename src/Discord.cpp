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
    , hasChallengeMode(false)
    , globalRank("")
    , levelRank("")
    , campaign(nullptr)
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
    this->iverb = new Client("sugar-glider/1.1");
    this->largeAsset.isActive = true;
    this->prevState = engine->hoststate->m_currentState;
}
Discord::~Discord()
{
    if (this->iverb) {
        delete this->iverb;
        this->iverb = nullptr;
    }
    this->Shutdown();
}
bool Discord::Init()
{
    DiscordEventHandlers handlers;
    std::memset(&handlers, 0, sizeof(handlers));
    handlers.ready = Discord::OnDiscordReady;
    handlers.disconnected = Discord::OnDiscordDisconnected;
    handlers.errored = Discord::OnDiscordErrored;

    auto mod = std::string(engine->GetGameDirectory());
    auto ends_with = [](const std::string& str, const std::string& suffix) {
        return str.size() >= suffix.size() && !str.compare(str.size() - suffix.size(), suffix.size(), suffix);
    };

    if (ends_with(mod, "portal2")) {
        Discord_Initialize(P2_APP_ID, &handlers, 1, P2_STEAM_APP_ID);
        this->hasChallengeMode = true;
        campaign = &Map::portal2;
    } else if (ends_with(mod, "aperturetag")) {
        Discord_Initialize(AT_APP_ID, &handlers, 1, AT_STEAM_APP_ID);
        campaign = &Map::aperturetag;
    } else if (ends_with(mod, "portal_stories")) {
        Discord_Initialize(PS_APP_ID, &handlers, 1, PS_STEAM_APP_ID);
        campaign = &Map::portal_stories;
    } else {
        console->Warning("SGP: Mod directory not supported!\n");
        return false;
    }

    Discord_RunCallbacks();

    if (this->hasChallengeMode) {
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
    }

    return true;
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
        return "Cooperative Game";
    }
    if (std::strstr(this->level, WORKSHOP_STRING)) {
        return "Workshop";
    }

    Map map;
    if (Map::GetMapByName(this->level, map, campaign)) {
        if (map.type == MapType::SinglePlayer)
            return "Single Player";
        if (map.type == MapType::Cooperative)
            return "Cooperative Game";
        if (map.type == MapType::Extras)
            return "Extras";
    }

    if (engine->hoststate->m_activeGame && std::strlen(this->level) != 0) {
        return "Custom Map";
    }

    return nullptr;
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
    if (!this->isMenuing && std::strlen(this->level) != 0) {
        Map map;
        if (Map::GetMapByName(this->level, map, campaign)) {
            std::strcpy(this->largeAsset.key, map.levelName);
            std::strcpy(this->largeAsset.text, map.chamberName);
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

    if (this->hasChallengeMode && std::strlen(this->globalRank) != 0) {
        std::strcpy(this->smallAsset.key, "iverb");

        Map map;
        Chamber chamber;
        if (!this->isMenuing
            && !this->isViewing
            && this->isGrinding
            && Map::GetMapByName(this->level, map, campaign)
            && map.HasLeaderboard()
            && this->iverb->TryGetChamber(map.bestTimeId, chamber)) {
            auto steamId = steam->GetLocalSteamId();
            for (const auto& entry : chamber.entries) {
                if (entry.first == steamId) {
                    auto rank = std::string("Rank ") + std::to_string(entry.second.score.playerRank);
                    console->Debug("Level rank of %llu for %s -> %i\n", steamId, map.chamberName, entry.second.score.playerRank);
                    std::strcpy(this->smallAsset.text, rank.c_str());
                    this->smallAsset.isActive = true;
                }
            }

            if (!this->smallAsset.isActive) {
                console->Debug("Unable to find rank of %llu for %s!\n", steamId, map.chamberName);
            }
        } else {
            std::strcpy(this->smallAsset.text, this->globalRank);
            this->smallAsset.isActive = true;
        }
    }
}
void Discord::SendPresence()
{
    auto details = this->GetDetails();
    auto state = this->GetState();

    if (!details || !state) {
        console->Debug("Ignored update!\n");
        return;
    }

    DiscordRichPresence presence;
    std::memset(&presence, 0, sizeof(presence));

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

    console->Debug("Update -> %s | %s\n", details, state);
    Discord_UpdatePresence(&presence);
}
void Discord::Update()
{
    auto change = false;

    // Detect changes
    DETECT_CHANGE_B(this->isCooping, engine->GetMaxClients() >= 2)
    DETECT_CHANGE_B(this->isGrinding, sv_bonus_challenge.GetBool())
    DETECT_CHANGE_B(this->isRouting, sv_cheats.GetBool())
    DETECT_CHANGE_B(this->isViewing, engine->IsPlayingBack(engine->demoplayer))
    DETECT_CHANGE_B(this->isRendering, engine->CL_IsRecordingMovie())
    DETECT_CHANGE_B(this->isListening, engine->IsInCommentaryMode())

    if (this->prevState != engine->hoststate->m_currentState) {
        this->prevState = engine->hoststate->m_currentState;
        DETECT_CHANGE_B(this->isMenuing, engine->IsInMenu())
    }

    if (std::strcmp(this->level, engine->m_szLevelName) != 0) {
        std::strcpy(this->level, engine->m_szLevelName);
        console->Debug("this->level = %s\n", this->level);
        change = change || std::strlen(engine->m_szLevelName) != 0;
    }

    if (change) {
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
