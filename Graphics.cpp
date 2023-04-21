#include "Graphics.h"


/* struct POINT*/

TCHAR* POINTSETClassName = TEXT("LuaEasyX_POINT");


//����1������POINT���ϱ�
//����ֵ1��POINT���ϱ�ת����POINT�����userdata
//����ֵ2��POINT���ϱ�Ԫ������
int graphics_POINT(lua_State* L) {
    double n1, n2;

    //���λ��
    int top = lua_gettop(L);

    if (top > 0) {
        if (lua_type(L, top) == LUA_TTABLE) {
            int len = lua_rawlen(L, top);
            if (len != 0) {

                int point_metatable_index;

                if (!luaL_newmetatable(L, POINTSETClassName)) {
                    if (!luaL_getmetatable(L, POINTSETClassName)) {
                        luaL_error(L, "POINT������ʧ��");
                        return 0;
                    } else {
                        point_metatable_index = lua_gettop(L);
                    }
                } else {
                    point_metatable_index = lua_gettop(L);

                    lua_pushstring(L, "__gc");
                    lua_pushcfunction(L, __gc_destroy_structure);
                    lua_settable(L, point_metatable_index);
                }

                LuaEasyX_StructureType *pls;
                pls = (LuaEasyX_StructureType*)lua_newuserdata(L, sizeof(LuaEasyX_StructureType));

                if (!pls)
                    return 0;

                pls->StructAddress = NULL;
                POINT *pPointSet;

                try {
                    pPointSet = new POINT[len];
                } catch (...) {return 0;}

                int pointset_index = lua_gettop(L);

                pls->uEndStructType = LUAEASYX_STRUCTTYP_POINTSET;
                pls->uStructType = LUAEASYX_STRUCTTYP_POINTSET;

                lua_pushvalue(L, point_metatable_index);
                lua_setmetatable(L, pointset_index);

                for (int i = 1; i <= len; i++) {
                    lua_rawgeti(L, top, i);
                    int t_top = lua_gettop(L);
                    if (lua_type(L, t_top) == LUA_TTABLE) {
                        if (lua_rawlen(L, t_top) == 2) {
                            //��һ��Ԫ��
                            lua_rawgeti(L, t_top, 1);
                            if (lua_type(L, -1) != LUA_TNUMBER) {
                                delete[] pPointSet;
                                luaL_error(L, "��[%d][1]Ԫ�ر���number���ͣ�\n", i);
                            } else {
                                n1 = (double)lua_tonumber(L, -1);
                            }
                            //�ڶ���Ԫ��
                            lua_rawgeti(L, t_top, 2);
                            if (lua_type(L, -1) != LUA_TNUMBER) {
                                delete[] pPointSet;
                                luaL_error(L, "��[%d][2]Ԫ�ر���number���ͣ�\n", i);
                            } else {
                                n2 = (double)lua_tonumber(L, -1);
                            }
                            pPointSet[i-1].x = (LONG)n1;
                            pPointSet[i-1].y = (LONG)n2;
                            lua_pop(L, 3);
                        } else {
                            delete[] pPointSet;
                            luaL_error(L, "��ά����Ԫ�ظ�������Ϊ2��\n");
                        }
                    } else {
                        delete[] pPointSet;
                        luaL_error(L, "����һά��� %d ��Ԫ�ز���table����!\n", i);
                    }
                }

                //for������
                ///////////////////////////////////////////
                //POINT��������
                pls->StructAddress = (PVOID)pPointSet;
                lua_pushvalue(L, pointset_index);

                //POINT����Ԫ������
                lua_pushinteger(L, (lua_Integer)(len));

                return 2;
                ///////////////////////////////////////////

            } else {
                luaL_error(L, "���ǿձ��봫��Ԫ����{Ԫ��1��Ԫ��2}\n");
            }
        } else {
            luaL_error(L, "�봫���ά��\n");
        }
    } else {
        luaL_error(L, "һ����������ά��\n");
    }

    return 0;
}


/*
void arc(
    int left,
    int top,
    int right,
    int bottom,
    double stangle,
    double endangle
);
*/
//����Բ��
int graphics_arc(lua_State* L) {
    int left    = (int)lua_tointeger(L, 1);
    int top     = (int)lua_tointeger(L, 2);
    int right   = (int)lua_tointeger(L, 3);
    int bottom  = (int)lua_tointeger(L, 4);
    double stangle  = (double)lua_tonumber(L, 5);
    double endangle = (double)lua_tonumber(L, 6);

    ::arc(left, top, right, bottom, stangle, endangle);

    return 0;
}


