#pragma once
#include "Interface.hpp"
#include "Module.hpp"

#include "SDK.hpp"

class Tier1 : public Module {
public:
    Interface* g_pCVar;
    ConCommandBase* m_pConCommandList;

    _RegisterConCommand RegisterConCommand;
    _UnregisterConCommand UnregisterConCommand;
    _FindCommandBase FindCommandBase;

    void* ConCommand_VTable;
    void* ConVar_VTable;
    void* ConVar_VTable2;
    _AutoCompletionFunc AutoCompletionFunc;

    Tier1();
    bool Init() override;
    void Shutdown() override;
};

extern Tier1* tier1;
