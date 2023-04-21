
#include "Option.h"

//��ѡ����


/*
void BeginBatchDraw();
*/
//��ʼ������ͼ
int Option_BeginBatchDraw(lua_State* L) {
    ::BeginBatchDraw();
    return 0;
}


/*
// �����������ƣ���ִ��δ��ɵĻ�������
void EndBatchDraw();

// �����������ƣ���ִ��ָ��������δ��ɵĻ�������
void EndBatchDraw(
    int left,
    int top,
    int right,
    int bottom
);
*/
//�����������ƣ���ִ��δ��ɵĻ�������
int Option_EndBatchDraw(lua_State* L) {
    switch (lua_gettop(L)) {
    case 0:
        ::EndBatchDraw();
        return 0;
    case 4:
        ::EndBatchDraw(
            (int)lua_tointeger(L, 1),
            (int)lua_tointeger(L, 2),
            (int)lua_tointeger(L, 3),
            (int)lua_tointeger(L, 4)
        );
        return 0;
    default:
        luaL_error(L, TEXT("��������������ĵ��÷�ʽ��"));
    }

    return 0;
}


/*
// ִ��δ��ɵĻ�������
void FlushBatchDraw();

// ִ��ָ��������δ��ɵĻ�������
void FlushBatchDraw(
    int left,
    int top,
    int right,
    int bottom
);
*/
//ִ��δ��ɵĻ�������
int Option_FlushBatchDraw(lua_State* L) {
    switch (lua_gettop(L)) {
    case 0:
        ::FlushBatchDraw();
        return 0;
    case 4:
        ::FlushBatchDraw(
            (int)lua_tointeger(L, 1),
            (int)lua_tointeger(L, 2),
            (int)lua_tointeger(L, 3),
            (int)lua_tointeger(L, 4)
        );
        return 0;
    default:
        luaL_error(L, TEXT("��������������ĵ��÷�ʽ��"));
    }

    return 0;
}


/*
TCHAR* GetEasyXVer();
*/
//��ȡ��ǰ EasyX ��İ汾��Ϣ
int Option_GetEasyXVer(lua_State* L) {
    lua_pushstring(L, ::GetEasyXVer());
    return 1;
}


/*
HWND GetHWnd();
*/
//��ȡ��ͼ���ھ��
int Option_GetHWnd(lua_State* L) {
    HWND hwnd = ::GetHWnd();
    lua_pushlightuserdata(L, (void*)(hwnd));
    return 1;
}


/*
bool InputBox(
	LPTSTR	pString,
	int	nMaxCount,
	LPCTSTR	pPrompt = NULL,
	LPCTSTR	pTitle = NULL,
	LPCTSTR	pDefault = NULL,
	int	width = 0,
	int	height = 0,
	bool	bHideCancelBtn = true
);
*/
//����1�������û������ַ�������
////------  �������Ĭ�ϲ���  -----
//����2��ָ����ʾ�ڴ����е���ʾ��Ϣ
//����3��ָ��InputBox �ı�����(��������ַ������ͣ���Ĭ�ϳ�������)
//����4��ָ����ʾ���û���������Ĭ��ֵ
//����5����
//����6����
//����7��ָ���Ƿ������û�ȡ������
//����ֵ1��������ַ���
//����ֵ2��InputBox����ֵ
//�ԶԻ�����ʽ��ȡ�û�����
int Option_InputBox(lua_State* L) {
	LPTSTR	pString;
	int	nMaxCount;

	LPCTSTR	pPrompt  = NULL;
	LPCTSTR	pTitle   = NULL;
	LPCTSTR	pDefault = NULL;
	int	width  = 0;
	int	height = 0;
	bool	bHideCancelBtn = true;

	bool ret;

    switch(lua_gettop(L)) {
    case 1:
        nMaxCount = (int)luaL_checkinteger(L, 1);
        pString = new TCHAR[nMaxCount+2];
        ret = ::InputBox(pString, nMaxCount);
        break;
    case 7:
        nMaxCount = (int)luaL_checkinteger(L, 1);

        if (lua_type(L, 2) == LUA_TSTRING)
            pPrompt = lua_tostring(L, 2);
        if (lua_type(L, 3) == LUA_TSTRING)
            pTitle  = lua_tostring(L, 3);
        if (lua_type(L, 4) == LUA_TSTRING)
            pDefault = lua_tostring(L, 4);

        width = (int)luaL_checkinteger(L, 5);
        height = (int)luaL_checkinteger(L, 6);
        bHideCancelBtn = l_lua_checkboolean(L, 7);

        pString = new TCHAR[nMaxCount+2];

        ret = ::InputBox(
            pString, nMaxCount,
            pPrompt, pTitle, pDefault,
            width, height,
            bHideCancelBtn
        );
        break;
    default:
        luaL_error(L, TEXT("��������������ĵ��÷�ʽ��"));
        return 0;
    }

    lua_pushstring(L, pString);
    lua_pushboolean(L, (int)(ret));

    delete[] pString;

    return 2;
}


TCHAR* OptionClassName = TEXT("LuaEasyX_Option");

#define OPTION_METHOD(name) {#name, Option_##name}

const struct luaL_Reg optionmethods[] = {
    OPTION_METHOD(BeginBatchDraw),
    OPTION_METHOD(EndBatchDraw),
    OPTION_METHOD(FlushBatchDraw),
    OPTION_METHOD(GetEasyXVer),
    OPTION_METHOD(GetHWnd),
    OPTION_METHOD(InputBox),
    {NULL, NULL}
};


int CreateOption(lua_State* L) {

    int top = lua_gettop(L);

    BOOL bIsNew;

    int ret = c_comment_metatable(L,
        OptionClassName,
        NULL,
        LUAEASYX_STRUCTTYP_NONETYPE,
        &bIsNew,
        NULL,
        TEXT("Option������ʧ��"),
        NULL
    );

    int option_methods_index;

    if (ret == COMMENT_CREATOR_SUCCESS_ONE) {
        if (bIsNew) {
            option_methods_index = lua_gettop(L);
            luaL_setfuncs(L, (luaL_Reg*)optionmethods, 0);
        }
    } else {
        lua_settop(L, top);
        return 0;
    }

    return 1;
}

LUAEASYX_EXPORT int luaopen_Option(lua_State* L) {
    return CreateOption(L);
}


