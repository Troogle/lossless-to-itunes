// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� LIBTRANS_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// LIBTRANS_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#ifdef LIBTRANS_EXPORTS
#define LIBTRANS_API __declspec(dllexport)
#else
#define LIBTRANS_API __declspec(dllimport)
#endif

EXTERN_C
// �����Ǵ� libtrans.dll ������
class LIBTRANS_API Clibtrans {
public:
	Clibtrans(void);
	// TODO: �ڴ�������ķ�����
};
LIBTRANS_API int __stdcall convertape(const char* outfilename,const char *filename);
LIBTRANS_API int fnlibtrans(void);