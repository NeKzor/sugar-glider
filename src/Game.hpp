#pragma once

enum class SourceGame {
    Unknown,
    Portal2
};

class Game {
public:
    SourceGame version;

public:
    virtual ~Game() = default;
    virtual void LoadOffsets() = 0;
    virtual const char* Version();

    bool IsPortal2Engine();

    static Game* CreateNew();
};
