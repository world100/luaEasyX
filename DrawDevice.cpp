#include "DrawDevice.h"


/*
HWND initgraph(
    int width,
    int height,
    int flag = NULL
);
*/
//��ʼ����ͼ����
//����1����
//����2����
//����3: flag
//����ֵ1�����ھ��
//����ֵ2���ɹ�����true,ʧ�ܷ���false
int DrawDevice_initgraph(lua_State* L) {
    int width  = (int)luaL_checkinteger(L, 1);
    int height = (int)luaL_checkinteger(L, 2);

    int flag = NULL;

    if (lua_gettop(L) == 3)
        flag = (int)luaL_checkinteger(L, 3);

    HWND hwnd = ::initgraph(width, height, flag);

    bool ret = hwnd ? true : false;

    lua_pushlightuserdata(L, hwnd);
    lua_pushboolean(L, (int)(ret));

    return 2;
}


/*
void closegraph();
*/
//�ر�ͼ�δ���
int DrawDevice_closegraph(lua_State* L) {
    ::closegraph();
    return 0;
}


/*
void getaspectratio(
    float *pxasp,
    float *pyasp
);
*/
//��ȡ��ǰ��������
//����ֵ1��xasp
//����ֵ2��yasp
int DrawDevice_getaspectratio(lua_State* L) {
    float xasp, yasp;

    ::getaspectratio(&xasp, &yasp);

    //x
    lua_pushnumber(L, (lua_Number)xasp);
    //y
    lua_pushnumber(L, (lua_Number)yasp);

    return 2;
}


/*
void setaspectratio(
    float xasp,
    float yasp
);
*/
//���õ�ǰ��������
//����1��xasp
//����2��yasp
int DrawDevice_setaspectratio(lua_State* L) {
    float xasp = (float)luaL_checknumber(L, 1);
    float yasp = (float)luaL_checknumber(L, 2);

    ::setaspectratio(xasp, yasp);

    return 0;
}


/*
void graphdefaults();
*/
//�ָ���ͼ����ΪĬ��ֵ
int DrawDevice_graphdefaults(lua_State* L) {
    ::graphdefaults();
    return 0;
}


/*
void setorigin(int x, int y);
*/
//��������ԭ��
//����1��x
//����2��y
int DrawDevice_setorigin(lua_State* L) {
    int x = (int)luaL_checkinteger(L, 1);
    int y = (int)luaL_checkinteger(L, 2);

    ::setorigin(x, y);

    return 0;
}


/*
void setcliprgn(HRGN hrgn);
*/
//���õ�ǰ��ͼ�豸�Ĳü���
//����1��hrgn
int DrawDevice_setcliprgn(lua_State* L) {
    if (lua_type(L, 1) == LUA_TLIGHTUSERDATA) {
        HRGN hRgn = (HRGN)lua_touserdata(L, 1);
        ::setcliprgn(hRgn);
    }
    return 0;
}

/*
void clearcliprgn();
*/
//����ü�������Ļ����
int DrawDevice_clearcliprgn(lua_State* L) {
    ::clearcliprgn();
    return 0;
}


/*
void cleardevice();
*/
//�����Ļ���ݡ�����ģ����õ�ǰ����ɫ�����Ļ��������ǰ������ (0, 0)
int DrawDevice_cleardevice(lua_State* L) {
    ::cleardevice();
    return 0;
}



#define DRAWDEVICE_METHOD(name) {#name, DrawDevice_##name}

const struct luaL_Reg ddmethodtable[] = {
    DRAWDEVICE_METHOD(initgraph),
    DRAWDEVICE_METHOD(closegraph),
    DRAWDEVICE_METHOD(getaspectratio),
    DRAWDEVICE_METHOD(setaspectratio),
    DRAWDEVICE_METHOD(graphdefaults),
    DRAWDEVICE_METHOD(setorigin),
    DRAWDEVICE_METHOD(setcliprgn),
    DRAWDEVICE_METHOD(clearcliprgn),
    DRAWDEVICE_METHOD(cleardevice),
    {NULL, NULL}
};

TCHAR* DrawDeviceClassName = TEXT("LuaEasyX_DrawDevice");

int CreateDrawDevice(lua_State* L) {

    int top = lua_gettop(L);

    BOOL bIsNew;

    int ret = c_comment_metatable(L,
        DrawDeviceClassName,
        NULL,
        LUAEASYX_STRUCTTYP_NONETYPE,
        &bIsNew,
        NULL,
        TEXT("DrawDevice������ʧ��"),
        NULL
    );

    int methods_index;

    //DrawDevice������
    if (ret == COMMENT_CREATOR_SUCCESS_ONE) {
        if (bIsNew) {
            methods_index = lua_gettop(L);
            luaL_setfuncs(L, (luaL_Reg*)ddmethodtable, 0);
        }
    } else {
        lua_settop(L, top);
        return 0;
    }


    return 1;
}


LUAEASYX_EXPORT int luaopen_DrawDevice(lua_State* L) {

    return CreateDrawDevice(L);
}


