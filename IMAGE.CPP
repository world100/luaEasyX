
#include "Image.h"

//所有函数中nil统一当做NULL


/*
// 从当前绘图设备获取图像
void getimage(
    IMAGE* pDstImg,   // 保存图像的 IMAGE 对象指针
    int srcX,         // 要获取图像区域左上角 x 坐标
    int srcY,         // 要获取图像区域的左上角 y 坐标
    int srcWidth,     // 要获取图像区域的宽度
    int srcHeight     // 要获取图像区域的高度
);
*/
//从当前绘图设备中获取图像
int Image_getimage(lua_State* L) {
    IMAGE* pDstImg = (IMAGE*)lua_touserdata(L, 1);
    int srcX = (int)luaL_checkinteger(L, 2);
    int srcY = (int)luaL_checkinteger(L, 3);
    int srcWidth  = (int)luaL_checkinteger(L, 4);
    int srcHeight = (int)luaL_checkinteger(L, 5);

    ::getimage(pDstImg, srcX, srcY, srcWidth, srcHeight);

    return 0;
}


/*
DWORD* GetImageBuffer(IMAGE* pImg = NULL);
*/
//获取绘图设备的显存指针
int Image_GetImageBuffer(lua_State* L) {
    IMAGE* pImg = NULL;
    if (lua_gettop(L) > 0) {
        pImg = (IMAGE*)lua_touserdata(L, lua_gettop(L));
    }

    DWORD *pMem = ::GetImageBuffer(pImg);

    if (!pMem)
        return 0;

    lua_pushlightuserdata(L, pMem);

    return 1;
}


/*
HDC GetImageHDC(IMAGE* pImg = NULL);
*/
//获取绘图设备句柄(HDC)
int Image_GetImageHDC(lua_State* L) {
    IMAGE* pImg = NULL;
    if (lua_gettop(L) > 0) {
        pImg = (IMAGE*)lua_touserdata(L, lua_gettop(L));
    }

    HDC hDC = ::GetImageHDC(pImg);

    if (!hDC)
        return 0;

    lua_pushlightuserdata(L, hDC);

    return 1;
}


/*
IMAGE* GetWorkingImage();
*/
//获取当前的绘图设备
int Image_GetWorkingImage(lua_State* L) {
    IMAGE* pImg = NULL;
    pImg = ::GetWorkingImage();
    lua_pushlightuserdata(L, pImg);
    return 1;
}


/*
// 从图片文件获取图像(bmp/gif/jpg/png/tif/emf/wmf/ico)
void loadimage(
	IMAGE* pDstImg,		// 保存图像的 IMAGE 对象指针
	LPCTSTR pImgFile,		// 图片文件名
	int nWidth = 0,		// 图片的拉伸宽度
	int nHeight = 0,		// 图片的拉伸高度
	bool bResize = false	// 是否调整 IMAGE 的大小以适应图片
);

// 从资源文件获取图像(bmp/gif/jpg/png/tif/emf/wmf/ico)
void loadimage(
	IMAGE* pDstImg,		// 保存图像的 IMAGE 对象指针
	LPCTSTR pResType,		// 资源类型
	LPCTSTR pResName,		// 资源名称
	int nWidth = 0,		// 图片的拉伸宽度
	int nHeight = 0,		// 图片的拉伸高度
	bool bResize = false	// 是否调整 IMAGE 的大小以适应图片
);
*/
//从文件中读取图像
int Image_loadimage(lua_State* L) {
    IMAGE* pDstImg = NULL;
    LPCTSTR pImgFile = NULL;
    LPCTSTR pResType = NULL;
    LPCTSTR pResName = NULL;
    int nWidth = 0, nHeight = 0;
    bool bResize = false;

    switch(lua_gettop(L)) {
    case 2:
        if (lua_type(L, 1) == LUA_TNIL)
            pDstImg = NULL;
        else
            pDstImg = (IMAGE*)lua_touserdata(L, 1);

        pImgFile = (LPCTSTR)luaL_checkstring(L, 2);
        ::loadimage(pDstImg, pImgFile);
        break;

    case 5:
        if (lua_type(L, 1) == LUA_TNIL)
            pDstImg = NULL;
        else
            pDstImg = (IMAGE*)lua_touserdata(L, 1);

        pImgFile = (LPCTSTR)luaL_checkstring(L, 2);
        nWidth  = (int)luaL_checkinteger(L, 3);
        nHeight = (int)luaL_checkinteger(L, 4);

        if (lua_type(L, 5) == LUA_TBOOLEAN) {
            bResize = lua_toboolean(L, 5) ? true : false;
        } else {
            luaL_error(L, TEXT("最后一个参数是boolean类型！"));
            return 0;
        }
        ::loadimage(pDstImg, pImgFile, nWidth, nHeight, bResize);
        break;

    case 3:
        if (lua_type(L, 1) == LUA_TNIL)
            pDstImg = NULL;
        else
            pDstImg = (IMAGE*)lua_touserdata(L, 1);

        pResType = (LPCTSTR)luaL_checkstring(L, 2);
        pResName = (LPCTSTR)luaL_checkstring(L, 3);
        ::loadimage(pDstImg, pResType, pResName);
        break;
    case 6:
        if (lua_type(L, 1) == LUA_TNIL)
            pDstImg = NULL;
        else
            pDstImg = (IMAGE*)lua_touserdata(L, 1);

        pResType = (LPCTSTR)luaL_checkstring(L, 2);
        pResName = (LPCTSTR)luaL_checkstring(L, 3);
        nWidth  = (int)luaL_checkinteger(L, 4);
        nHeight = (int)luaL_checkinteger(L, 5);

        if (lua_type(L, 6) == LUA_TBOOLEAN) {
            bResize = lua_toboolean(L, 6) ? true : false;
        } else {
            luaL_error(L, TEXT("最后一个参数是boolean类型！"));
            return 0;
        }

        ::loadimage(pDstImg, pResType, pResName, nWidth, nHeight, bResize);
        break;
    default:
        luaL_error(L, TEXT("最后一个参数是boolean类型！"));
    }

    return 0;
}


