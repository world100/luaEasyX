#include "Structure.h"

#include <iostream>


//结构体、对象统一gc
int __gc_destroy_structure(lua_State* L) {
    DEBUGOUT("进入__gc！");
    if (lua_type(L, 1) == LUA_TUSERDATA) {
        LuaEasyX_StructureType *pStruct = (LuaEasyX_StructureType*)lua_touserdata(L, 1);
        if (pStruct && \
            (pStruct->uStructType == pStruct->uEndStructType) && \
            pStruct->StructAddress) {

            switch (pStruct->uStructType) {
            case LUAEASYX_STRUCTTYP_MOUSEMSG:
                delete (MOUSEMSG*)(pStruct->StructAddress);
                DEBUGOUT("MOUSEMSG被回收");
                break;
            case LUAEASYX_STRUCTTYP_IMAGE:
                delete (IMAGE*)(pStruct->StructAddress);
                DEBUGOUT("IMAGE被回收");
                break;
            case LUAEASYX_STRUCTTYP_POINTSET:
                delete[] (POINT*)(pStruct->StructAddress);
                DEBUGOUT("POINT被回收");
                break;
            case LUAEASYX_STRUCTTYP_HGDIOBJ:
                DeleteObject((HGDIOBJ)(pStruct->StructAddress));
                DEBUGOUT("HGDIOBJ被回收");
                break;
            default:break;
            }

            pStruct->StructAddress = NULL;
            DEBUGOUT("对象 " << pStruct << " 被销毁！");
        }
    }
    return 0;
}



/////////////////////////通用元表///////////////////////////////////
//返回值：
//栈顶：userdata
//栈顶之下第一个位置：mettable
//如果pObjAddr为NULL则不创建userdata，栈顶是metatable
int c_comment_metatable(
    lua_State* L,               //Lua状态机
    LPCTSTR lpObjectClassName,  //ClassName
    PVOID   pObjAddr,           //userdata：对象真实地址, NULL则不申请LuaEasyX_Struct
    DWORD   dwType,             //类型, LUAEASYX_STRUCTTYP_NONETYPE 不设置通用__gc
    BOOL*   pbIsNew,            //判断元表是否是新表
    LPCTSTR lpArgsErr,          //没有传入ClassName
    LPCTSTR lpNotFoundErr,      //注册表内找不到元表
    LPCTSTR lpMemoryErr         //LuaEasyX_Struct对象申请失败
) {

    if (!lpObjectClassName) {
        if (!lpArgsErr)
            luaL_error(L, lpArgsErr);
        return COMMENT_CREATOR_ARGUMENTSERROR;
    }


    int metatable_index;

    if (!luaL_newmetatable(L, lpObjectClassName)) {
        if (!luaL_getmetatable(L, lpObjectClassName)) {
            if (!lpNotFoundErr)
                luaL_error(L, lpNotFoundErr);
            return COMMENT_CREATOR_NOTFOUNDMETATABLE;
        } else {
            metatable_index = lua_gettop(L);
            *pbIsNew = FALSE;
        }
    } else {
        metatable_index = lua_gettop(L);

        *pbIsNew = TRUE;

        if (dwType != LUAEASYX_STRUCTTYP_NONETYPE) {
            lua_pushstring(L, "__gc");
            lua_pushcfunction(L, __gc_destroy_structure);
            lua_rawset(L, metatable_index);
        }
    }

    if (!pObjAddr || dwType == LUAEASYX_STRUCTTYP_NONETYPE)
        goto _RETURN_;

    LuaEasyX_StructureType *pls;

    pls = (LuaEasyX_StructureType*)lua_newuserdata(L, sizeof(LuaEasyX_StructureType));
    if (!pls) {
        if (!lpMemoryErr)
            luaL_error(L, lpMemoryErr);
        return COMMENT_CREATOR_MEMORYERROR;
    }

    int userdata_index = lua_gettop(L);

    pls->uStructType = dwType;
    pls->uEndStructType = dwType;

    pls->StructAddress = pObjAddr;

    lua_pushvalue(L, metatable_index);
    lua_setmetatable(L, userdata_index);

    return COMMENT_CREATOR_SUCCESS_TWO;
_RETURN_:return COMMENT_CREATOR_SUCCESS_ONE;
}




//获取对象地址
int GetObjectAddress(lua_State* L) {
    if (lua_type(L, lua_gettop(L)) == LUA_TUSERDATA) {
        LuaEasyX_StructureType *pStruct = (LuaEasyX_StructureType*)lua_touserdata(L, lua_gettop(L));
        if (pStruct->uStructType == pStruct->uEndStructType) {
            if (pStruct->StructAddress) {
                lua_pushlightuserdata(L, pStruct->StructAddress);
                return 1;
            }
        }
    }

    return 0;
}


//将luaopen得到的值添加进index中的表中
void l_setobj(lua_State* L, lua_CFunction luaopenfunction, TCHAR* name, int index) {
    lua_pushstring(L, name);
    if (luaopenfunction(L) != 0)
        lua_settable(L, index);
    else
        lua_remove(L, -1);
}

//获取tbindex表中的值
void l_getitemintable(lua_State* L, int tbindex, TCHAR* keyname) {
    lua_pushstring(L, keyname);
    lua_rawget(L, tbindex);
}

bool l_lua_checkboolean(lua_State* L, int index) {
    if (lua_type(L, index) != LUA_TBOOLEAN) {
        luaL_argerror(L, index, TEXT("number expected, got no value"));
    }

    return (lua_toboolean(L, index) ? true : false);
}

