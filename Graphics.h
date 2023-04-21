#ifndef GRAPHICS_H_INCLUDED
#define GRAPHICS_H_INCLUDED

#pragma once

#include "LuaEasyX.h"

#ifdef BUILD_DLL
    #define LUAEASYX_EXPORT __declspec(dllexport)
#else
    #define LUAEASYX_EXPORT __declspec(dllimport)
#endif


extern "C" LUAEASYX_EXPORT int luaopen_Graphics(lua_State* L);

#endif // GRAPHICS_H_INCLUDED