/*
// 绘制图像
void putimage(
    int dstX,              // 绘制位置的 x 坐标
    int dstY,              // 绘制位置的 y 坐标
    IMAGE *pSrcImg,        // 要绘制的 IMAGE 对象指针
    DWORD dwRop = SRCCOPY  // 三元光栅操作码（详见备注）
);

// 绘制图像(指定宽高和起始位置)
void putimage(
    int dstX,              // 绘制位置的 x 坐标
    int dstY,              // 绘制位置的 y 坐标
    int dstWidth,          // 绘制的宽度
    int dstHeight,         // 绘制的高度
    IMAGE *pSrcImg,        // 要绘制的 IMAGE 对象指针
    int srcX,              // 绘制内容在 IMAGE 对象中的左上角 x 坐标
    int srcY,              // 绘制内容在 IMAGE 对象中的左上角 y 坐标
    DWORD dwRop = SRCCOPY  // 三元光栅操作码（详见备注）
);
*/
//在当前设备上绘制指定图像
int Image_putimage(lua_State* L) {
    int srcX, srcY;
    IMAGE *pSrcImg;
    DWORD dwRop = SRCCOPY;

    int top = lua_gettop(L);

    if (3 <= top && top <= 4) {
        srcX = (int)luaL_checkinteger(L, 1);
        srcY = (int)luaL_checkinteger(L, 2);
        pSrcImg = (IMAGE*)lua_touserdata(L, 3);
        if (top == 4) {
            dwRop = (DWORD)luaL_checkinteger(L, 4);
        }
        ::putimage(srcX, srcY, pSrcImg, dwRop);
        return 0;
    }

    if (7 <= top && top <= 8) {
        int dstX = (int)luaL_checkinteger(L, 1);
        int dstY = (int)luaL_checkinteger(L, 2);
        int dstWidth  = (int)luaL_checkinteger(L, 3);
        int dstHeight = (int)luaL_checkinteger(L, 4);
        pSrcImg = (IMAGE*)lua_touserdata(L, 5);
        srcX = (int)luaL_checkinteger(L, 6);
        srcY = (int)luaL_checkinteger(L, 7);

        if (top == 8) {
            dwRop = (DWORD)luaL_checkinteger(L, 8);
        }

        ::putimage(dstX, dstY, dstWidth, dstHeight, pSrcImg, srcX, srcY, dwRop);
        return 0;
    }

    luaL_error(L, TEXT("参数错误！"));
    return 0;
}


