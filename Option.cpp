
#include "Option.h"

//可选函数


/*
void BeginBatchDraw();
*/
//开始批量绘图
int Option_BeginBatchDraw(lua_State* L) {
    ::BeginBatchDraw();
    return 0;
}


/*
// 结束批量绘制，并执行未完成的绘制任务
void EndBatchDraw();

// 结束批量绘制，并执行指定区域内未完成的绘制任务
void EndBatchDraw(
    int left,
    int top,
    int right,
    int bottom
);
*/
//结束批量绘制，并执行未完成的绘制任务
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
        luaL_error(L, TEXT("参数出错！请检查你的调用方式！"));
    }

    return 0;
}


/*
// 执行未完成的绘制任务
void FlushBatchDraw();

// 执行指定区域内未完成的绘制任务
void FlushBatchDraw(
    int left,
    int top,
    int right,
    int bottom
);
*/
//执行未完成的绘制任务
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
        luaL_error(L, TEXT("参数出错！请检查你的调用方式！"));
    }

    return 0;
}


/*
TCHAR* GetEasyXVer();
*/
//获取当前 EasyX 库的版本信息
int Option_GetEasyXVer(lua_State* L) {
    lua_pushstring(L, ::GetEasyXVer());
    return 1;
}


/*
HWND GetHWnd();
*/
//获取绘图窗口句柄
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
//参数1：限制用户输入字符的数量
////------  下面的是默认参数  -----
//参数2：指定显示在窗体中的提示信息
//参数3：指定InputBox 的标题栏(如果不是字符串类型，则默认程序名称)
//参数4：指定显示在用户输入区的默认值
//参数5：宽
//参数6：高
//参数7：指定是否允许用户取消输入
//返回值1：输入的字符串
//返回值2：InputBox返回值
//以对话框形式获取用户输入
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
        luaL_error(L, TEXT("参数出错！请检查你的调用方式！"));
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
        TEXT("Option函数表丢失！"),
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


