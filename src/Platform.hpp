#pragma once

#ifdef _WIN32
#define MODULE_EXTENSION ".dll"
#define __func __thiscall
#define DLL_EXPORT extern "C" __declspec(dllexport)
#define GO_THE_FUCK_TO_SLEEP(ms) Sleep(ms)
#else
#define MODULE_EXTENSION ".so"
#define __func2 __attribute__((__cdecl__))
#define __cdecl __attribute__((__cdecl__))
#define __stdcall __attribute__((__stdcall__))
#define __fastcall __attribute__((__fastcall__))
#define DLL_EXPORT extern "C" __attribute__((visibility("default")))
#define GO_THE_FUCK_TO_SLEEP(ms) usleep(ms)
#endif

#define MODULE(name) name MODULE_EXTENSION