/*
void Resize(IMAGE* pImg, int width, int height);
*/
//调整指定绘图设备的尺寸
int Image_Resize(lua_State* L) {
    IMAGE* pImg;
    if (lua_gettop(L) == 3) {
        if (lua_type(L, 1) == LUA_TNIL) {
            pImg = NULL;
        } else {
            pImg = (IMAGE*)lua_touserdata(L, 1);
        }
        int width  = (int)luaL_checkinteger(L, 2);
        int height = (int)luaL_checkinteger(L, 3);
        ::Resize(pImg, width, height);
    } else {
        luaL_error(L, TEXT("参数错误！"));
    }

    return 0;
}


/*
void rotateimage(
	IMAGE *dstimg,
	IMAGE *srcimg,
	double radian,
	COLORREF bkcolor = BLACK,
	bool autosize = false,
	bool highquality = true
);
*/
//旋转 IMAGE 中的绘图内容
int Image_rotateimage(lua_State* L) {
    IMAGE* dstimg;
    IMAGE* srcimg;
    double radian;
    COLORREF bkcolor = BLACK;
    bool autosize = false;
    bool highquality = false;

    int top = lua_gettop(L);

    if (top >= 3) {
        dstimg = (IMAGE*)lua_touserdata(L, 1);
        srcimg = (IMAGE*)lua_touserdata(L, 2);
        radian = (double)luaL_checknumber(L, 3);
        if (!(3 < top && top == 7)) {
            bkcolor = (COLORREF)luaL_checkinteger(L, 4);

            if (lua_type(L, 5) != LUA_TBOOLEAN)
                luaL_error(L, TEXT("autosize为boolean类型！"));
            autosize = lua_toboolean(L, 5) ? true : false;

            if (lua_type(L, 6) != LUA_TBOOLEAN)
                luaL_error(L, TEXT("highquality为boolean类型！"));
            highquality  = lua_toboolean(L, 6) ? true : false;
        }

        ::rotateimage(dstimg, srcimg, radian, bkcolor, autosize, highquality);
        return 0;
    }

    luaL_error(L, TEXT("参数出错！"));

    return 0;
}


/*
void saveimage(
	LPCTSTR strFileName,
	IMAGE* pImg = NULL
);
*/
//保存绘图内容至图片文件
int Image_saveimage(lua_State* L) {
    LPCTSTR strFileName;
    IMAGE* pImg;

    strFileName = (LPCTSTR)luaL_checkstring(L, 1);
    if (lua_type(L, 2) == LUA_TNIL) {
        pImg = NULL;
    } else {
        pImg = (IMAGE*)lua_touserdata(L, 2);
    }

    ::saveimage(strFileName, pImg);

    return 1;
}


/*
void SetWorkingImage(IMAGE* pImg = NULL);
*/
//设定当前的绘图设备
int Image_SetWorkingImage(lua_State* L) {
    IMAGE* pImg;

    if (lua_type(L, 1) == LUA_TNIL) {
        pImg = NULL;
    } else {
        pImg = (IMAGE*)lua_touserdata(L, 1);
    }

    ::SetWorkingImage(pImg);

    return 0;
}


//参数1：IMAGE指针
//返回值1：宽
//返回值2: 高
int Image_getimagesize(lua_State* L) {
    IMAGE* pImg = (IMAGE*)lua_touserdata(L, 1);
    lua_pushinteger(L, pImg->getwidth());
    lua_pushinteger(L, pImg->getheight());

    return 2;
}


//参数1：目标图像对象地址
//参数2：源图像对象地址
//返回值1：目标图像对象的地址
int Image_copyimage(lua_State* L) {
    IMAGE* dstImg;
    IMAGE* srcImg;

    dstImg = (IMAGE*)lua_touserdata(L, 1);
    srcImg = (IMAGE*)lua_touserdata(L, 2);

    (*dstImg) = (*srcImg);

    lua_pushlightuserdata(L, dstImg);

    return 1;
}


//获取本对象尺寸
int Image_IMAGE_getimagesize(lua_State* L) {
    LuaEasyX_StructureType* pls;
    pls = (LuaEasyX_StructureType*)lua_touserdata(L, 1);
    if (pls->uEndStructType == pls->uStructType && pls->StructAddress) {
        lua_settop(L, 0);
        lua_pushlightuserdata(L, pls->StructAddress);
        return Image_getimagesize(L);
    }

    return 0;
}


