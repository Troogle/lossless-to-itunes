// libtrans.cpp : ���� DLL Ӧ�ó���ĵ���������
//

#include "stdafx.h"
#include "libtrans.h"


// ���ǵ���������һ��ʾ��
LIBTRANS_API int nlibtrans=0;

// ���ǵ���������һ��ʾ����
LIBTRANS_API int fnlibtrans(void)
{
	return 42;
}

// �����ѵ�����Ĺ��캯����
// �й��ඨ�����Ϣ������� libtrans.h
Clibtrans::Clibtrans()
{
	return;
}
