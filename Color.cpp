#include "Color.h"



/*
COLORREF RGB(
    BYTE byRed,     // 颜色的红色部分
    BYTE byGreen,   // 颜色的绿色部分
    BYTE byBlue     // 颜色的蓝色部分
);
*/
//通过红、绿、蓝颜色分量合成颜色
int Color_RGB(lua_State* L) {
    BYTE byRed   = (BYTE)luaL_checkinteger(L, 1);
    BYTE byGreen = (BYTE)luaL_checkinteger(L, 2);
    BYTE byBlue  = (BYTE)luaL_checkinteger(L, 3);

    lua_pushinteger(L, (lua_Integer)(RGB(byRed, byGreen, byBlue)));

    return 1;
}


/*
COLORREF BGR(COLORREF color);
*/
//交换颜色中的红色和蓝色
int Color_BGR(lua_State* L) {
    COLORREF color = (COLORREF)luaL_checkinteger(L, 1);

    lua_pushinteger(L, (lua_Integer)(BGR(color)));

    return 1;
}


/*
BYTE GetRValue(COLORREF rgb);
*/
//返回指定颜色中的红色值
int Color_GetRValue(lua_State* L) {
    COLORREF color = (COLORREF)luaL_checkinteger(L, 1);

    lua_pushinteger(L, (lua_Integer)(GetRValue(color)));

    return 1;
}


/*
BYTE GetGValue(COLORREF rgb);
*/
//返回指定颜色中的绿色值
int Color_GetGValue(lua_State* L) {
    COLORREF color = (COLORREF)luaL_checkinteger(L, 1);

    lua_pushinteger(L, (lua_Integer)(GetGValue(color)));

    return 1;
}


/*
BYTE GetBValue(COLORREF rgb);
*/
//返回指定颜色中的蓝色值
int Color_GetBValue(lua_State* L) {
    COLORREF color = (COLORREF)luaL_checkinteger(L, 1);

    lua_pushinteger(L, (lua_Integer)(GetBValue(color)));

    return 1;
}


/*
COLORREF HSLtoRGB(
    float H,
    float S,
    float L
);
*/
//转换 HSL 颜色为 RGB 颜色
int Color_HSLtoRGB(lua_State* L) {
    float H = (float)luaL_checknumber(L, 1);
    float S = (float)luaL_checknumber(L, 2);
    float _L = (float)luaL_checknumber(L, 3);

    COLORREF color = HSLtoRGB(H, S, _L);

    lua_pushinteger(L, (lua_Integer)(color));

    return 1;
}


/*
COLORREF HSVtoRGB(
    float H,
    float S,
    float V
);
*/
//转换 HSV 颜色为 RGB 颜色
int Color_HSVtoRGB(lua_State* L) {
    float H = (float)luaL_checkinteger(L, 1);
    float S = (float)luaL_checkinteger(L, 2);
    float V = (float)luaL_checkinteger(L, 3);

    COLORREF color = ::HSVtoRGB(H, S, V);

    lua_pushinteger(L, (lua_Integer)(color));

    return 1;
}


/*
COLORREF RGBtoGRAY(
    COLORREF rgb
);
*/
//转换 RGB 颜色为 灰度颜色
int Color_RGBtoGRAY(lua_State* L) {
    COLORREF color = (COLORREF)luaL_checkinteger(L, 1);

    lua_pushinteger(L, (lua_Integer)(::RGBtoGRAY(color)));

    return 1;
}


/*
void RGBtoHSL(
    COLORREF rgb,
    float *H,
    float *S,
    float *L
);
*/
//转换 RGB 颜色为 HSL 颜色
//参数1：rgb颜色值
//返回值1：H
//返回值2：S
//返回值3：L
int Color_RGBtoHSL(lua_State* L) {
    float H, S, _L;

    COLORREF color = (COLORREF)luaL_checkinteger(L, 1);

    ::RGBtoHSL(color, &H, &S, &_L);

    lua_pushnumber(L, (lua_Number)(H));
    lua_pushnumber(L, (lua_Number)(S));
    lua_pushnumber(L, (lua_Number)(_L));

    return 3;
}


/*
void RGBtoHSV(
    COLORREF rgb,
    float *H,
    float *S,
    float *V
);
*/
//转换 RGB 颜色为 HSV 颜色
//参数1：rgb色值
//返回值1：H
//返回值2：S
//返回值3：V
int Color_RGBtoHSV(lua_State* L) {
    float H, S, V;

    COLORREF color = (COLORREF)luaL_checkinteger(L, 1);

    ::RGBtoHSV(color, &H, &S, &V);

    lua_pushnumber(L, (lua_Number)(H));
    lua_pushnumber(L, (lua_Number)(S));
    lua_pushnumber(L, (lua_Number)(V));

    return 3;
}

//返回值1：R
//返回值2：G
//返回值3：B
int Color_ExplainRGB(lua_State* L) {
    COLORREF RGBcolor = (COLORREF)luaL_checkinteger(L, 1);
    //R
    lua_pushinteger(L, (lua_Integer)(GetRValue(RGBcolor)));
    //G
    lua_pushinteger(L, (lua_Integer)(GetGValue(RGBcolor)));
    //B
    lua_pushinteger(L, (lua_Integer)(GetBValue(RGBcolor)));

    return 3;
}


TCHAR* ColorClassName = TEXT("LuaEasyX_Color");

#define COLOR_METHOD(name) {#name, Color_##name}

const struct luaL_Reg clrmethodtable[] = {
    COLOR_METHOD(RGB),
    COLOR_METHOD(BGR),
    COLOR_METHOD(GetRValue),
    COLOR_METHOD(GetGValue),
    COLOR_METHOD(GetBValue),
    COLOR_METHOD(HSLtoRGB),
    COLOR_METHOD(HSVtoRGB),
    COLOR_METHOD(RGBtoGRAY),
    COLOR_METHOD(RGBtoHSL),
    COLOR_METHOD(RGBtoHSV),
    COLOR_METHOD(ExplainRGB),
    {NULL, NULL}
};



int CreateColor(lua_State* L) {

    int top = lua_gettop(L);

    BOOL bIsNew;

    int ret = c_comment_metatable(
        L,
        ColorClassName,
        NULL,
        LUAEASYX_STRUCTTYP_NONETYPE,
        &bIsNew,
        NULL,
        TEXT("Color函数表丢失！"),
        NULL
    );

    int color_methods_index;
    //Color函数表
    if (ret == COMMENT_CREATOR_SUCCESS_ONE) {

        if (bIsNew) {

            color_methods_index = lua_gettop(L);

            luaL_setfuncs(L, (luaL_Reg*)clrmethodtable, 0);

            //为__call绑定生成RGB色函数
            lua_pushliteral(L, "__call");
            lua_pushcfunction(L, Color_RGB);
            lua_settable(L, color_methods_index);

            lua_pushvalue(L, color_methods_index);
            lua_setmetatable(L, color_methods_index);
        }
    } else {
        lua_settop(L, top);
        return 0;
    }

    return 1;
}


LUAEASYX_EXPORT int luaopen_Color(lua_State* L) {

    return CreateColor(L);
}


