LuaEasyX 是对 EasyX 封装的一个 Lua 模块，可以实现 EasyX 提供的功能，优点是可以不依赖 C 编译器的情况下实现 EasyX 的功能，只要装有 Lua 即可通过编写 Lua 代码调用 EasyX 的 api，实现 EasyX 提供的 api 相应功能。

只实现了 EasyX 文档中提及的功能，其他的得自己扩展。我自己也添加了一些 win32api 到 LuaEasyX，其函数在 LuaEasyX.WinGUI 表中。

附带了若干示例。例如其中的 LuaDino 是 Chrome 浏览器自带的小游戏 dino 的简单实现。想玩原版的可以在 chrome 浏览器的地址栏输入 chrome://dino 试试。

编译器：	Visual C++ 2010
lua 版本：	Lua 5.3.5
EasyX 版本：	EasyX 20180727(beta)
最后修改：	2018-12-26

********************************************************************************************************

与文档中各个部分对应的 Lua 表
DrawDevice：	绘图环境相关函数
Color：		颜色模型
Style：		图形颜色及样式设置相关函数
Graphics：	图形绘制相关函数
Text：		文字输出相关函数
Image：		图像处理相关函数
Event.MouseEvent：	鼠标相关函数
Event.KeyboardEvent：	键盘事件
Option：	其他函数
Extend：	graphics.h 新增函数
WinGUI：	自己扩展的win32api

上述的表都在 LuaEasyX 中

一些函数的使用方法在 samples 中给出示例

在获取到一个对象后 (userdata) 要用函数 GetObjectAddress 获取到对象真正的地址再传入到需要这个地址的函数
GetObjectAddress 返回的是 lightuserdata

还有一些细节大家就自己去看源代码吧，hiahiahia，因为源代码对每个函数都做了详细的注解。
如果有哪位小伙伴愿意写文档的话，欢迎鸭，嘻嘻~~

在这个压缩包中，我把我编译时用到的 Lua 动态库、头文件、以及导入文件到放进来的了，包括 lua53 解释器


emmmmm 实在想不出要说啥了，日后有空再给大家写点有趣的小栗子吧mua~~~

这是原作者，我只是传到github做个记录
https://codebus.cn/contributor/xiaobing-luaeasyx