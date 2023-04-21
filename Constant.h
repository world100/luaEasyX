#ifndef CONSTANT_H_INCLUDED
#define CONSTANT_H_INCLUDED


#pragma once

#include "LuaEasyX.h"


#ifdef BUILD_DLL
    #define LUAEASYX_EXPORT __declspec(dllexport)
#else
    #define LUAEASYX_EXPORT __declspec(dllimport)
#endif

extern "C" LUAEASYX_EXPORT int luaopen_Constant(lua_State* L);








#ifndef VK_HANGUEL
#define VK_HANGUEL 0x15
#endif

#ifndef SM_CONVERTIBLESLATEMODE
#define SM_CONVERTIBLESLATEMODE 0x2003
#endif // SM_CONVERTIBLESLATEMODE

#ifndef SM_DIGITIZER
#define SM_DIGITIZER 94
#endif // SM_DIGITIZER

#ifndef SM_MAXIMUMTOUCHES
#define SM_MAXIMUMTOUCHES 95
#endif // SM_MAXIMUMTOUCHES

#ifndef SM_SYSTEMDOCKED
#define SM_SYSTEMDOCKED 0x2004
#endif // SM_SYSTEMDOCKED




#endif // CONSTANT_H_INCLUDED
