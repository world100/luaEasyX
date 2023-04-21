
#include "Event.h"
#include "Structure.h"


////////////////////////////////// 鼠标事件结构 ///////////////////////////////
//鼠标消息
int MouseEventStruct_Message(lua_State* L) {
    struct MOUSEMSG* msg = (struct MOUSEMSG*)lua_touserdata(L, 1);
    lua_pushinteger(L, (lua_Integer)(msg->uMsg));
    return 1;
}

//Ctrl是否按下
int MouseEventStruct_isCtrl(lua_State* L) {
    struct MOUSEMSG* msg = (struct MOUSEMSG*)lua_touserdata(L, 1);
    lua_pushboolean(L, (int)(msg->mkCtrl));
    return 1;
}

//Shift是否按下
int MouseEventStruct_isShift(lua_State* L) {
    struct MOUSEMSG* msg = (struct MOUSEMSG*)lua_touserdata(L, 1);
    lua_pushboolean(L, (int)(msg->mkShift));
    return 1;
}

//鼠标左键是否按下
int MouseEventStruct_isLButton(lua_State* L) {
    struct MOUSEMSG* msg = (struct MOUSEMSG*)lua_touserdata(L, 1);
    lua_pushboolean(L, (int)(msg->mkLButton));
    return 1;
}

//鼠标中键是否按下
int MouseEventStruct_isMButton(lua_State* L) {
    struct MOUSEMSG* msg = (struct MOUSEMSG*)lua_touserdata(L, 1);
    lua_pushboolean(L, (int)(msg->mkMButton));
    return 1;
}

//鼠标右键是否按下
int MouseEventStruct_isRButton(lua_State* L) {
    struct MOUSEMSG* msg = (struct MOUSEMSG*)lua_touserdata(L, 1);
    lua_pushboolean(L, (int)(msg->mkRButton));
    return 1;
}

//当前鼠标的x ,y 坐标
//返回值1：x
//返回值2：y
int MouseEventStruct_MousePosition(lua_State* L) {
    struct MOUSEMSG* msg = (struct MOUSEMSG*)lua_touserdata(L, 1);
    lua_pushinteger(L, (lua_Integer)(msg->x));
    lua_pushinteger(L, (lua_Integer)(msg->y));
    return 2;
}


int MouseEventStruct_MouseWheelValue(lua_State* L) {
    struct MOUSEMSG* msg = (struct MOUSEMSG*)lua_touserdata(L, 1);
    lua_pushinteger(L, (lua_Integer)(msg->wheel));
    return 1;
}


/*
MOUSEMSG GetMouseMsg();
*/
//获取一个鼠标消息。如果当前鼠标消息队列中没有，就一直等待
int MouseEventStruct_GetMouseMsg(lua_State* L) {
    struct MOUSEMSG* msg = (struct MOUSEMSG*)lua_touserdata(L, 1);
    *msg = ::GetMouseMsg();
    lua_pushlightuserdata(L, msg);
    return 1;
}


/*
void FlushMouseMsgBuffer();
*/
//清空鼠标消息缓冲区
int MouseEventStruct_FlushMouseMsgBuffer(lua_State* L) {
    ::FlushMouseMsgBuffer();
    return 0;
}


/*
bool MouseHit();
*/
//检测当前是否有鼠标消息
int MouseEventStruct_MouseHit(lua_State* L) {
    lua_pushboolean(L, (int)(::MouseHit()));
    return 1;
}



int MouseEventStruct_Thunk(lua_State* L) {
    lua_CFunction func = (lua_CFunction)lua_touserdata(L, lua_upvalueindex(1));

    if (!func)
        return 0;

    LuaEasyX_StructureType* p = (LuaEasyX_StructureType*)lua_touserdata(L, 1);

    if (p->uEndStructType == p->uStructType && p->uStructType == LUAEASYX_STRUCTTYP_MOUSEMSG) {
        if (p->StructAddress) {
            lua_pushlightuserdata(L, p->StructAddress);
            return func(L);
        }
    }

    return 0;
}


TCHAR* MouseEventClassName = TEXT("LuaEasyX_MouseEvent");
TCHAR* MOUSEEVENTClassName = TEXT("EVENT_MOUSE_EVENT");

#define MOUSEEVENT_METHOD(name) {#name, MouseEventStruct_##name}

const struct luaL_Reg mouseeventmethods[] = {
    {"Message", MouseEventStruct_Message},
    {"isCtrl", MouseEventStruct_isCtrl},
    {"isShift", MouseEventStruct_isShift},
    {"isLButton", MouseEventStruct_isLButton},
    {"isMButton", MouseEventStruct_isMButton},
    {"isRButton", MouseEventStruct_isRButton},
    {"MousePosition", MouseEventStruct_MousePosition},
    {"MouseWheelValue", MouseEventStruct_MouseWheelValue},
    MOUSEEVENT_METHOD(GetMouseMsg),
    MOUSEEVENT_METHOD(FlushMouseMsgBuffer),
    MOUSEEVENT_METHOD(MouseHit),
    {NULL, NULL}
};



