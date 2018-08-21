#pragma once
#include "SDK.hpp"

#define SGP_SIGNATURE \
    new char[11] { 115, 101, 110, 100, 32, 110, 117, 100, 101, 115, 00 }

// CServerPlugin
#define CServerPlugin_m_Size 16
#define CServerPlugin_m_Plugins 4

class Plugin {
public:
    CPlugin* ptr;
    int index;
    bool found;

public:
    Plugin();
};
