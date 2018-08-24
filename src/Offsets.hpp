#pragma once

namespace Offsets {

// CCvar
extern int RegisterConCommand;
extern int UnregisterConCommand;
extern int FindCommandBase;
extern int m_pConCommandList;

// CEngineClient
extern int ClientCmd;
extern int GetMaxClients;
extern int IsHammerRunning;
extern int IsInCommentaryMode;
extern int ServerCmdKeyValues;

// ConVar
extern int InternalSetValue;
extern int InternalSetFloatValue;
extern int InternalSetIntValue;

// CGameEventManager
extern int AddListener;
extern int RemoveListener;
extern int FireEventClientSide;
extern int FireEventIntern;
extern int ConPrintEvent;

// CClientState
extern int SetSignonState;
extern int Disconnect;

// CDemoPlayer
extern int IsPlayingBack;

// Others
extern int GetClientStateFunction;
extern int cl;
extern int demoplayer;
extern int AutoCompletionFunc;
extern int HostState_OnClientConnected;
extern int hoststate;
extern int CL_IsRecordingMovie;
}
