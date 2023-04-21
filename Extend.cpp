
#include "Extend.h"

//扩展函数，新增函数


/*
void bar(
    int left,
    int top,
    int right,
    int bottom
);
*/
//画填充矩形（无边框）
int Extend_bar(lua_State* L) {
    int left    = (int)lua_tointeger(L, 1);
    int top     = (int)lua_tointeger(L, 2);
    int right   = (int)lua_tointeger(L, 3);
    int bottom  = (int)lua_tointeger(L, 4);

    ::bar(left, top, right, bottom);

    return 0;
}


/*
void bar3d(
    int left,
    int top,
    int right,
    int bottom,
    int depth,
    bool topflag
);
*/
//画有边框三维填充矩形
int Extend_bar3d(lua_State* L) {
    int left    = (int)lua_tointeger(L, 1);
    int top     = (int)lua_tointeger(L, 2);
    int right   = (int)lua_tointeger(L, 3);
    int bottom  = (int)lua_tointeger(L, 4);
    int depth   = (int)lua_tointeger(L, 5);
    bool topflag = lua_toboolean(L, 6) ? true : false;

    ::bar3d(left, top, right, bottom , depth, topflag);

    return 0;
}


/*
void drawpoly(
	int numpoints,
	const int *polypoints
);
*/
//画多边形
int Extend_drawpoly(lua_State* L) {
    int numpoints = (int)luaL_checkinteger(L, 1);
    const int* polypoints;
    if (lua_type(L, 2) == LUA_TLIGHTUSERDATA) {
        polypoints = (const int*)lua_touserdata(L, 2);
    } else {
        luaL_error(L, TEXT("请传入POINT数组地址(lightuserdata)"));
        return 0;
    }

    ::drawpoly(numpoints*2, polypoints);

    return 0;
}


/*
void fillpoly(
	int numpoints,
	const int *polypoints
);
*/
//画填充多边形（有边框）
int Extend_fillpoly(lua_State* L) {
    int numpoints = (int)luaL_checkinteger(L, 1);
    const int* polypoints;
    if (lua_type(L, 2) == LUA_TLIGHTUSERDATA) {
        polypoints = (const int*)lua_touserdata(L, 2);
    } else {
        luaL_error(L, TEXT("请传入POINT数组地址(lightuserdata)"));
        return 0;
    }

    ::fillpoly(numpoints*2, polypoints);

    return 0;
}


/*
COLORREF getcolor();
*/
//获取当前绘图前景色
int Extend_getcolor(lua_State* L) {
    lua_pushinteger(L, (COLORREF)(::getcolor()));
    return 1;
}


/*
int getmaxx();
int getmaxy();
*/
//返回值1：x
//返回值2：y
//获取绘图窗口的物理坐标中的最大 x 坐标 和 y 坐标
int Extend_getmaxxy(lua_State* L) {
    lua_pushinteger(L, (int)(::getmaxx()));
    lua_pushinteger(L, (int)(::getmaxy()));

    return 2;
}


/*
void setcolor(COLORREF color);
*/
//设置当前绘图前景色
int Extend_setcolor(lua_State* L) {
    ::setcolor((COLORREF)luaL_checkinteger(L, 1));
    return 0;
}


/*
void setwritemode(int mode);
*/
//设置前景的二元光栅操作模式
int Extend_setwritemode(lua_State* L) {
    ::setwritemode((int)luaL_checkinteger(L, 1));
    return 0;
}


TCHAR* ExtendClassName = TEXT("LuaEasyX_Extend");

#define EXTEND_METHOD(name) {#name, Extend_##name}

const struct luaL_Reg extend_methods[] = {
    EXTEND_METHOD(bar),
    EXTEND_METHOD(bar3d),
    EXTEND_METHOD(drawpoly),
    EXTEND_METHOD(fillpoly),
    EXTEND_METHOD(getcolor),
    EXTEND_METHOD(getmaxxy),
    EXTEND_METHOD(setcolor),
    EXTEND_METHOD(setwritemode),
    {NULL, NULL}
};

int CreateExtend(lua_State* L) {

    int top = lua_gettop(L);

    BOOL bIsNew;

    int ret = c_comment_metatable(L,
        TEXT("Extend函数表丢失！"),
        NULL,
        LUAEASYX_STRUCTTYP_NONETYPE,
        &bIsNew,
        NULL,
        TEXT("Extend函数表丢失！"),
        NULL
    );

    int extend_methods_index;

    if (ret == COMMENT_CREATOR_SUCCESS_ONE) {
        if (bIsNew) {
            extend_methods_index = lua_gettop(L);
            luaL_setfuncs(L, (luaL_Reg*)extend_methods, 0);
        }
    } else {
        lua_settop(L, top);
        return 0;
    }

    return 1;
}

LUAEASYX_EXPORT int luaopen_Extend(lua_State* L) {
    return CreateExtend(L);
}


