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

// �����Ǵ� libtrans.dll ������
class LIBTRANS_API Clibtrans {
public:
	Clibtrans(void);
	int convertape(const char* outfilename,const char *filename);
	int ConverAudio(const char* input_file, const char* output_file, int samples_rate, int channel);
	int audio_dec(const char *outfilename, const char *filename);
private:
	static int decode_packet(int *got_frame, int cached);
	static int open_codec_context(int *stream_idx,AVFormatContext *fmt_ctx, enum AVMediaType type);
	static int get_format_from_sample_fmt(const char **fmt,enum AVSampleFormat sample_fmt);
		int audio_dec2(const char *outfilename, const char *filename);
	// TODO: �ڴ�������ķ�����
};

LIBTRANS_API int fnlibtrans(void);