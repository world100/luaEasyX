#ifndef TEXT_H_INCLUDED
#define TEXT_H_INCLUDED

#pragma once

#include "LuaEasyX.h"


#ifdef BUILD_DLL
    #define LUAEASYX_EXPORT __declspec(dllexport)
#else
    #define LUAEASYX_EXPORT __declspec(dllimport)
#endif


extern "C" LUAEASYX_EXPORT int luaopen_Text(lua_State* L);


#endif // TEXT_H_INCLUDED
