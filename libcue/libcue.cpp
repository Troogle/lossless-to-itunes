// libcue.cpp : ���� DLL Ӧ�ó���ĵ���������
//

#include "stdafx.h"
#include "libcue.h"

/*
ʵ�ֹ��ܣ�
1.����CUE��ת��UTF-8������
�����ȣ�2.����CUE���ı�tak/tta/wav/flac--->ape
3.�Ϲ죨��������

*/

// ���ǵ���������һ��ʾ��
LIBCUE_API int nlibcue=0;

// ���ǵ���������һ��ʾ����
LIBCUE_API int fnlibcue(void)
{
	return 42;
}

// �����ѵ�����Ĺ��캯����
// �й��ඨ�����Ϣ������� libcue.h
Clibcue::Clibcue()
{
	return;
}
int Clibcue::choosecoding(char name[])
{
	ifstream infile (name);
}
