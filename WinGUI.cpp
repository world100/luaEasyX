
#include "WinGUI.h"

#pragma comment(lib, "winmm.lib")


//使用DeleteObject清除对象
TCHAR* WINGUI_OBJECT_GDIClassName = TEXT("WINGUI_HGDIOBJ");

int c_CreateGdiOecjt(lua_State* L, PVOID pgdiobj) {
    BOOL bIsNew;
    return c_comment_metatable(
        L,
        WINGUI_OBJECT_GDIClassName,
        pgdiobj,
        LUAEASYX_STRUCTTYP_HGDIOBJ,
        &bIsNew,
        NULL, NULL, NULL
    );
}

//主动删除对象
int WinGUI_DeleteObject(lua_State* L) {
    LuaEasyX_StructureType* pls;
    pls = (LuaEasyX_StructureType*)luaL_checkudata (L, 1, WINGUI_OBJECT_GDIClassName);
    if (pls->StructAddress && pls->uEndStructType == pls->uStructType \
            && pls->uStructType == LUAEASYX_STRUCTTYP_HGDIOBJ) {
        HGDIOBJ hObj = (HGDIOBJ)(pls->StructAddress);
        DeleteObject(hObj);
        pls->StructAddress = NULL;
        DEBUGOUT(TEXT("主动删除HGDIOBJ对象!"));
    }

    return 0;
}


//////////////////////// 创建区域 /////////////////////////

/*
HRGN CreateRectRgn(
  int x1,
  int y1,
  int x2,
  int y2
);
*/
//创建矩形区域
int WinGUI_CreateRectRgn(lua_State* L) {
    int x1 = (int)luaL_checkinteger(L, 1);
    int y1 = (int)luaL_checkinteger(L, 2);
    int x2 = (int)luaL_checkinteger(L, 3);
    int y2 = (int)luaL_checkinteger(L, 4);

    HRGN hRgn = ::CreateRectRgn(x1, y1, x2, y2);

    int ret = c_CreateGdiOecjt(L, (PVOID)hRgn);

    if (ret == COMMENT_CREATOR_NOTFOUNDMETATABLE) {
        DeleteObject((HGDIOBJ)hRgn);
    } else if (ret == COMMENT_CREATOR_SUCCESS_TWO) {
        return 1;
    }

    return 0;
}


/*
HRGN CreateEllipticRgn(
  int x1,
  int y1,
  int x2,
  int y2
);
*/
//创建椭圆形的区域
int WinGUI_CreateEllipticRgn(lua_State* L) {
    int x1 = (int)luaL_checkinteger(L, 1);
    int y1 = (int)luaL_checkinteger(L, 2);
    int x2 = (int)luaL_checkinteger(L, 3);
    int y2 = (int)luaL_checkinteger(L, 4);

    HRGN hRgn = ::CreateEllipticRgn(x1, y1, x2, y2);

    int ret = c_CreateGdiOecjt(L, (PVOID)hRgn);

    if (ret == COMMENT_CREATOR_NOTFOUNDMETATABLE) {
        DeleteObject((HGDIOBJ)hRgn);
    } else if (ret == COMMENT_CREATOR_SUCCESS_TWO) {
        return 1;
    }

    return 0;
}


/*
HRGN CreatePolygonRgn(
  const POINT *pptl,
  int         cPoint,
  int         iMode
);
*/
//创建多边形的区域
int WinGUI_CreatePolygonRgn(lua_State* L) {
    //传入GetObjectAddress获取到的地址
    if (lua_type(L, 1) != LUA_TLIGHTUSERDATA)
        return 0;
    const POINT* pplt = (const POINT*)lua_touserdata(L, 1);
    int cPoint = (int)luaL_checkinteger(L, 2);
    int iMode  = (int)luaL_checkinteger(L, 3);

    HRGN hRgn = ::CreatePolygonRgn(pplt, cPoint, iMode);

    int ret = c_CreateGdiOecjt(L, (PVOID)hRgn);

    if (ret == COMMENT_CREATOR_NOTFOUNDMETATABLE) {
        DeleteObject((HGDIOBJ)hRgn);
    } else if (ret == COMMENT_CREATOR_SUCCESS_TWO) {
        return 1;
    }

    return 0;
}


/*
HRGN CreateRoundRectRgn(
  int x1,
  int y1,
  int x2,
  int y2,
  int w,
  int h
);
*/
//创建圆角矩形区域
int WinGUI_CreateRoundRectRgn(lua_State* L) {
    int x1 = (int)luaL_checkinteger(L, 1);
    int y1 = (int)luaL_checkinteger(L, 2);
    int x2 = (int)luaL_checkinteger(L, 3);
    int y2 = (int)luaL_checkinteger(L, 4);
    int w = (int)luaL_checkinteger(L, 5);
    int h = (int)luaL_checkinteger(L, 6);

    HRGN hRgn = ::CreateRoundRectRgn(x1, y1, x2, y2, w, h);

    int ret = c_CreateGdiOecjt(L, (PVOID)hRgn);

    if (ret == COMMENT_CREATOR_NOTFOUNDMETATABLE) {
        DeleteObject((HGDIOBJ)hRgn);
    } else if (ret == COMMENT_CREATOR_SUCCESS_TWO) {
        return 1;
    }

    return 0;
}


