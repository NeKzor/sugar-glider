#pragma once
#include <cmath>
#include <cstdint>

#ifdef _WIN32
#define __funccc __thiscall
#else
#define __funccc __attribute__((__cdecl__))
#endif

struct Vector {
    float x, y, z;
    inline float Length()
    {
        return sqrt(x * x + y * y + z * z);
    }
    inline float Length2D()
    {
        return sqrt(x * x + y * y);
    }
    inline Vector operator*(float fl)
    {
        Vector res;
        res.x = x * fl;
        res.y = y * fl;
        res.z = z * fl;
        return res;
    }
    inline float& operator[](int i)
    {
        return ((float*)this)[i];
    }
    inline float operator[](int i) const
    {
        return ((float*)this)[i];
    }
};

struct QAngle {
    float x, y, z;
};

struct Color {
    Color()
    {
        *((int*)this) = 255;
    }
    Color(int _r, int _g, int _b)
    {
        SetColor(_r, _g, _b, 255);
    }
    Color(int _r, int _g, int _b, int _a)
    {
        SetColor(_r, _g, _b, _a);
    }
    void SetColor(int _r, int _g, int _b, int _a = 255)
    {
        _color[0] = (unsigned char)_r;
        _color[1] = (unsigned char)_g;
        _color[2] = (unsigned char)_b;
        _color[3] = (unsigned char)_a;
    }
    inline int r() const { return _color[0]; }
    inline int g() const { return _color[1]; }
    inline int b() const { return _color[2]; }
    inline int a() const { return _color[3]; }
    unsigned char _color[4];
};

#define FCVAR_DEVELOPMENTONLY (1 << 1)
#define FCVAR_HIDDEN (1 << 4)
#define FCVAR_NEVER_AS_STRING (1 << 12)
#define FCVAR_CHEAT (1 << 14)

#define COMMAND_COMPLETION_MAXITEMS 64
#define COMMAND_COMPLETION_ITEM_LENGTH 64

struct CCommand;
class ConCommandBase;

using _CommandCallback = void (*)(const CCommand& args);
using _CommandCompletionCallback = int (*)(const char* partial, char commands[COMMAND_COMPLETION_MAXITEMS][COMMAND_COMPLETION_ITEM_LENGTH]);
using _InternalSetValue = void(__funccc*)(void* thisptr, const char* value);
using _InternalSetFloatValue = void(__funccc*)(void* thisptr, float value);
using _InternalSetIntValue = void(__funccc*)(void* thisptr, int value);
using _RegisterConCommand = void(__funccc*)(void* thisptr, ConCommandBase* pCommandBase);
using _UnregisterConCommand = void(__funccc*)(void* thisptr, ConCommandBase* pCommandBase);
using _FindCommandBase = void*(__funccc*)(void* thisptr, const char* name);
using _AutoCompletionFunc = int(__funccc*)(void* thisptr, char const* partial, char commands[COMMAND_COMPLETION_MAXITEMS][COMMAND_COMPLETION_ITEM_LENGTH]);

class ConCommandBase {
public:
    void* ConCommandBase_VTable; // 0
    ConCommandBase* m_pNext; // 4
    bool m_bRegistered; // 8
    const char* m_pszName; // 12
    const char* m_pszHelpString; // 16
    int m_nFlags; // 20

public:
    ConCommandBase()
        : ConCommandBase_VTable(nullptr)
        , m_pNext(nullptr)
        , m_bRegistered(false)
        , m_pszName(nullptr)
        , m_pszHelpString(nullptr)
        , m_nFlags(0)
    {
    }
};

struct CCommand {
    enum {
        COMMAND_MAX_ARGC = 64,
        COMMAND_MAX_LENGTH = 512
    };
    int m_nArgc;
    int m_nArgv0Size;
    char m_pArgSBuffer[COMMAND_MAX_LENGTH];
    char m_pArgvBuffer[COMMAND_MAX_LENGTH];
    const char* m_ppArgv[COMMAND_MAX_ARGC];

