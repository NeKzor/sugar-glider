#include "Game.hpp"

#include "Memory.hpp"

#include "Portal2.hpp"

const char* Game::Version()
{
    return "Unknown";
}
bool Game::IsPortal2Engine()
{
    return this->version == SourceGame::Portal2;
}
Game* Game::CreateNew()
{
    if (Memory::GetProcessName() == Portal2::Process()) {
        return new Portal2();
    }
    return nullptr;
}