/*
void circle(
    int x,
    int y,
    int radius
);
*/
//��Բ
int graphics_circle(lua_State* L) {
    int x = (int)lua_tointeger(L, 1);
    int y = (int)lua_tointeger(L, 2);
    int radius = (int)lua_tointeger(L, 3);

    ::circle(x, y, radius);

    return 0;
}


/*
void clearcircle(
    int x,
    int y,
    int radius
);
*/
//���Բ������
int graphics_clearcircle(lua_State* L) {
    int x = (int)lua_tointeger(L, 1);
    int y = (int)lua_tointeger(L, 2);
    int radius = (int)lua_tointeger(L, 3);

    ::clearcircle(x, y, radius);

    return 0;
}


/*
void clearellipse(
    int left,
    int top,
    int right,
    int bottom
);
*/
//�����Բ����
int graphics_clearellipse(lua_State* L) {
    int left    = (int)lua_tointeger(L, 1);
    int top     = (int)lua_tointeger(L, 2);
    int right   = (int)lua_tointeger(L, 3);
    int bottom  = (int)lua_tointeger(L, 4);

    ::clearellipse(left, top, right, bottom);

    return 0;
}


/*
void clearpie(
    int left,
    int top,
    int right,
    int bottom,
    double stangle,
    double endangle
);
*/
//�����Բ��������
int graphics_clearpie(lua_State* L) {
    int left    = (int)lua_tointeger(L, 1);
    int top     = (int)lua_tointeger(L, 2);
    int right   = (int)lua_tointeger(L, 3);
    int bottom  = (int)lua_tointeger(L, 4);
    double stangle  = (double)lua_tonumber(L, 5);
    double endangle = (double)lua_tonumber(L, 6);

    ::clearpie(left, top, right, bottom, stangle, endangle);

    return 0;
}


/*
void clearpolygon(
    const POINT *points,
    int num
);
*/
//��ն��������
int graphics_clearpolygon(lua_State* L) {
    const POINT* points = (const POINT*)lua_touserdata(L, 1);
    int num = (int)luaL_checkinteger(L, 2);

    ::clearpolygon(points, num);

    return 0;
}


/*
void clearrectangle(
    int left,
    int top,
    int right,
    int bottom
);
*/
//��վ�������
int graphics_clearrectangle(lua_State* L) {
    int left    = (int)lua_tointeger(L, 1);
    int top     = (int)lua_tointeger(L, 2);
    int right   = (int)lua_tointeger(L, 3);
    int bottom  = (int)lua_tointeger(L, 4);

    ::clearrectangle(left, top, right, bottom);

    return 0;
}


/*
void clearroundrect(
    int left,
    int top,
    int right,
    int bottom,
    int ellipsewidth,
    int ellipseheight
);
*/
//���Բ�Ǿ�������
int graphics_clearroundrect(lua_State* L) {
    int left    = (int)lua_tointeger(L, 1);
    int top     = (int)lua_tointeger(L, 2);
    int right   = (int)lua_tointeger(L, 3);
    int bottom  = (int)lua_tointeger(L, 4);
    int ellipsewidth  = (int)lua_tointeger(L, 5);
    int ellipseheight = (int)lua_tointeger(L, 6);

    ::clearroundrect(left, top, right, bottom, ellipsewidth, ellipseheight);

    return 0;
}


/*
void ellipse(
    int left,
    int top,
    int right,
    int bottom
);
*/
//����Բ
int graphics_ellipse(lua_State* L) {
    int left    = (int)lua_tointeger(L, 1);
    int top     = (int)lua_tointeger(L, 2);
    int right   = (int)lua_tointeger(L, 3);
    int bottom  = (int)lua_tointeger(L, 4);

    ::ellipse(left, top, right, bottom);

    return 0;
}


/*
void fillcircle(
    int x,
    int y,
    int radius
);
*/
//�����Բ���б߿�
int graphics_fillcircle(lua_State* L) {
    int x = (int)lua_tointeger(L, 1);
    int y = (int)lua_tointeger(L, 2);
    int radius = (int)lua_tointeger(L, 3);

    ::fillcircle(x, y, radius);

    return 0;
}


