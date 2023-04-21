#include "Style.h"



/*
COLORREF getbkcolor();
*/
//获取当前绘图背景色
int Style_getbkcolor(lua_State* L) {
    lua_pushinteger(L, (lua_Integer)(::getbkcolor()));
    return 1;
}


/*
int getbkmode();
*/
//获取图案填充和文字输出时的背景模式
int Style_getbkmode(lua_State* L) {
    lua_pushinteger(L, (lua_Integer)(::getbkmode()));
    return 1;
}


/*
COLORREF getfillcolor();
*/
//获取当前的填充颜色
int Style_getfillcolor(lua_State* L) {
    lua_pushinteger(L, (lua_Integer)(::getfillcolor()));
    return 1;
}


/*
COLORREF getlinecolor();
*/
//获取当前画线颜色
int Style_getlinecolor(lua_State* L) {
    lua_pushinteger(L, (lua_Integer)(::getlinecolor()));
    return 1;
}


/*
int getpolyfillmode();
*/
//获取当前多边形填充模式
int Style_getpolyfillmode(lua_State* L) {
    lua_pushinteger(L, (lua_Integer)(::getpolyfillmode()));
    return 1;
}


/*
int getrop2();
*/
//获取前景的二元光栅操作模式
int Style_getrop2(lua_State* L) {
    lua_pushinteger(L, (lua_Integer)(::getrop2()));
    return 1;
}


/*
void setbkcolor(COLORREF color);
*/
//设置当前绘图背景色
int Style_setbkcolor(lua_State* L) {
    int top = lua_gettop(L);
    if (top > 0) {
        COLORREF color = (COLORREF)luaL_checkinteger(L, top);
        ::setbkcolor(color);
    }
    return 0;
}


/*
void setbkmode(int mode);
*/
//设置图案填充和文字输出时的背景模式
int Style_setbkmode(lua_State* L) {
    int top = lua_gettop(L);
    if (top > 0) {
        int mode = (int)luaL_checkinteger(L, top);
        ::setbkmode(mode);
    }
    return 0;
}


/*
void setfillcolor(COLORREF color);
*/
//设置当前的填充颜色
int Style_setfillcolor(lua_State* L) {
    int top = lua_gettop(L);
    if (top > 0) {
        COLORREF color = (COLORREF)luaL_checkinteger(L, top);
        ::setfillcolor(color);
    }
    return 0;
}


/*
void setlinecolor(COLORREF color);
*/
//设置当前画线颜色
int Style_setlinecolor(lua_State* L) {
    int top = lua_gettop(L);
    if (top > 0) {
        COLORREF color = (COLORREF)luaL_checkinteger(L, top);
        ::setlinecolor(color);
    }
    return 0;
}


/*
void setpolyfillmode(int mode);
*/
//设置当前多边形填充模式
int Style_setpolyfillmode(lua_State* L) {
    int top = lua_gettop(L);
    if (top > 0) {
        int mode = (int)luaL_checkinteger(L, top);
        ::setpolyfillmode(mode);
    }
    return 0;
}


/*
void setrop2(int mode);
*/
//设置前景的二元光栅操作模式
int Style_setrop2(lua_State* L) {
    int top = lua_gettop(L);
    if (top > 0) {
        int mode = (int)luaL_checkinteger(L, top);
        ::setrop2(mode);
    }
    return 0;
}


/*
void getfillstyle(
    FILLSTYLE* pstyle
);
*/
//获取当前填充样式
//返回值：style
//返回值2：hatch
//返回值3：ppattern (lightuserdata)
int Style_getfillstyle(lua_State* L) {
    FILLSTYLE cStyle;

    ::getfillstyle(&cStyle);

    lua_pushinteger(L, (lua_Integer)(cStyle.style));
    lua_pushinteger(L, (lua_Integer)(cStyle.hatch));
    lua_pushlightuserdata(L, cStyle.ppattern);

    return 3;
}


