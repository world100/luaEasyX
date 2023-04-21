
#include "Text.h"
#include <cstddef>
#include <cstring>

/*
void outtext(LPCTSTR str);
*/
//�ڵ�ǰλ������ַ���
int Text_outtext(lua_State* L) {
    LPCTSTR lpStr = (LPCTSTR)luaL_checkstring(L, 1);

    ::outtext(lpStr);

    return 0;
}


/*
void outtextxy(
    int x,
    int y,
    LPCTSTR str
);
*/
//��ָ��λ������ַ���
int Text_outtextxy(lua_State* L) {
    int x = (int)luaL_checkinteger(L, 1);
    int y = (int)luaL_checkinteger(L, 2);
    LPCTSTR lpStr = (LPCTSTR)luaL_checkstring(L, 3);

    ::outtextxy(x, y, lpStr);

    return 0;
}


/*
void settextcolor(COLORREF color);
*/
//���õ�ǰ������ɫ
int Text_settextcolor(lua_State* L) {
    COLORREF color = (COLORREF)luaL_checkinteger(L, 1);

    ::settextcolor(color);

    return 0;
}


/*
COLORREF gettextcolor();
*/
//��ȡ��ǰ������ɫ
int Text_gettextcolor(lua_State* L) {
    COLORREF color = ::gettextcolor();
    lua_pushinteger(L, (lua_Integer)(color));
    return 1;
}


/*
int textwidth(LPCTSTR str);
int textheight(LPCTSTR str);
*/
//��ȡ�ַ���ʵ��ռ�õ����ظ߶�
//����1���ַ���
//����ֵ1������
//����ֵ2���߶�
int Text_textsize(lua_State* L) {
    LPCTSTR str = (LPCTSTR)luaL_checkstring(L, 1);

    int fw = ::textwidth(str);
    int fh = ::textheight(str);

    lua_pushinteger(L, (lua_Integer)(fw));
    lua_pushinteger(L, (lua_Integer)(fh));

    return 2;
}


#define TEXT_TYPE_LONG  0
#define TEXT_TYPE_BYTE  2
#define TEXT_TYPE_TCHAR 4

template <typename T>
void c_settablebykey(lua_State* L, int tbindex, TCHAR* keyname, T& value, UINT vtype) {

    lua_pushstring(L, keyname);

    switch (vtype) {
    case TEXT_TYPE_LONG:
    case TEXT_TYPE_BYTE:
        lua_pushinteger(L, (lua_Integer)(value));
        break;
    case TEXT_TYPE_TCHAR:
        lua_pushstring(L, (const char*)(value));
        break;
    default: break;
    }

    lua_rawset(L, tbindex);
}

#define settablebykey(L, tbindex, o, name, t) \
    c_settablebykey(L, tbindex, #name, o.##name, t)

/*
void gettextstyle(LOGFONT *font);
*/
//��ȡ��ǰ������ʽ
//������LOGFANOT�����ֶ�һ�����ı���ֵ
int Text_gettextstyle(lua_State* L) {
    LOGFONT font;

    ::gettextstyle(&font);

    //����һ����������14��LOGFONT�ֶ���ֵ�ı�
    //���صı�����������������Ԫ��
    lua_createtable(L, 14, 0);

    int tbindex = lua_gettop(L);

    //LONG
    settablebykey(L, tbindex, font, lfHeight, TEXT_TYPE_LONG);
    settablebykey(L, tbindex, font, lfWidth , TEXT_TYPE_LONG);
    settablebykey(L, tbindex, font, lfEscapement, TEXT_TYPE_LONG);
    settablebykey(L, tbindex, font, lfOrientation, TEXT_TYPE_LONG);
    settablebykey(L, tbindex, font, lfWeight, TEXT_TYPE_LONG);

    //BYTE
    settablebykey(L, tbindex, font, lfItalic, TEXT_TYPE_BYTE);
    settablebykey(L, tbindex, font, lfUnderline, TEXT_TYPE_BYTE);
    settablebykey(L, tbindex, font, lfStrikeOut, TEXT_TYPE_BYTE);
    settablebykey(L, tbindex, font, lfCharSet, TEXT_TYPE_BYTE);
    settablebykey(L, tbindex, font, lfOutPrecision, TEXT_TYPE_BYTE);
    settablebykey(L, tbindex, font, lfClipPrecision, TEXT_TYPE_BYTE);
    settablebykey(L, tbindex, font, lfQuality, TEXT_TYPE_BYTE);
    settablebykey(L, tbindex, font, lfPitchAndFamily, TEXT_TYPE_BYTE);

    //TCHAR
    settablebykey(L, tbindex, font, lfFaceName, TEXT_TYPE_TCHAR);

    lua_settop(L, tbindex);
    lua_pushvalue(L, tbindex);

    return 1;
}


/*
void settextstyle(
    int nHeight,
    int nWidth,
    LPCTSTR lpszFace
);

void settextstyle(
    int nHeight,
    int nWidth,
    LPCTSTR lpszFace,
    int nEscapement,
    int nOrientation,
    int nWeight,
    bool bItalic,
    bool bUnderline,
    bool bStrikeOut
);

void settextstyle(
    int nHeight,
    int nWidth,
    LPCTSTR lpszFace,
    int nEscapement,
    int nOrientation,
    int nWeight,
    bool bItalic,
    bool bUnderline,
    bool bStrikeOut,
    BYTE fbCharSet,
    BYTE fbOutPrecision,
    BYTE fbClipPrecision,
    BYTE fbQuality,
    BYTE fbPitchAndFamily
);

void settextstyle(const LOGFONT *font);
*/
//���õ�ǰ������ʽ
//ֻ�ṩ��һ�����صķ�װ
int Text_settextstyle(lua_State* L) {

    int nHeight = (int)luaL_checkinteger(L, 1);
    int nWidth = (int)luaL_checkinteger(L, 2);
    LPCTSTR lpszFace = (LPCTSTR)luaL_checkstring(L, 3);

    ::settextstyle(nHeight, nWidth, lpszFace);

    return 0;
}


#define TEXT_METHOD(name) {#name, Text_##name}

const struct luaL_Reg textmethods[] = {
    TEXT_METHOD(outtext),
    TEXT_METHOD(outtextxy),
    TEXT_METHOD(settextcolor),
    TEXT_METHOD(textsize),
    TEXT_METHOD(gettextcolor),
    TEXT_METHOD(gettextstyle),
    TEXT_METHOD(settextstyle),
    {NULL, NULL}
};


TCHAR* TextClassName = TEXT("LuaEasyX_Text");

int Text_CreateText(lua_State* L) {

    int top = lua_gettop(L);

    BOOL bIsNew;

    int ret = c_comment_metatable(L,
        TextClassName,
        NULL,
        LUAEASYX_STRUCTTYP_NONETYPE,
        &bIsNew,
        NULL,
        TEXT("Text��������ʧ��"),
        NULL
    );

    int textindex;
    if (ret == COMMENT_CREATOR_SUCCESS_ONE) {
        if (bIsNew) {
            textindex = lua_gettop(L);

            luaL_setfuncs(L, textmethods, 0);
        }
    } else {
        lua_settop(L, top);
        return 0;
    }

    return 1;
}


LUAEASYX_EXPORT int luaopen_Text(lua_State* L) {
    return Text_CreateText(L);
}