/*
int CombineRgn(
  HRGN hrgnDst,
  HRGN hrgnSrc1,
  HRGN hrgnSrc2,
  int  iMode
);
*/
//合并两个区域
int WinGUI_CombineRgn(lua_State* L) {
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    luaL_checktype(L, 2, LUA_TLIGHTUSERDATA);
    luaL_checktype(L, 3, LUA_TLIGHTUSERDATA);

    int iMode = (int)luaL_checkinteger(L, 4);

    int iCRRet = ::CombineRgn(
        (HRGN)lua_touserdata(L, 1),
        (HRGN)lua_touserdata(L, 2),
        (HRGN)lua_touserdata(L, 3),
        iMode
    );

    lua_pushinteger(L, (lua_Integer)(iCRRet));

    return 1;
}



///////////////////////////////////////////////////////////


int WinGUI_MessageBox(lua_State* L) {
    HWND hWnd       = (HWND)lua_touserdata(L, 1);
    LPSTR lpText    = (LPSTR)lua_tostring(L, 2);
    LPSTR lpCaption = (LPSTR)lua_tostring(L, 3);
    UINT uType      = (UINT)luaL_checkinteger(L, 4);

    int ret = MessageBox(hWnd, lpText, lpCaption, uType);
    lua_pushinteger(L, (lua_Integer)(ret));

    return 1;
}

int WinGUI_SetWindowText(lua_State* L) {
    HWND hwnd;
    LPCSTR lpString;
    switch (lua_gettop(L)) {
    case 2:
        hwnd = (HWND)lua_touserdata(L, 1);
        lpString = (LPCSTR)lua_tostring(L, 2);
        SetWindowText(hwnd, lpString);
        lua_pushboolean(L, (int)true);
        break;
    default:
        lua_pushboolean(L, (int)false);
    }

    return 1;
}



/*
int GetSystemMetrics(
  int nIndex
);
*/
//
int WinGUI_GetSystemMetrics(lua_State* L) {
    int nIndex = (int)luaL_checkinteger(L, 1);
    int iRet = GetSystemMetrics(nIndex);
    lua_pushinteger(L, (lua_Integer)iRet);
    return 1;
}


/*
SHORT GetAsyncKeyState(
  int vKey
);
*/
//
int WinGUI_GetAsyncKeyState(lua_State* L) {
    int vKey = (int)luaL_checkinteger(L, 1);
    SHORT ret = ::GetAsyncKeyState(vKey);
    lua_pushinteger(L, (lua_Integer)(ret));
    return 1;
}


/*
SHORT GetKeyState(
  int nVirtKey
);
*/
//
int WinGUI_GetKeyState(lua_State* L) {
    int nVirKey = (int)luaL_checkinteger(L, 1);
    SHORT ret = ::GetKeyState(nVirKey);
    lua_pushinteger(L, (lua_Integer)(ret));
    return 1;
}


/*
UINT MapVirtualKey(
  UINT uCode,
  UINT uMapType
);
*/
//
int WinGUI_MapVirtualKey(lua_State* L) {
    UINT uCode = (UINT)luaL_checkinteger(L, 1);
    UINT uMapType = (UINT)luaL_checkinteger(L, 2);

    UINT ret = ::MapVirtualKey(uCode, uMapType);

    lua_pushinteger(L, (lua_Integer)(ret));

    return 1;
}


/*
MCIERROR mciSendString(
    LPCTSTR lpszCommand,
    LPTSTR lpszReturnString,
    UINT cchReturn,
    HANDLE hwndCallback
);
*/
//
int WinGUI_mciSendString(lua_State* L) {
    LPCTSTR lpszCommand = (LPCTSTR)luaL_checkstring(L, 1);

    MCIERROR e = ::mciSendString(lpszCommand, NULL, 0, NULL);

    lua_pushinteger(L, (lua_Integer)(e));

    return 1;
}



TCHAR* WinGUIClassName = TEXT("LuaEasyX_WinGUI");

#define WINGUI_METHOD(name) {#name, WinGUI_##name}

const struct luaL_Reg wingui_methods[] = {
    WINGUI_METHOD(SetWindowText),
    WINGUI_METHOD(MessageBox),
    WINGUI_METHOD(DeleteObject),
    WINGUI_METHOD(CreateRectRgn),
    WINGUI_METHOD(CreateEllipticRgn),
    WINGUI_METHOD(CreatePolygonRgn),
    WINGUI_METHOD(CreateRoundRectRgn),
    WINGUI_METHOD(CombineRgn),
    WINGUI_METHOD(GetSystemMetrics),
    WINGUI_METHOD(GetAsyncKeyState),
    WINGUI_METHOD(GetKeyState),
    WINGUI_METHOD(MapVirtualKey),
    WINGUI_METHOD(mciSendString),
    {NULL, NULL}
};

int CreateWinGUI(lua_State* L) {

    int top = lua_gettop(L);

    BOOL bIsNew;

    int ret = c_comment_metatable(L,
        WinGUIClassName,
        NULL,
        LUAEASYX_STRUCTTYP_NONETYPE,
        &bIsNew,
        NULL,
        TEXT("WinGUI函数表丢失！"),
        NULL
    );

    int wingui_methods_index;

    if (ret == COMMENT_CREATOR_SUCCESS_ONE) {

        if (bIsNew) {
             wingui_methods_index = lua_gettop(L);

             luaL_setfuncs(L, (luaL_Reg*)wingui_methods, 0);
        }
    } else {
        lua_settop(L, top);
        return 0;
    }

    return 1;
}

LUAEASYX_EXPORT int luaopen_WinGUI(lua_State* L) {
    return CreateWinGUI(L);
}