/*
void fillellipse(
    int left,
    int top,
    int right,
    int bottom
);
*/
//��������Բ���б߿�
int graphics_fillellipse(lua_State* L) {
    int left    = (int)lua_tointeger(L, 1);
    int top     = (int)lua_tointeger(L, 2);
    int right   = (int)lua_tointeger(L, 3);
    int bottom  = (int)lua_tointeger(L, 4);

    ::fillellipse(left, top, right, bottom);

    return 0;
}


/*
void fillpie(
    int left,
    int top,
    int right,
    int bottom,
    double stangle,
    double endangle
);
*/
//��������Բ���Σ��б߿�
int graphics_fillpie(lua_State* L) {
    int left    = (int)lua_tointeger(L, 1);
    int top     = (int)lua_tointeger(L, 2);
    int right   = (int)lua_tointeger(L, 3);
    int bottom  = (int)lua_tointeger(L, 4);
    double stangle  = (double)lua_tonumber(L, 5);
    double endangle = (double)lua_tonumber(L, 6);

    ::fillpie(left, top, right, bottom, stangle, endangle);

    return 0;
}


/*
void fillpolygon(
    const POINT *points,
    int num
);
*/
//��������Σ��б߿�
int graphics_fillpolygon(lua_State* L) {
    const POINT* points = (const POINT *)lua_touserdata(L, 1);
    int num = (int)luaL_checkinteger(L, 2);

    ::fillpolygon(points, num);

    return 0;
}


/*
void fillrectangle(
    int left,
    int top,
    int right,
    int bottom
);
*/
//�������Σ��б߿�
int graphics_fillrectangle(lua_State* L) {
    int left    = (int)lua_tointeger(L, 1);
    int top     = (int)lua_tointeger(L, 2);
    int right   = (int)lua_tointeger(L, 3);
    int bottom  = (int)lua_tointeger(L, 4);

    ::fillrectangle(left, top, right, bottom);

    return 0;
}


/*
void fillroundrect(
    int left,
    int top,
    int right,
    int bottom,
    int ellipsewidth,
    int ellipseheight
);
*/
//�����Բ�Ǿ��Σ��б߿�
int graphics_fillroundrect(lua_State* L) {
    int left    = (int)lua_tointeger(L, 1);
    int top     = (int)lua_tointeger(L, 2);
    int right   = (int)lua_tointeger(L, 3);
    int bottom  = (int)lua_tointeger(L, 4);
    int ellipsewidth  = (int)lua_tointeger(L, 5);
    int ellipseheight = (int)lua_tointeger(L, 6);

    ::fillroundrect(left, top, right, bottom, ellipsewidth, ellipseheight);

    return 0;
}


/*
void floodfill(int x, int y, COLORREF color, int filltype = FLOODFILLBORDER)
*/
//�������
int graphics_floodfill(lua_State* L) {
    int x = (int)lua_tointeger(L, 1);
    int y = (int)lua_tointeger(L, 2);
    COLORREF color = (COLORREF)luaL_checkinteger(L, 3);

    int filltype;
    if (lua_gettop(L) == 3)
        filltype = FLOODFILLBORDER;
    else if (lua_gettop(L) == 4)
        filltype = (int)luaL_checkinteger(L, 4);
    else
        return 0;

    ::floodfill(x, y, color, filltype);

    return 0;
}


/*
int getheight();
*/
//��ȡ��ͼ���߶�
int graphics_getheight(lua_State* L) {
    lua_pushinteger(L, (int)(::getheight()));
    return 1;
}

/*
int getwidth();
*/
//��ȡ��ͼ�����
int graphics_getwidth(lua_State* L) {
    lua_pushinteger(L, (int)(::getwidth()));
    return 1;
}


/*
COLORREF getpixel(int x, int y);
*/
//��ȡ�����ɫ
int graphics_getpixel(lua_State* L) {
    int x = (int)luaL_checkinteger(L, 1);
    int y = (int)luaL_checkinteger(L, 2);

    lua_pushinteger(L, (lua_Integer)(::getpixel(x, y)));

    return 1;
}