    int ArgC() const
    {
        return this->m_nArgc;
    }
    const char* Arg(int nIndex) const
    {
        return this->m_ppArgv[nIndex];
    }
    const char* operator[](int nIndex) const
    {
        return Arg(nIndex);
    }
};

class ConCommand : public ConCommandBase {
public:
    union {
        void* m_fnCommandCallbackV1;
        _CommandCallback m_fnCommandCallback;
        void* m_pCommandCallback;
    };

    union {
        _CommandCompletionCallback m_fnCompletionCallback;
        void* m_pCommandCompletionCallback;
    };

    bool m_bHasCompletionCallback : 1;
    bool m_bUsingNewCommandCallback : 1;
    bool m_bUsingCommandCallbackInterface : 1;

public:
    ConCommand()
        : ConCommandBase()
        , m_fnCommandCallbackV1(nullptr)
        , m_fnCompletionCallback(nullptr)
    {
    }
};

class ConVar : public ConCommandBase {
public:
    void* ConVar_VTable; // 24
    ConVar* m_pParent; // 28
    const char* m_pszDefaultValue; // 32
    char* m_pszString; // 36
    int m_StringLength; // 40
    float m_fValue; // 44
    int m_nValue; // 48
    bool m_bHasMin; // 52
    float m_fMinVal; // 56
    bool m_bHasMax; // 60
    float m_fMaxVal; // 64
    void* m_fnChangeCallback; // 68

public:
    ConVar()
        : ConCommandBase()
        , ConVar_VTable(nullptr)
        , m_pParent(nullptr)
        , m_pszDefaultValue(nullptr)
        , m_pszString(nullptr)
        , m_StringLength(0)
        , m_fValue(0)
        , m_nValue(0)
        , m_bHasMin(0)
        , m_fMinVal(0)
        , m_bHasMax(0)
        , m_fMaxVal(0)
        , m_fnChangeCallback(nullptr)
    {
    }
    ~ConVar()
    {
        if (this->m_pszString) {
            delete[] this->m_pszString;
            this->m_pszString = nullptr;
        }
    }
};

class ConVar2 : public ConVar {
public:
    // CUtlVector<FnChangeCallback_t> m_fnChangeCallback
    // CUtlMemory<FnChangeCallback_t> m_Memory
    int m_nAllocationCount; // 72
    int m_nGrowSize; // 76
    int m_Size; // 80
    void* m_pElements; // 84

public:
    ConVar2()
        : ConVar()
        , m_nAllocationCount(0)
        , m_nGrowSize(0)
        , m_Size(0)
        , m_pElements(nullptr)
    {
    }
};

typedef enum {
    HS_NEW_GAME = 0,
    HS_LOAD_GAME = 1,
    HS_CHANGE_LEVEL_SP = 2,
    HS_CHANGE_LEVEL_MP = 3,
    HS_RUN = 4,
    HS_GAME_SHUTDOWN = 5,
    HS_SHUTDOWN = 6,
    HS_RESTART = 7
} HOSTSTATES;

struct CHostState {
    HOSTSTATES m_currentState; // 0
    HOSTSTATES m_nextState; // 4
    Vector m_vecLocation; // 8, 12, 16
    QAngle m_angLocation; // 20, 24, 28
    char m_levelName[256]; // 32
    char m_landmarkName[256]; // 288
    char m_saveName[256]; // 544
    float m_flShortFrameTime; // 800
    bool m_activeGame; // 804
    bool m_bRememberLocation; // 805
    bool m_bBackgroundLevel; // 806
    bool m_bWaitingForConnection; // 807
};

#define INTERFACEVERSION_ISERVERPLUGINCALLBACKS "ISERVERPLUGINCALLBACKS002"
#define INTERFACEVERSION_ISERVERPLUGINHELPERS "ISERVERPLUGINHELPERS001"

typedef void* (*CreateInterfaceFn)(const char* pName, int* pReturnCode);
typedef void* (*InstantiateInterfaceFn)();