int CreateMouseEventStructure(lua_State* L) {

    int top = lua_gettop(L);

    BOOL bIsNew;

    MOUSEMSG* pMMsg = new MOUSEMSG;

    int ret = c_comment_metatable(
        L,
        MOUSEEVENTClassName,
        (PVOID)pMMsg,
        LUAEASYX_STRUCTTYP_MOUSEMSG,
        &bIsNew,
        NULL,
        TEXT("EVENT_MOUSE_EVENT表丢失！"),
        NULL
    );

    if (ret != COMMENT_CREATOR_SUCCESS_TWO) {
        lua_settop(L, top);
        return 0;
    }

    int newtop = lua_gettop(L);

    int udMMsg_index = newtop;
    int ememt_index = newtop - 1;

    if (bIsNew) {

        for (struct luaL_Reg* p = (struct luaL_Reg*)mouseeventmethods;
                p->name && p->func; p++) {

            lua_pushstring(L, p->name);
            lua_pushlightuserdata(L, p->func);
            lua_pushcclosure(L, MouseEventStruct_Thunk, 1);
            lua_settable(L, ememt_index);
        }

        lua_pushliteral(L, "__index");
        lua_pushvalue(L, ememt_index);
        lua_settable(L, ememt_index);
    }

    lua_pushvalue(L, ememt_index);
    lua_setmetatable(L, udMMsg_index);

    lua_settop(L, udMMsg_index);

    return 1;
}

int CreateMouseEvent(lua_State* L) {

    int top = lua_gettop(L);

    BOOL bIsNew;

    int ret = c_comment_metatable(
        L,
        MouseEventClassName,
        NULL,
        LUAEASYX_STRUCTTYP_NONETYPE,
        &bIsNew,
        NULL,
        TEXT("LuaEasyX_MouseEvent函数表丢失！"),
        NULL
    );

    int mouseevnt_methods_index;

    if (ret == COMMENT_CREATOR_SUCCESS_ONE) {

        if (bIsNew) {
            mouseevnt_methods_index = lua_gettop(L);

            luaL_setfuncs(L, (luaL_Reg*)mouseeventmethods, 0);

            lua_pushliteral(L, "__call");
            lua_pushcfunction(L, CreateMouseEventStructure);
            lua_settable(L, mouseevnt_methods_index);

            lua_pushvalue(L, mouseevnt_methods_index);
            //把自己设为元表就可以call了
            lua_setmetatable(L, -1);

        }
    } else {
        lua_settop(L, top);
        return 0;
    }

    return 1;
}


LUAEASYX_EXPORT int luaopen_CreateMouseEvent(lua_State* L) {
    return CreateMouseEvent(L);
}



////////////////////// keyboard //////////////////////////

int Event_Keyboard_kbhit(lua_State* L) {
    lua_pushboolean(L, (::_kbhit()) ? 1 : 0);
    return 1;
}

int Event_Keyboard_getch(lua_State* L) {
    lua_pushinteger(L, (lua_Integer)(::_getch()));
    return 1;
}


int Event_Keyboard_FlushKeyboardBuffer(lua_State* L) {
    ::FlushConsoleInputBuffer(::GetStdHandle(STD_INPUT_HANDLE));
    return 0;
}


TCHAR* KeyboardEventClassName = TEXT("LuaEasyX_KeyboardEvent");

#define KEYBOARDEVENT_METHOD(name) {#name, Event_Keyboard_##name}

const struct luaL_Reg kbeventmethods[] = {
    KEYBOARDEVENT_METHOD(kbhit),
    KEYBOARDEVENT_METHOD(getch),
    KEYBOARDEVENT_METHOD(FlushKeyboardBuffer),
    {NULL, NULL}
};

int CreateKeyboardEvent(lua_State* L) {
    int top = lua_gettop(L);

    BOOL bIsNew;

    int ret = c_comment_metatable(
        L,
        KeyboardEventClassName,
        NULL,
        LUAEASYX_STRUCTTYP_NONETYPE,
        &bIsNew,
        NULL,
        TEXT("KeyboardEvent函数表丢失！"),
        NULL
    );

    if (ret == COMMENT_CREATOR_SUCCESS_ONE) {
        if (bIsNew) {
            int kbeventmt_index = lua_gettop(L);
            luaL_setfuncs(L, kbeventmethods, 0);
        }
    } else {
        lua_settop(L, top);
        return 0;
    }

    return 1;
}


//////////////////////////////////////////////////////////

TCHAR* EventClassName = TEXT("LuaEasyX_Event");



int c_CreateEvent(lua_State* L) {

    int top = lua_gettop(L);

    BOOL bIsNew;

    int ret = c_comment_metatable(
        L,
        EventClassName,
        NULL,
        LUAEASYX_STRUCTTYP_NONETYPE,
        &bIsNew,
        NULL,
        TEXT("LuaEasyX_Event函数表丢失！"),
        NULL
    );

    int eventindex = 0;

    if (ret == COMMENT_CREATOR_SUCCESS_ONE) {

        if (bIsNew) {

            eventindex = lua_gettop(L);

            l_setobj(L, luaopen_CreateMouseEvent, "MouseEvent", eventindex);
            l_setobj(L, CreateKeyboardEvent, "KeyboardEvent", eventindex);
        }
    } else {
        lua_settop(L, top);
        return 0;
    }

    return 1;
}


LUAEASYX_EXPORT int luaopen_Event(lua_State* L) {
    return c_CreateEvent(L);
}

