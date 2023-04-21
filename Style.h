#ifndef STYLE_H_INCLUDED
#define STYLE_H_INCLUDED

#pragma once

#include "LuaEasyX.h"


#ifdef BUILD_DLL
    #define LUAEASYX_EXPORT __declspec(dllexport)
#else
    #define LUAEASYX_EXPORT __declspec(dllimport)
#endif

extern "C" {

LUAEASYX_EXPORT int luaopen_LINESTYLE(lua_State* L);

LUAEASYX_EXPORT int luaopen_Style(lua_State* L);

}


#endif // STYLE_H_INCLUDED