extern TCHAR* Image_IMAGEClassName;

TCHAR* Image_IMAGEClassName = TEXT("Image_IMAGE");

int Image_CreateIMAGEOBJECT(lua_State* L) {

    int top = lua_gettop(L);

    if (top > 2 || top == 1) {
        luaL_error(L, TEXT("IMAGE参数只能是 0个 或 2个 ！"));
        return 0;
    }

    int width = 0;
    int height = 0;

    if (top == 2) {
        width = (int)luaL_checkinteger(L, 1);
        height = (int)luaL_checkinteger(L, 2);
    }

    BOOL bIsNew;

    int ret = c_comment_metatable(L,
        Image_IMAGEClassName,
        (PVOID)(new IMAGE(width, height)),
        LUAEASYX_STRUCTTYP_IMAGE,
        &bIsNew,
        NULL,
        TEXT("Image对象表丢失！"),
        NULL
    );

    int newtop = lua_gettop(L);

    int udimage_index = newtop;
    int image_IMAGE_index = newtop - 1;


    if (ret == COMMENT_CREATOR_SUCCESS_TWO) {
        if (bIsNew) {

            lua_pushstring(L, "getimagesize");
            lua_pushcfunction(L, Image_IMAGE_getimagesize);
            lua_settable(L, image_IMAGE_index);

            lua_pushliteral(L, "__index");
            lua_pushvalue(L, image_IMAGE_index);
            lua_settable(L, image_IMAGE_index);
        }
    } else {
        lua_settop(L, top);
        return 0;
    }

    lua_pushvalue(L, image_IMAGE_index);
    lua_setmetatable(L, udimage_index);

    lua_settop(L, udimage_index);

    return 1;
}


//参数1：图片路径
//返回值1：IMAGE对象
int Image__call_loadimage(lua_State* L) {

    //第一个参数为Image表本身
    LPCTSTR lpFileName = (LPCTSTR)luaL_checkstring(L, 2);

    lua_settop(L, 0);

    if (Image_CreateIMAGEOBJECT(L) == 0)
        return 0;

    LuaEasyX_StructureType* pls = (LuaEasyX_StructureType*)lua_touserdata(L, -1);

    ::loadimage((IMAGE*)(pls->StructAddress), lpFileName);

    return 1;
}




TCHAR* ImageClassName = TEXT("LuaEasyX_Image");

#define IMAGE_METHOD(name) {#name, Image_##name}

const struct luaL_Reg imagemethods[] = {
    IMAGE_METHOD(getimage),
    IMAGE_METHOD(GetImageBuffer),
    IMAGE_METHOD(GetImageHDC),
    IMAGE_METHOD(GetWorkingImage),
    IMAGE_METHOD(loadimage),
    IMAGE_METHOD(putimage),
    IMAGE_METHOD(Resize),
    IMAGE_METHOD(rotateimage),
    IMAGE_METHOD(saveimage),
    IMAGE_METHOD(SetWorkingImage),
    IMAGE_METHOD(getimagesize),
    {NULL, NULL}
};

int c_CreateImage(lua_State* L) {

    int top = lua_gettop(L);

    BOOL bIsNew;

    int ret = c_comment_metatable(
        L,
        ImageClassName,
        NULL,
        LUAEASYX_STRUCTTYP_NONETYPE,
        &bIsNew,
        NULL,
        TEXT("Image函数表丢失！"),
        NULL
    );

    int image_index;

    if (ret == COMMENT_CREATOR_SUCCESS_ONE) {
        if (bIsNew) {
            image_index = lua_gettop(L);

            luaL_setfuncs(L, imagemethods, 0);

            lua_pushliteral(L, "__call");
            lua_pushcfunction(L, Image__call_loadimage);
            lua_settable(L, image_index);

            lua_pushliteral(L, "IMAGE");
            lua_pushcfunction(L, Image_CreateIMAGEOBJECT);
            lua_settable(L, image_index);

            lua_pushvalue(L, image_index);
            lua_setmetatable(L, image_index);
        }
    } else {
        lua_settop(L, top);
        return 0;
    }

    lua_settop(L, image_index);

    return 1;
}


LUAEASYX_EXPORT int luaopen_Image(lua_State* L) {
    return c_CreateImage(L);
}

