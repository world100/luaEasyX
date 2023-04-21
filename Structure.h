#ifndef STRUCTURE_H_INCLUDED
#define STRUCTURE_H_INCLUDED

#pragma once

#include "LuaEasyX.h"



#ifdef BUILD_DLL
    #define LUAEASYX_EXPORT __declspec(dllexport)
#else
    #define LUAEASYX_EXPORT __declspec(dllimport)
#endif

typedef struct LuaEasyX_UserStyle {
    DWORD	*puserstyle;
    DWORD	userstylecount;
} LuaEasyX_UserStyle;


////////////////////////////////////////////////////////////////////////

typedef struct LuaEasyX_StructureType {
    DWORD uStructType;      //结构体类型
    PVOID StructAddress;    //结构体指针
    DWORD uEndStructType;   //结构体类型结束标记
} LuaEasyX_StructureType;

#define LUAEASYX_STRUCTTYP_NONETYPE     0
#define LUAEASYX_STRUCTTYP_MOUSEMSG     1
#define LUAEASYX_STRUCTTYP_IMAGE        2
#define LUAEASYX_STRUCTTYP_POINTSET     3
#define LUAEASYX_STRUCTTYP_HGDIOBJ      4


//#define LUAEASYX_STRUCTTYP_USERSTYLE    3


typedef struct stktype {
    unsigned int type   : 8;
    unsigned int type_id: 16;
    unsigned int endtype: 8;
} stktype;


typedef struct LuaEasyX_Constant {
    TCHAR* ConstantName;
    UINT uConstantValue;
} LuaEasyX_Constant;



extern "C" {
int __gc_destroy_structure(lua_State* L);

int GetObjectAddress(lua_State* L);

void l_setobj(lua_State* L, lua_CFunction luaopenfunction, TCHAR* name, int index);

void l_getitemintable(lua_State* L, int tbindex, TCHAR* keyname);


int c_comment_metatable(
    lua_State* L,
    LPCTSTR lpObjectClassName,
    PVOID   pObjAddr,
    DWORD   dwType,
    BOOL*   pbIsNew,
    LPCTSTR lpArgsErr,
    LPCTSTR lpNotFoundErr,
    LPCTSTR lpMemoryErr
);

bool l_lua_checkboolean(lua_State* L, int index);

}


#define COMMENT_CREATOR_ARGUMENTSERROR    -1
#define COMMENT_CREATOR_NOTFOUNDMETATABLE -2
#define COMMENT_CREATOR_MEMORYERROR       -3
#define COMMENT_CREATOR_SUCCESS_ONE        1
#define COMMENT_CREATOR_SUCCESS_TWO        2



#endif // STRUCTURE_H_INCLUDED
