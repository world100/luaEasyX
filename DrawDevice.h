#ifndef DRAWDEVICE_H_INCLUDED
#define DRAWDEVICE_H_INCLUDED

#include "LuaEasyX.h"

#pragma once


#ifdef BUILD_DLL
    #define LUAEASYX_EXPORT __declspec(dllexport)
#else
    #define LUAEASYX_EXPORT __declspec(dllimport)
#endif

extern "C" LUAEASYX_EXPORT int luaopen_DrawDevice(lua_State* L);

#endif // DRAWDEVICE_H_INCLUDED
