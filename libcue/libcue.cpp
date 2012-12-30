// libcue.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "libcue.h"

/*
实现功能：
1.输入CUE，转码UTF-8（？）
（优先）2.输入CUE，改变tak/tta/wav/flac--->ape
3.合轨（？？？）

*/

// 这是导出变量的一个示例
LIBCUE_API int nlibcue=0;

// 这是导出函数的一个示例。
LIBCUE_API int fnlibcue(void)
{
	return 42;
}

// 这是已导出类的构造函数。
// 有关类定义的信息，请参阅 libcue.h
Clibcue::Clibcue()
{
	return;
}
int Clibcue::choosecoding(char name[])
{
	ifstream infile (name);
}
