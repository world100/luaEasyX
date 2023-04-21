#include "LuaEasyX.h"

/*
author: lovingxiaobing | 小冰哟
*/

/*******************************************
* LuaEasyX 是对 LuaEasy 库 进行的二次封装 *
* EasyX版本:20180727(beta)                 *
* Lua C/API 版本:5.3.5                     *
********************************************
*/

int l_sleep(lua_State* L) {
    DWORD dwMilliSeconds = (DWORD)lua_tointeger(L, 1);
    Sleep(dwMilliSeconds);
    return 0;
}

int l_gettime(lua_State* L) {
    double t = timeGetTime();
    lua_pushnumber(L, t);
    return 1;
}

//int l_playsound(lua_State* L) {
//    if( !luaL_checkstring(L, -1) || !luaL_checkstring(L, -2)){
//        return 0;
//    }
//    const char* str_name = lua_tostring(L, -1);
//    const char* str_path = lua_tostring(L, -2);
//    char cmd[100];
//    sprintf(cmd, "close %s", str_name);
//    mciSendString(_T(cmd), 0, 0, 0);
//    sprintf(cmd, "open %s  alias %s", str_path, str_name);
//    mciSendString(_T(cmd), 0, 0, 0);
//    sprintf(cmd, "play %s", str_name);
//    mciSendString(_T(cmd), 0, 0, 0);
//    return 0;
//}


long ID_TIMER = 1;

void CALLBACK TimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime) {
    // 停止声音
    PlaySound(NULL, NULL, 0);
    // 销毁定时器
    KillTimer(hwnd, ID_TIMER);
}

int l_playsound(lua_State* L) {
    if (!luaL_checkstring(L, -1)) {
        return 0;
    }
    const char* str_name = lua_tostring(L, -1);
    int time = 1000;//播放时间默认1秒
    if (lua_isinteger(L, -2)) {
        time = (int)lua_tonumber(L, -2);
    }
    // 播放声音（异步
    PlaySound(TEXT(str_name), NULL, SND_FILENAME | SND_ASYNC);
    // 设置定时器
    SetTimer(NULL, ID_TIMER, time, TimerProc);
    ID_TIMER = ID_TIMER + 1;
    return 0;
}



// 载入PNG图并去透明部分
int l_drawAlpha(lua_State* L) //x为载入图片的X坐标，y为Y坐标
{
    IMAGE* picture = (IMAGE*)lua_touserdata(L, -1);
    int  picture_x = 0;
    int picture_y = 0;

    printf("x%d \n", lua_tointeger(L, -2));
    printf("y%d \n", lua_tointeger(L, -3));
    if(lua_isinteger(L, -2)){
        picture_x = lua_tointeger(L, -2);
        //printf("x%d \n", picture_x);
    }
    if (lua_isinteger(L, -3)) {
        picture_y = lua_tointeger(L, -3);
       // printf("y%d \n", picture_y);
    }
    // 变量初始化
    DWORD* dst = GetImageBuffer();    // GetImageBuffer()函数，用于获取绘图设备的显存指针，EASYX自带
    DWORD* draw = GetImageBuffer();
    DWORD* src = GetImageBuffer(picture); //获取picture的显存指针
    int picture_width = picture->getwidth(); //获取picture的宽度，EASYX自带
    int picture_height = picture->getheight(); //获取picture的高度，EASYX自带
    int graphWidth = getwidth();       //获取绘图区的宽度，EASYX自带
    int graphHeight = getheight();     //获取绘图区的高度，EASYX自带
    int dstX = 0;    //在显存里像素的角标

    // 实现透明贴图 公式： Cp=αp*FP+(1-αp)*BP ， 贝叶斯定理来进行点颜色的概率计算
    for (int iy = 0; iy < picture_height; iy++)
    {
        for (int ix = 0; ix < picture_width; ix++)
        {
            int srcX = ix + iy * picture_width; //在显存里像素的角标
            int sa = ((src[srcX] & 0xff000000) >> 24); //0xAArrggbb;AA是透明度
            int sr = ((src[srcX] & 0xff0000) >> 16); //获取RGB里的R
            int sg = ((src[srcX] & 0xff00) >> 8);   //G
            int sb = src[srcX] & 0xff;              //B
            if (ix >= 0 && ix <= graphWidth && iy >= 0 && iy <= graphHeight && dstX <= graphWidth * graphHeight)
            {
                dstX = (ix + picture_x) + (iy + picture_y) * graphWidth; //在显存里像素的角标
                int dr = ((dst[dstX] & 0xff0000) >> 16);
                int dg = ((dst[dstX] & 0xff00) >> 8);
                int db = dst[dstX] & 0xff;
                draw[dstX] = ((sr * sa / 255 + dr * (255 - sa) / 255) << 16)  //公式： Cp=αp*FP+(1-αp)*BP  ； αp=sa/255 , FP=sr , BP=dr
                    | ((sg * sa / 255 + dg * (255 - sa) / 255) << 8)         //αp=sa/255 , FP=sg , BP=dg
                    | (sb * sa / 255 + db * (255 - sa) / 255);              //αp=sa/255 , FP=sb , BP=db
            }
        }
    }
    return 0;
}