/*
int getx();
int gety();
*/
//��ȡ��ǰ��x�����y����
//����ֵ1��x
//����ֵ2��y
int graphics_getxy(lua_State* L) {
    lua_pushinteger(L, (lua_Integer)(::getx()));
    lua_pushinteger(L, (lua_Integer)(::gety()));
    return 2;
}


/*
void line(
    int x1,
    int y1,
    int x2,
    int y2
);
*/
//��ֱ�ߡ��������� linerel �� lineto ��ֱ��
int graphics_line(lua_State* L) {
    int x1 = (int)lua_tointeger(L, 1);
    int y1 = (int)lua_tointeger(L, 2);
    int x2 = (int)lua_tointeger(L, 3);
    int y2 = (int)lua_tointeger(L, 4);

    ::line(x1, y1, x2, y2);

    return 0;
}


/*
void linerel(
    int dx,
    int dy
);
*/
//��ֱ�ߡ��������� line �� lineto ��ֱ��
int graphics_linerel(lua_State* L) {
    int dx = (int)lua_tointeger(L, 1);
    int dy = (int)lua_tointeger(L, 2);

    ::linerel(dx, dy);

    return 0;
}


/*
void lineto(
    int x,
    int y
);
*/
//��ֱ�ߡ��������� line �� linerel ��ֱ��
int graphics_lineto(lua_State* L) {
    int x = (int)lua_tointeger(L, 1);
    int y = (int)lua_tointeger(L, 2);

    ::lineto(x, y);

    return 0;
}


/*
void moverel(
    int dx,
    int dy
);
*/
//�ƶ���ǰ�㡣��Щ��ͼ������ӡ���ǰ�㡱��ʼ����������������øõ㡣
//�������� moveto ���õ�ǰ��
int graphics_moverel(lua_State *L) {
    int dx = (int)lua_tointeger(L, 1);
    int dy = (int)lua_tointeger(L, 2);

    ::moverel(dx, dy);

    return 0;
}


/*
void moveto(
    int x,
    int y
);
*/
//�ƶ���ǰ�㡣��Щ��ͼ������ӡ���ǰ�㡱��ʼ����������������øõ㡣
//�������� moverel ���õ�ǰ��
int graphics_moveto(lua_State* L) {
    int x = (int)lua_tointeger(L, 1);
    int y = (int)lua_tointeger(L, 2);

    ::moveto(x, y);

    return 0;
}


/*
void pie(
    int left,
    int top,
    int right,
    int bottom,
    double stangle,
    double endangle
);
*/
//����Բ����
int graphics_pie(lua_State* L) {
    int left    = (int)lua_tointeger(L, 1);
    int top     = (int)lua_tointeger(L, 2);
    int right   = (int)lua_tointeger(L, 3);
    int bottom  = (int)lua_tointeger(L, 4);
    double stangle  = (double)lua_tonumber(L, 5);
    double endangle = (double)lua_tonumber(L, 6);

    ::pie(left, top, right, bottom, stangle, endangle);

    return 0;
}


/*
void polybezier(
    const POINT *points,
    int num
);
*/
//�����η�����������
int graphics_polybezier(lua_State* L) {
    const POINT* points = (const POINT*)lua_touserdata(L, 1);
    int num = (int)luaL_checkinteger(L, 2);

    ::polybezier(points, num);

    return 0;
}


/*
void polygon(
    const POINT *points,
    int num
);
*/
//�������
int graphics_polygon(lua_State* L) {
    const POINT* points = (const POINT*)lua_touserdata(L, 1);
    int num = (int)luaL_checkinteger(L, 2);

    ::polygon(points, num);

    return 0;
}


/*
void polyline(
    const POINT *points,
    int num
);
*/
//��������������
int graphics_polyline(lua_State* L) {
    const POINT* points = (const POINT*)lua_touserdata(L, 1);
    int num = (int)luaL_checkinteger(L, 2);

    ::polyline(points, num);

    return 0;
}
///////////////////////////////////////////


/*
void putpixel(int x, int y, COLORREF color);
*/
//����
int graphics_putpixel(lua_State* L) {
    ::putpixel(
        (int)lua_tointeger(L, 1),
        (int)lua_tointeger(L, 2),
        (COLORREF)luaL_checkinteger(L, 3)
    );

    return 0;
}


