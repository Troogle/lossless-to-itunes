// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� LIBCUE_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// LIBCUE_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#ifdef LIBCUE_EXPORTS
#define LIBCUE_API __declspec(dllexport)
#else
#define LIBCUE_API __declspec(dllimport)
#endif

// �����Ǵ� libcue.dll ������
class LIBCUE_API Clibcue {
public:
	Clibcue(void);
	int choosecoding(char name[]);
    void convert_to_utf8(char name[]);
	void setcue(char name[])

	// TODO: �ڴ�������ķ�����
};

extern LIBCUE_API int nlibcue;

LIBCUE_API int fnlibcue(void);
