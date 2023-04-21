#include "LuaEasyX.h"

/*
author: lovingxiaobing | С��Ӵ
*/

/*******************************************
* LuaEasyX �Ƕ� LuaEasy �� ���еĶ��η�װ *
* EasyX�汾:20180727(beta)                 *
* Lua C/API �汾:5.3.5                     *
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
    // ֹͣ����
    PlaySound(NULL, NULL, 0);
    // ���ٶ�ʱ��
    KillTimer(hwnd, ID_TIMER);
}

int l_playsound(lua_State* L) {
    if (!luaL_checkstring(L, -1)) {
        return 0;
    }
    const char* str_name = lua_tostring(L, -1);
    int time = 1000;//����ʱ��Ĭ��1��
    if (lua_isinteger(L, -2)) {
        time = (int)lua_tonumber(L, -2);
    }
    // �����������첽
    PlaySound(TEXT(str_name), NULL, SND_FILENAME | SND_ASYNC);
    // ���ö�ʱ��
    SetTimer(NULL, ID_TIMER, time, TimerProc);
    ID_TIMER = ID_TIMER + 1;
    return 0;
}



// ����PNGͼ��ȥ͸������
int l_drawAlpha(lua_State* L) //xΪ����ͼƬ��X���꣬yΪY����
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
    // ������ʼ��
    DWORD* dst = GetImageBuffer();    // GetImageBuffer()���������ڻ�ȡ��ͼ�豸���Դ�ָ�룬EASYX�Դ�
    DWORD* draw = GetImageBuffer();
    DWORD* src = GetImageBuffer(picture); //��ȡpicture���Դ�ָ��
    int picture_width = picture->getwidth(); //��ȡpicture�Ŀ�ȣ�EASYX�Դ�
    int picture_height = picture->getheight(); //��ȡpicture�ĸ߶ȣ�EASYX�Դ�
    int graphWidth = getwidth();       //��ȡ��ͼ���Ŀ�ȣ�EASYX�Դ�
    int graphHeight = getheight();     //��ȡ��ͼ���ĸ߶ȣ�EASYX�Դ�
    int dstX = 0;    //���Դ������صĽǱ�

    // ʵ��͸����ͼ ��ʽ�� Cp=��p*FP+(1-��p)*BP �� ��Ҷ˹���������е���ɫ�ĸ��ʼ���
    for (int iy = 0; iy < picture_height; iy++)
    {
        for (int ix = 0; ix < picture_width; ix++)
        {
            int srcX = ix + iy * picture_width; //���Դ������صĽǱ�
            int sa = ((src[srcX] & 0xff000000) >> 24); //0xAArrggbb;AA��͸����
            int sr = ((src[srcX] & 0xff0000) >> 16); //��ȡRGB���R
            int sg = ((src[srcX] & 0xff00) >> 8);   //G
            int sb = src[srcX] & 0xff;              //B
            if (ix >= 0 && ix <= graphWidth && iy >= 0 && iy <= graphHeight && dstX <= graphWidth * graphHeight)
            {
                dstX = (ix + picture_x) + (iy + picture_y) * graphWidth; //���Դ������صĽǱ�
                int dr = ((dst[dstX] & 0xff0000) >> 16);
                int dg = ((dst[dstX] & 0xff00) >> 8);
                int db = dst[dstX] & 0xff;
                draw[dstX] = ((sr * sa / 255 + dr * (255 - sa) / 255) << 16)  //��ʽ�� Cp=��p*FP+(1-��p)*BP  �� ��p=sa/255 , FP=sr , BP=dr
                    | ((sg * sa / 255 + dg * (255 - sa) / 255) << 8)         //��p=sa/255 , FP=sg , BP=dg
                    | (sb * sa / 255 + db * (255 - sa) / 255);              //��p=sa/255 , FP=sb , BP=db
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
        TEXT("LuaEasyX�����ʧ��"),
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
            lua_pushstring(L, TEXT("�� lovingxiaobing|С��Ӵ �� EasyX ��װ����лEasyX�������ṩ��EasyXͼ�ο⣡"));
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