/*
void rectangle(
    int left,
    int top,
    int right,
    int bottom
);
*/
//�����ľ���
int graphics_rectangle(lua_State* L) {
    int left    = (int)lua_tointeger(L, 1);
    int top     = (int)lua_tointeger(L, 2);
    int right   = (int)lua_tointeger(L, 3);
    int bottom  = (int)lua_tointeger(L, 4);

    ::rectangle(left, top, right, bottom);

    return 0;
}


/*
void roundrect(
    int left,
    int top,
    int right,
    int bottom,
    int ellipsewidth,
    int ellipseheight
);
*/
//������Բ�Ǿ���
int graphics_roundrect(lua_State* L) {
    int left    = (int)lua_tointeger(L, 1);
    int top     = (int)lua_tointeger(L, 2);
    int right   = (int)lua_tointeger(L, 3);
    int bottom  = (int)lua_tointeger(L, 4);
    int ellipsewidth   = (int)lua_tointeger(L, 5);
    int ellipseheight  = (int)lua_tointeger(L, 6);

    ::roundrect(left, top, right, bottom, ellipsewidth, ellipseheight);

    return 0;
}


/*
void solidcircle(
    int x,
    int y,
    int radius
);
*/
//�����Բ���ޱ߿�
int graphics_solidcircle(lua_State* L) {
    int x = (int)lua_tointeger(L, 1);
    int y = (int)lua_tointeger(L, 2);
    int radius = (int)lua_tointeger(L, 3);

    ::solidcircle(x, y, radius);

    return 0;
}


/*
void solidellipse(
    int left,
    int top,
    int right,
    int bottom
);
*/
//��������Բ���ޱ߿�
int graphics_solidellipse(lua_State* L) {
    int left    = (int)lua_tointeger(L, 1);
    int top     = (int)lua_tointeger(L, 2);
    int right   = (int)lua_tointeger(L, 3);
    int bottom  = (int)lua_tointeger(L, 4);

    ::solidellipse(left, top, right, bottom);

    return 0;
}


/*
void solidpie(
    int left,
    int top,
    int right,
    int bottom,
    double stangle,
    double endangle
);
*/
//��������Բ���Σ��ޱ߿�
int graphics_solidpie(lua_State* L) {
    int left    = (int)lua_tointeger(L, 1);
    int top     = (int)lua_tointeger(L, 2);
    int right   = (int)lua_tointeger(L, 3);
    int bottom  = (int)lua_tointeger(L, 4);
    double stangle   = (double)lua_tonumber(L, 5);
    double endangle  = (double)lua_tonumber(L, 6);

    ::solidpie(left, top, right, bottom, stangle, endangle);

    return 0;
}


/*
void solidpolygon(
    const POINT *points,
    int num
);
*/
//�����Ķ���Σ��ޱ߿�
int graphics_solidpolygon(lua_State* L) {
    const POINT* points = (const POINT*)lua_touserdata(L, 1);
    int num = (int)luaL_checkinteger(L, 2);

    ::solidpolygon(points, num);

    return 0;
}


/*
void solidrectangle(
    int left,
    int top,
    int right,
    int bottom
);
*/
//�������Σ��ޱ߿�
int graphics_solidrectangle(lua_State* L) {
    int left    = (int)lua_tointeger(L, 1);
    int top     = (int)lua_tointeger(L, 2);
    int right   = (int)lua_tointeger(L, 3);
    int bottom  = (int)lua_tointeger(L, 4);

    ::solidrectangle(left, top, right, bottom);

    return 0;
}


/*
void solidroundrect(
    int left,
    int top,
    int right,
    int bottom,
    int ellipsewidth,
    int ellipseheight
);
*/
//�����Բ�Ǿ��Σ��ޱ߿�
int graphics_solidroundrect(lua_State* L) {
    int left    = (int)lua_tointeger(L, 1);
    int top     = (int)lua_tointeger(L, 2);
    int right   = (int)lua_tointeger(L, 3);
    int bottom  = (int)lua_tointeger(L, 4);
    int ellipsewidth  = (int)lua_tointeger(L, 5);
    int ellipseheight = (int)lua_tointeger(L, 6);

    ::solidroundrect(left, top, right, bottom, ellipsewidth, ellipsewidth);

    return 0;
}

#define GRAPHICS_METHOD(name) {#name, graphics_##name}