TCHAR* LuaEasyXClassName = TEXT("LuaEasyX_Happy");

int CreateLuaEasyXTable(lua_State* L) {

    int top = lua_gettop(L);

    BOOL bIsNew;

    int ret = c_comment_metatable(L,
        LuaEasyXClassName,
        NULL,
        LUAEASYX_STRUCTTYP_NONETYPE,
        &bIsNew,
        NULL,
        TEXT("LuaEasyX对象表丢失！"),
        NULL
    );

    int lexindex = 0;

    if (ret == COMMENT_CREATOR_SUCCESS_ONE) {

        if (bIsNew) {

            lexindex = lua_gettop(L);

            l_setobj(L, luaopen_DrawDevice, "DrawDevice", lexindex);
            l_setobj(L, luaopen_Graphics, "Graphics", lexindex);
            l_setobj(L, luaopen_Color, "Color", lexindex);
            l_setobj(L, luaopen_Option, "Option", lexindex);
            l_setobj(L, luaopen_Extend, "Extend", lexindex);
            l_setobj(L, luaopen_Event, "Event", lexindex);
            l_setobj(L, luaopen_Style, "Style", lexindex);
            l_setobj(L, luaopen_Constant, "Constant", lexindex);
            l_setobj(L, luaopen_Image, "Image", lexindex);
            l_setobj(L, luaopen_WinGUI, "WinGUI", lexindex);
            l_setobj(L, luaopen_Text, "Text", lexindex);

            //Sleep
            lua_pushliteral(L, "sleep");
            lua_pushcfunction(L, l_sleep);
            lua_settable(L, lexindex);

            lua_pushliteral(L, "gettime");
            lua_pushcfunction(L, l_gettime);
            lua_settable(L, lexindex);

            lua_pushliteral(L, "playsound");
            lua_pushcfunction(L, l_playsound);
            lua_settable(L, lexindex);

            lua_pushliteral(L, "drawAlpha");
            lua_pushcfunction(L, l_drawAlpha);
            lua_settable(L, lexindex);

            lua_pushliteral(L, "GetObjectAddress");
            lua_pushcfunction(L, GetObjectAddress);
            lua_settable(L, lexindex);

            lua_pushstring(L, "__author__");
            lua_pushstring(L, TEXT("由 lovingxiaobing|小冰哟 对 EasyX 封装，感谢EasyX开发者提供的EasyX图形库！"));
            lua_settable(L, lexindex);
        }
    } else {
        lua_settop(L, top);
        return 0;
    }


    return 1;
}



LUAEASYX_EXPORT int luaopen_LuaEasyX(lua_State* L) {
    return CreateLuaEasyXTable(L);
}
