/*******************************************************************************
 * Author :          Sequin_YF
 * Email :           catherine199787@outlook.com
 * Last modified :   2016-08-19 11:20
 * Filename :        test.c
 * Description :
 * *****************************************************************************/

#include<stdio.h>
#include<unistd.h>
#include<errno.h>

// ---- 设置窗口大小
void CMyConsole::SetSize(short len, short wid)  // 设置同样的缓冲区和窗口大小，不会出现滚动条
{
	COORD pos = {len, wid};
	SMALL_RECT rc = {0, 0, len-1, wid-1};		// 坐标从0开始

	// -- 设置窗口信息
	// @param	HANDLE	[in] 窗口句柄
	// @param	bool	[in] 意思不明，但在true时才起作用
	// @param	RECT *	[in] 分别指定窗口左上角坐标和右下角坐标
	// #return	bool		 成功返回非0值
	SetConsoleWindowInfo(hOut, true, &rc);

	// -- 设置缓冲区大小
	// -- 长和宽不得小于控制台大小；不得小于系统最小限制。否则设置无效
	// @param	HANDLE	[in] 窗口句柄
	// @param	COORD	[in] 坐标结构，包含长和宽
	// #return	bool		 成功返回非0值
	SetConsoleScreenBufferSize(hOut, pos);
}

// ---- 设置光标位置
void CMyConsole::Goto(short x, short y)
{
	COORD pos = {x, y};

	SetConsoleCursorPosition(hOut, pos);
}

int main()
{
    CMyConsole myConsole;

    myConsole.SetSize(60, 30);
    myConsole.Goto(18, 14);
    printf("设置完毕！尺寸大小：60*30！\n");
    myConsole.Goto(0, 0);
    system("pause");

    return 0;
}

