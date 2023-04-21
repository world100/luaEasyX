#include "Structure.h"

#include <iostream>


//�ṹ�塢����ͳһgc
int __gc_destroy_structure(lua_State* L) {
    DEBUGOUT("����__gc��");
    if (lua_type(L, 1) == LUA_TUSERDATA) {
        LuaEasyX_StructureType *pStruct = (LuaEasyX_StructureType*)lua_touserdata(L, 1);
        if (pStruct && \
            (pStruct->uStructType == pStruct->uEndStructType) && \
            pStruct->StructAddress) {

            switch (pStruct->uStructType) {
            case LUAEASYX_STRUCTTYP_MOUSEMSG:
                delete (MOUSEMSG*)(pStruct->StructAddress);
                DEBUGOUT("MOUSEMSG������");
                break;
            case LUAEASYX_STRUCTTYP_IMAGE:
                delete (IMAGE*)(pStruct->StructAddress);
                DEBUGOUT("IMAGE������");
                break;
            case LUAEASYX_STRUCTTYP_POINTSET:
                delete[] (POINT*)(pStruct->StructAddress);
                DEBUGOUT("POINT������");
                break;
            case LUAEASYX_STRUCTTYP_HGDIOBJ:
                DeleteObject((HGDIOBJ)(pStruct->StructAddress));
                DEBUGOUT("HGDIOBJ������");
                break;
            default:break;
            }

            pStruct->StructAddress = NULL;
            DEBUGOUT("���� " << pStruct << " �����٣�");
        }
    }
    return 0;
}



/////////////////////////ͨ��Ԫ��///////////////////////////////////
//����ֵ��
//ջ����userdata
//ջ��֮�µ�һ��λ�ã�mettable
//���pObjAddrΪNULL�򲻴���userdata��ջ����metatable
int c_comment_metatable(
    lua_State* L,               //Lua״̬��
    LPCTSTR lpObjectClassName,  //ClassName
    PVOID   pObjAddr,           //userdata��������ʵ��ַ, NULL������LuaEasyX_Struct
    DWORD   dwType,             //����, LUAEASYX_STRUCTTYP_NONETYPE ������ͨ��__gc
    BOOL*   pbIsNew,            //�ж�Ԫ���Ƿ����±�
    LPCTSTR lpArgsErr,          //û�д���ClassName
    LPCTSTR lpNotFoundErr,      //ע������Ҳ���Ԫ��
    LPCTSTR lpMemoryErr         //LuaEasyX_Struct��������ʧ��
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




//��ȡ�����ַ
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


//��luaopen�õ���ֵ��ӽ�index�еı���
void l_setobj(lua_State* L, lua_CFunction luaopenfunction, TCHAR* name, int index) {
    lua_pushstring(L, name);
    if (luaopenfunction(L) != 0)
        lua_settable(L, index);
    else
        lua_remove(L, -1);
}

//��ȡtbindex���е�ֵ
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

