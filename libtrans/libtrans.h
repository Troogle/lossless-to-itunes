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
	int convertape(void);
	int ConverAudio(const char* input_file, const char* output_file, int samples_rate, int channel);
	void audio_dec(const char *outfilename, const char *filename);
	// TODO: 在此添加您的方法。
};

LIBTRANS_API int fnlibtrans(void);
static int decode_packet(int *got_frame, int cached)
{
    int ret = 0;

    if (pkt.stream_index == audio_stream_idx) {
        /* decode audio frame */
        ret = avcodec_decode_audio4(audio_dec_ctx, frame, got_frame, &pkt);
        if (ret < 0) {
            fprintf(stderr, "Error decoding audio frame\n");
            return ret;
        }
		
		if (*got_frame) {
            printf("audio_frame%s n:%d nb_samples:%d pts:%s\n",
                   cached ? "(cached)" : "",
                   audio_frame_count++, frame->nb_samples,
                   av_ts2timestr(frame->pts, &audio_dec_ctx->time_base));

            ret = av_samples_alloc(audio_dst_data, &audio_dst_linesize, frame->channels,
                                   frame->nb_samples, frame->format, 1);
            if (ret < 0) {
                fprintf(stderr, "Could not allocate audio buffer\n");
                return AVERROR(ENOMEM);
            }

            /* TODO: extend return code of the av_samples_* functions so that this call is not needed */
            audio_dst_bufsize =
                av_samples_get_buffer_size(NULL, frame->channels,
                                           frame->nb_samples, frame->format, 1);

            /* copy audio data to destination buffer:
             * this is required since rawaudio expects non aligned data */
            av_samples_copy(audio_dst_data, frame->data, 0, 0,
                            frame->nb_samples, frame->channels, frame->format);

            /* write to rawaudio file */
            fwrite(audio_dst_data[0], 1, audio_dst_bufsize, audio_dst_file);
            av_freep(&audio_dst_data[0]);
        }
    }

    return ret;
}
static int open_codec_context(int *stream_idx,
                              AVFormatContext *fmt_ctx, enum AVMediaType type)
{
    int ret;
    AVStream *st;
    AVCodecContext *dec_ctx = NULL;
    AVCodec *dec = NULL;

    ret = av_find_best_stream(fmt_ctx, type, -1, -1, NULL, 0);
    if (ret < 0) {
        fprintf(stderr, "Could not find %s stream in input file '%s'\n",
                av_get_media_type_string(type), src_filename);
        return ret;
    } else {
        *stream_idx = ret;
        st = fmt_ctx->streams[*stream_idx];

        /* find decoder for the stream */
        dec_ctx = st->codec;
        dec = avcodec_find_decoder(dec_ctx->codec_id);
        if (!dec) {
            fprintf(stderr, "Failed to find %s codec\n",
                    av_get_media_type_string(type));
            return ret;
        }

        if ((ret = avcodec_open2(dec_ctx, dec, NULL)) < 0) {
            fprintf(stderr, "Failed to open %s codec\n",
                    av_get_media_type_string(type));
            return ret;
        }
    }

    return 0;
}

static int get_format_from_sample_fmt(const char **fmt,
                                      enum AVSampleFormat sample_fmt)
{
    int i;
    struct sample_fmt_entry {
        enum AVSampleFormat sample_fmt; const char *fmt_be, *fmt_le;
    } sample_fmt_entries[] = {
        { AV_SAMPLE_FMT_U8,  "u8",    "u8"    },
        { AV_SAMPLE_FMT_S16, "s16be", "s16le" },
        { AV_SAMPLE_FMT_S32, "s32be", "s32le" },
        { AV_SAMPLE_FMT_FLT, "f32be", "f32le" },
        { AV_SAMPLE_FMT_DBL, "f64be", "f64le" },
    };
    *fmt = NULL;

    for (i = 0; i < FF_ARRAY_ELEMS(sample_fmt_entries); i++) {
        struct sample_fmt_entry *entry = &sample_fmt_entries[i];
        if (sample_fmt == entry->sample_fmt) {
            *fmt = AV_NE(entry->fmt_be, entry->fmt_le);
            return 0;
        }
    }

    fprintf(stderr,
            "sample format %s is not supported as output format\n",
            av_get_sample_fmt_name(sample_fmt));
    return -1;
}