/*
void getlinestyle(
    LINESTYLE* pstyle
);
*/
//获取当前画线样式
//返回值1：style
//返回值2：thickness
int Style_getlinestyle(lua_State* L) {
    LINESTYLE cStyle;

    ::getlinestyle(&cStyle);

    lua_pushinteger(L, (lua_Integer)(cStyle.style));
    lua_pushinteger(L, (lua_Integer)(cStyle.thickness));

    return 2;
}


/*
void setfillstyle(
    FILLSTYLE* pstyle
);

void setfillstyle(
    int style,
    long hatch = NULL,
    IMAGE* ppattern = NULL
);

void setfillstyle(
    BYTE* ppattern8x8
);
*/
//
//设置当前填充样式
//值提供第二种（三个参数）的重载函数的封装
//参数1：必要
//参数2：默认要设为nil
//参数3：默认要设为nil
int Style_setfillstyle(lua_State* L) {
    int style = (int)luaL_checkinteger(L, 1);

    long hatch = (lua_type(L, 2) != LUA_TNIL) ? (long)luaL_checkinteger(L, 2) : 0;
    IMAGE* ppattern = (lua_type(L, 3) == LUA_TLIGHTUSERDATA) ? (IMAGE*)lua_touserdata(L, 3) : NULL;

    ::setfillstyle(style, hatch, ppattern);

    return 0;
}


/*
void setlinestyle(
    int style,
    int thickness = 1,
    const DWORD *puserstyle = NULL,
    DWORD userstylecount = 0
);
*/
//设置当前画线样式
//注意的是，这里不提供最后两个参数的功能！
int Style_setlinestyle(lua_State* L) {
    int style;
    int thickness = 1;

    int top = lua_gettop(L);

    if (top > 0) {
        style = (int)luaL_checkinteger(L, 1);
        if (top == 2) {
            thickness = (int)luaL_checkinteger(L, 2);
        }
        if (top > 2) {
            return 0;
        }
        ::setlinestyle(style, thickness, NULL, 0);
    }

    return 0;
}


TCHAR* StyleClassName = TEXT("LuaEasyX_Style");

#define STYLE_METHOD(name) {#name, Style_##name}

const struct luaL_Reg stylemethods[] = {
    STYLE_METHOD(getbkcolor),
    STYLE_METHOD(getbkmode),
    STYLE_METHOD(getfillcolor),
    STYLE_METHOD(getfillstyle),
    STYLE_METHOD(getlinecolor),
    STYLE_METHOD(getlinestyle),
    STYLE_METHOD(getpolyfillmode),
    STYLE_METHOD(getrop2),
    STYLE_METHOD(setbkcolor),
    STYLE_METHOD(setbkmode),
    STYLE_METHOD(setfillcolor),
    STYLE_METHOD(setfillstyle),
    STYLE_METHOD(setlinecolor),
    STYLE_METHOD(setlinestyle),
    STYLE_METHOD(setpolyfillmode),
    STYLE_METHOD(setrop2),
    {NULL, NULL}
};


int CreateStyleObject(lua_State* L) {

    int top = lua_gettop(L);

    BOOL bIsNew;

    int ret = c_comment_metatable(L,
        StyleClassName,
        NULL,
        LUAEASYX_STRUCTTYP_NONETYPE,
        &bIsNew,
        NULL,
        TEXT("Style函数表丢失！"),
        NULL
    );

    int style_index;

    if (ret == COMMENT_CREATOR_SUCCESS_ONE) {
        if (bIsNew) {
            style_index = lua_gettop(L);
            luaL_setfuncs(L, stylemethods, 0);
        }
    } else {
        lua_settop(L, top);
        return 0;
    }

    lua_settop(L, style_index);

    return 1;
}



LUAEASYX_EXPORT int luaopen_Style(lua_State* L) {
    return CreateStyleObject(L);
}