struct InterfaceReg {
    InstantiateInterfaceFn m_CreateFn;
    const char* m_pName;
    InterfaceReg* m_pNext;
    static InterfaceReg* s_pInterfaceRegs;

    InterfaceReg(InstantiateInterfaceFn fn, const char* pName)
        : m_pName(pName)
    {
        m_CreateFn = fn;
        m_pNext = s_pInterfaceRegs;
        s_pInterfaceRegs = this;
    }
};

class IServerPluginCallbacks {
public:
    virtual bool Load(CreateInterfaceFn interfaceFactory, CreateInterfaceFn gameServerFactory) = 0;
    virtual void Unload() = 0;
    virtual void Pause() = 0;
    virtual void UnPause() = 0;
    virtual const char* GetPluginDescription() = 0;
    virtual void LevelInit(char const* pMapName) = 0;
    virtual void ServerActivate(void* pEdictList, int edictCount, int clientMax) = 0;
    virtual void GameFrame(bool simulating) = 0;
    virtual void LevelShutdown() = 0;
    virtual void ClientFullyConnect(void* pEdict) = 0;
    virtual void ClientActive(void* pEntity) = 0;
    virtual void ClientDisconnect(void* pEntity) = 0;
    virtual void ClientPutInServer(void* pEntity, char const* playername) = 0;
    virtual void SetCommandClient(int index) = 0;
    virtual void ClientSettingsChanged(void* pEdict) = 0;
    virtual int ClientConnect(bool* bAllowConnect, void* pEntity, const char* pszName, const char* pszAddress, char* reject, int maxrejectlen) = 0;
    virtual int ClientCommand(void* pEntity, const void*& args) = 0;
    virtual int NetworkIDValidated(const char* pszUserName, const char* pszNetworkID) = 0;
    virtual void OnQueryCvarValueFinished(int iCookie, void* pPlayerEntity, int eStatus, const char* pCvarName, const char* pCvarValue) = 0;
    virtual void OnEdictAllocated(void* edict) = 0;
    virtual void OnEdictFreed(const void* edict) = 0;
};

struct CPlugin {
    char m_szName[128]; //0
    bool m_bDisable; // 128
    IServerPluginCallbacks* m_pPlugin; // 132
    int m_iPluginInterfaceVersion; // 136
    void* m_pPluginModule; // 140
};

#define EXPOSE_INTERFACE_FN(functionName, interfaceName, versionName) \
    static InterfaceReg __g_Create##interfaceName##_reg(functionName, versionName);

#define EXPOSE_INTERFACE(className, interfaceName, versionName)                                           \
    static void* __Create##className##_interface() { return static_cast<interfaceName*>(new className); } \
    static InterfaceReg __g_Create##className##_reg(__Create##className##_interface, versionName);

#define EXPOSE_SINGLE_INTERFACE_GLOBALVAR(className, interfaceName, versionName, globalVarName)                           \
    static void* __Create##className##interfaceName##_interface() { return static_cast<interfaceName*>(&globalVarName); } \
    static InterfaceReg __g_Create##className##interfaceName##_reg(__Create##className##interfaceName##_interface, versionName);

