// libtrans.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "libtrans.h"

// 这是导出函数的一个示例。
LIBTRANS_API int fnlibtrans(void)
{
	return 42;
}

// 这是已导出类的构造函数。
// 有关类定义的信息，请参阅 libtrans.h
Clibtrans::Clibtrans()
{
	return;
}
inline void debug_string(char* errconfig){fprintf(stderr, errconfig);}
int __stdcall convertape(const char* outfilename,const char *filename)
{
	int nAudioBytes = MAX_AUDIO_BYTES_UNKNOWN;
	const str_utf16 cOutputFile= (str_utf16)outfilename;
	avcodec_register_all();
	av_register_all();
	//////////////////////// 输入 ////////////////////////
	AVFormatContext *infmt_ctx;
	avformat_alloc_output_context2(&infmt_ctx,NULL,NULL,filename);
	//打开输入文件
	if(avformat_open_input(&infmt_ctx, filename, NULL, NULL)<0)
	{
		debug_string("can't open input file/n");
		return -1;
	} 
	//取出流信息
	if(av_find_stream_info(infmt_ctx) <0)
	{
		debug_string("can't find suitable codec parameters/n");
		return -2;
	}

	// 查找音频流信息
	int audioindex=-1;
	for(unsigned int j = 0; j < infmt_ctx->nb_streams; j++)
	{   
		if(infmt_ctx->streams[j]->codec->codec_type == AVMEDIA_TYPE_AUDIO)
		{
			audioindex=j;
			break;
		}
	}
	if(audioindex == -1) //没有找到音频流
	{
		debug_string("can't find audio stream/n");
		return -3;
	}
	AVCodecContext *incodec_ctx=NULL;
	AVCodec *incodec;
	incodec_ctx = infmt_ctx->streams[audioindex]->codec;
	//找到合适的音频解码器
	incodec = avcodec_find_decoder(incodec_ctx->codec_id);
	if(incodec == NULL)
	{
		debug_string("can't find suitable audio decoder/n");
		return -4;
	}
	//打开该音频解码器
	incodec_ctx = avcodec_alloc_context3(incodec);
    if (!incodec_ctx) {
        debug_string("Could not allocate audio codec context\n");
        exit(1);
    }
	if(avcodec_open2(incodec_ctx, incodec,NULL) < 0)
	{
		debug_string("can't open the audio decoder/n");
		return -5;
	}
	// set the output WAV format
	WAVEFORMATEX wfeAudioFormat; FillWaveFormatEx(&wfeAudioFormat, incodec_ctx->sample_rate, 16, incodec_ctx->channels);

	#define INBUF_SIZE 4096
    #define AUDIO_INBUF_SIZE 20480
    #define AUDIO_REFILL_THRESH 4096
    int len;
    FILE *f;
    uint8_t inbuf[AUDIO_INBUF_SIZE + FF_INPUT_BUFFER_PADDING_SIZE];
    AVPacket avpkt;
    AVFrame *decoded_frame = NULL;
    av_init_packet(&avpkt);

    fopen_s(&f,filename, "rb");
    if (!f) {
        debug_string("Could not open file\n");
        exit(1);
    }

	IAPECompress * pAPECompress = CreateIAPECompress();
	int nRetVal = pAPECompress->Start(&cOutputFile, &wfeAudioFormat, nAudioBytes, 
	COMPRESSION_LEVEL_NORMAL, NULL, CREATE_WAV_HEADER_ON_DECOMPRESSION);

	/* decode until eof */
	//on-the-fly encode
    avpkt.data = inbuf;
    avpkt.size = fread(inbuf, 1, AUDIO_INBUF_SIZE, f);

    while (avpkt.size > 0) {
        int got_frame = 0;
        if (!decoded_frame) {
            if (!(decoded_frame = avcodec_alloc_frame())) {
                debug_string("Could not allocate audio frame\n");
                exit(1);
            }
        } else
        avcodec_get_frame_defaults(decoded_frame);
        len = avcodec_decode_audio4(incodec_ctx, decoded_frame, &got_frame, &avpkt);
        if (len < 0) {
            debug_string( "Error while decoding\n");
            exit(1);
        }
        if (got_frame) {
            /* if a frame has been decoded, output it */
            int data_size = av_samples_get_buffer_size(NULL, incodec_ctx->channels,
                                                       decoded_frame->nb_samples,
                                                       incodec_ctx->sample_fmt, 1);
		// lock the compression buffer
		int nBufferBytesAvailable = 0;
		unsigned char * pBuffer = pAPECompress->LockBuffer(&nBufferBytesAvailable);

		pBuffer=decoded_frame->data[0];
		
		// unlock the buffer and let it get processed
		int nRetVal = pAPECompress->UnlockBuffer(data_size, TRUE);
		if (nRetVal != 0)
		{
			fprintf(stderr,"Error Encoding Frame (error: %d)\n", nRetVal);
			break;
		}
        }
        avpkt.size -= len;
        avpkt.data += len;
        avpkt.dts =
        avpkt.pts = AV_NOPTS_VALUE;
        if (avpkt.size < AUDIO_REFILL_THRESH) {
            /* Refill the input buffer, to avoid trying to decode
             * incomplete frames. Instead of this, one could also use
             * a parser, or use a proper container format through
             * libavformat. */
            memmove(inbuf, avpkt.data, avpkt.size);
            avpkt.data = inbuf;
            len = fread(avpkt.data + avpkt.size, 1,
                        AUDIO_INBUF_SIZE - avpkt.size, f);
            if (len > 0)
                avpkt.size += len;
        }
    }

	//clean up

    fclose(f);
    avcodec_close(incodec_ctx);
    av_free(incodec_ctx);
    avcodec_free_frame(&decoded_frame);
	if (pAPECompress->Finish(NULL, 0, 0) != 0)
	{
		debug_string("Error finishing encoder.\n");
	}
	SAFE_DELETE(pAPECompress)
	debug_string("Done.\n");
	return 0;

}
