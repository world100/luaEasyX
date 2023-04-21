#ifndef __MAIN_H__
#define __MAIN_H__

//author: lovingxiaobing | Ð¡±ùÓ´

#include <windows.h>
#include <mmsystem.h>

#include <lua.hpp>

#include <graphics.h>

#include "DrawDevice.h"
#include "Color.h"
#include "Structure.h"
#include "Style.h"
#include "Graphics.h"
#include "Option.h"
#include "Extend.h"
#include "Event.h"
#include "WinGUI.h"
#include "Constant.h"
#include "Image.h"
#include "Text.h"


#pragma comment(lib, "lua53.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "gdiplus.lib")
#pragma comment(lib, "ole32.lib")
#pragma comment(lib, "kernel32.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "shell32.lib")

#ifdef BUILD_DLL
    #define LUAEASYX_EXPORT __declspec(dllexport)
#else
    #define LUAEASYX_EXPORT __declspec(dllimport)
#endif


#define DEBUG 0
#if DEBUG
#include <iostream>
#include <cstdio>
#define DEBUGOUT(v) std::cout<< v << std::endl
#else
#define DEBUGOUT(v)
#endif // DEBUG


#ifdef __cplusplus
extern "C"
{
#endif

LUAEASYX_EXPORT int luaopen_LuaEasyX(lua_State* L);

#ifdef __cplusplus
}
#endif

#endif // __MAIN_H__
