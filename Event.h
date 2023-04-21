#ifndef EVENT_H_INCLUDED
#define EVENT_H_INCLUDED

#pragma once

#include "LuaEasyX.h"
#include "conio.h"

#ifdef BUILD_DLL
    #define LUAEASYX_EXPORT __declspec(dllexport)
#else
    #define LUAEASYX_EXPORT __declspec(dllimport)
#endif


extern "C" {
LUAEASYX_EXPORT int luaopen_Event(lua_State* L);

LUAEASYX_EXPORT int luaopen_CreateMouseEvent(lua_State* L);

}

#endif // EVENT_H_INCLUDED