const struct luaL_Reg grap_methods[] = {
    GRAPHICS_METHOD(arc),
    GRAPHICS_METHOD(circle),
    GRAPHICS_METHOD(clearcircle),
    GRAPHICS_METHOD(clearellipse),
    GRAPHICS_METHOD(clearpie),
    GRAPHICS_METHOD(clearrectangle),
    GRAPHICS_METHOD(clearroundrect),
    GRAPHICS_METHOD(ellipse),
    GRAPHICS_METHOD(fillcircle),
    GRAPHICS_METHOD(fillellipse),
    GRAPHICS_METHOD(fillpie),
    GRAPHICS_METHOD(fillrectangle),
    GRAPHICS_METHOD(fillroundrect),
    GRAPHICS_METHOD(floodfill),
    GRAPHICS_METHOD(getheight),
    GRAPHICS_METHOD(getwidth),
    GRAPHICS_METHOD(getpixel),
    GRAPHICS_METHOD(getxy),
    GRAPHICS_METHOD(line),
    GRAPHICS_METHOD(linerel),
    GRAPHICS_METHOD(lineto),
    GRAPHICS_METHOD(moverel),
    GRAPHICS_METHOD(moveto),
    GRAPHICS_METHOD(pie),
    GRAPHICS_METHOD(putpixel),
    GRAPHICS_METHOD(rectangle),
    GRAPHICS_METHOD(roundrect),
    GRAPHICS_METHOD(solidcircle),
    GRAPHICS_METHOD(solidellipse),
    GRAPHICS_METHOD(solidpie),
    GRAPHICS_METHOD(solidrectangle),
    GRAPHICS_METHOD(solidroundrect),
    GRAPHICS_METHOD(POINT),
    {NULL, NULL}
};

//����Դ�����ҪPOINT
const struct luaL_Reg grap_methods_points[] = {
    GRAPHICS_METHOD(clearpolygon),
    GRAPHICS_METHOD(fillpolygon),
    GRAPHICS_METHOD(polybezier),
    GRAPHICS_METHOD(polygon),
    GRAPHICS_METHOD(polyline),
    GRAPHICS_METHOD(solidpolygon),
    {NULL, NULL}
};


int GraphicsThunkPoints(lua_State* L) {
    luaL_Reg* p;

    p = (luaL_Reg*)lua_touserdata(L, lua_upvalueindex(1));

    if (p == NULL) {
        luaL_error(L, TEXT("Graphics������ʧ��"));
        return 0;
    }

    //��ҪPOINT���ϵĺ������ڵ�һ������
    int top = lua_gettop(L);

    if (top != 2) {
        luaL_error(L, TEXT("������ȷ�ĵ��÷�ʽ������ȷ��ȷ��������"));
        return 0;
    }

    if (lua_type(L, 1) == LUA_TUSERDATA) {
        LuaEasyX_StructureType* pls;
        pls = (LuaEasyX_StructureType*)luaL_checkudata(L, 1, POINTSETClassName);

        if (pls && pls->uEndStructType == pls->uStructType && pls->StructAddress) {
            lua_pushlightuserdata(L, pls->StructAddress);
            lua_replace(L, 1);

            return p->func(L);
        }
    }

    return 0;
}

TCHAR* GraphicsClassName = TEXT("LuaEasyX_Graphics");


int graphics_CreateGraphics(lua_State* L) {

    int top = lua_gettop(L);

    BOOL bIsNew;

    int ret = c_comment_metatable(L,
        GraphicsClassName,
        NULL,
        LUAEASYX_STRUCTTYP_NONETYPE,
        &bIsNew,
        NULL,
        TEXT("Graphics������ʧ��"),
        NULL
    );

    int grah_method_index = 0;

    if (ret == COMMENT_CREATOR_SUCCESS_ONE) {

        if (bIsNew) {
            grah_method_index = lua_gettop(L);

            luaL_setfuncs(L, (luaL_Reg*)grap_methods, 0);

            for (struct luaL_Reg* p = (luaL_Reg*)grap_methods_points;
                    p->name && p->func; p++) {

                lua_pushstring(L, p->name);
                lua_pushlightuserdata(L, p);
                lua_pushcclosure(L, p->func, 1);
                lua_settable(L, grah_method_index);
            }
        }
    } else {
        lua_settop(L, top);
        return 0;
    }

    return 1;
}



LUAEASYX_EXPORT int luaopen_Graphics(lua_State* L) {
    return graphics_CreateGraphics(L);
}

