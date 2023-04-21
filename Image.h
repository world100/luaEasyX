#ifndef IMAGE_H_INCLUDED
#define IMAGE_H_INCLUDED

#include "LuaEasyX.h"

#ifdef BUILD_DLL
    #define LUAEASYX_EXPORT __declspec(dllexport)
#else
    #define LUAEASYX_EXPORT __declspec(dllimport)
#endif

extern "C" LUAEASYX_EXPORT int luaopen_Image(lua_State* L);


#endif // IMAGE_H_INCLUDED
