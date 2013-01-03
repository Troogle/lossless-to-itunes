// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 LIBTRANS_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// LIBTRANS_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
#ifdef LIBTRANS_EXPORTS
#define LIBTRANS_API __declspec(dllexport)
#else
#define LIBTRANS_API __declspec(dllimport)
#endif

// 此类是从 libtrans.dll 导出的
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
	// TODO: 在此添加您的方法。
};

LIBTRANS_API int fnlibtrans(void);