#define EXPOSE_SINGLE_INTERFACE(className, interfaceName, versionName) \
    static className __g_##className##_singleton;                      \
    EXPOSE_SINGLE_INTERFACE_GLOBALVAR(className, interfaceName, versionName, __g_##className##_singleton)

enum EUniverse {
    k_EUniverseInvalid = 0,
    k_EUniversePublic = 1,
    k_EUniverseBeta = 2,
    k_EUniverseInternal = 3,
    k_EUniverseDev = 4,
    k_EUniverseRC = 5,
    k_EUniverseMax
};

struct CSteamID {
    union SteamID_t {
        struct SteamIDComponent_t {
            uint32_t m_unAccountID : 32;
            unsigned int m_unAccountInstance : 20;
            unsigned int m_EAccountType : 4;
            EUniverse m_EUniverse : 8;
        } m_comp;

        uint64_t m_unAll64Bits;
    } m_steamid;

    uint64_t ConvertToUint64() const
    {
        return m_steamid.m_unAll64Bits;
    }
};

struct CGameID {
    enum EGameIDType {
        k_EGameIDTypeApp = 0,
        k_EGameIDTypeGameMod = 1,
        k_EGameIDTypeShortcut = 2,
        k_EGameIDTypeP2P = 3,
    };

    struct GameID_t {
        unsigned int m_nAppID : 24;
        unsigned int m_nType : 8;
        unsigned int m_nModID : 32;
    };

    union {
        unsigned long m_ulGameID;
        GameID_t m_gameID;
    };
};

#define STEAMUSER_INTERFACE_VERSION "SteamUser019"

class ISteamUser {
public:
    virtual int GetHSteamUser() = 0;
    virtual bool BLoggedOn() = 0;
    virtual CSteamID GetSteamID() = 0;
    virtual int InitiateGameConnection(void* pBlob, int cbMaxBlob, CSteamID steamID, CGameID gameID, unsigned int unIPServer, unsigned short usPortServer, bool bSecure, void* pvSteam2GetEncryptionKey, int cbSteam2GetEncryptionKey) = 0;
    virtual void TerminateGameConnection(unsigned int unIPServer, unsigned short usPortServer) = 0;
    virtual void TrackAppUsageEvent(CGameID gameID, int eAppUsageEvent, const char* pchExtraInfo = "") = 0;
    virtual void RefreshSteam2Login() = 0;
};

typedef int32_t HSteamPipe;
typedef int32_t HSteamUser;

class ISteamClient {
public:
    virtual HSteamPipe CreateSteamPipe() = 0;
    virtual bool BReleaseSteamPipe(HSteamPipe hSteamPipe) = 0;
    virtual HSteamUser ConnectToGlobalUser(HSteamPipe hSteamPipe) = 0;
    virtual HSteamUser CreateLocalUser(HSteamPipe* phSteamPipe) = 0;
    virtual void ReleaseUser(HSteamPipe hSteamPipe, HSteamUser hUser) = 0;
    virtual ISteamUser* GetISteamUser(HSteamUser hSteamUser, HSteamPipe hSteamPipe, const char* pchVersion) = 0;
    virtual void* GetISteamGameServer(HSteamUser hSteamUser, HSteamPipe hSteamPipe, const char* pchVersion) = 0;
    virtual void SetLocalIPBinding(int32_t unIP, int16_t usPort) = 0;
    virtual void* GetISteamFriends(HSteamUser hSteamUser, HSteamPipe hSteamPipe, const char* pchVersion) = 0;
    virtual void* GetISteamUtils(HSteamPipe hSteamPipe, const char* pchVersion) = 0;
    virtual void* GetISteamMatchmaking(HSteamUser hSteamUser, HSteamPipe hSteamPipe, const char* pchVersion) = 0;
    virtual void* GetISteamContentServer(HSteamUser hSteamUser, HSteamPipe hSteamPipe, const char* pchVersion) = 0;
    virtual void* GetISteamMasterServerUpdater(HSteamUser hSteamUser, HSteamPipe hSteamPipe, const char* pchVersion) = 0;
    virtual void* GetISteamMatchmakingServers(HSteamUser hSteamUser, HSteamPipe hSteamPipe, const char* pchVersion) = 0;
    virtual void* GetISteam2Bridge(HSteamUser hSteamUser, HSteamPipe hSteamPipe, const char* pchVersion) = 0;
    virtual void RunFrame() = 0;
    virtual int32_t GetIPCCallCount() = 0;
    virtual void* GetISteamUserStats(HSteamUser hSteamUser, HSteamPipe hSteamPipe, const char* pchVersion) = 0;
    virtual void* GetISteamApps(HSteamUser hSteamUser, HSteamPipe hSteamPipe, const char* pchVersion) = 0;
};
