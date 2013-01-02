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
void Clibtrans::audio_dec(const char *outfilename, const char *filename)
{
    av_register_all();
	if (avformat_open_input(&fmt_ctx, filename, NULL, NULL) < 0) {
        (stderr, "Could not open source file %s\n", src_filename);
        exit(1);
    }
	if (avformat_find_stream_info(fmt_ctx, NULL) < 0) {
        fprintf(stderr, "Could not find stream information\n");
        exit(1);
    }
	    if (open_codec_context(&audio_stream_idx, fmt_ctx, AVMEDIA_TYPE_AUDIO) >= 0) {
        int nb_planes;

        audio_stream = fmt_ctx->streams[audio_stream_idx];
        audio_dec_ctx = audio_stream->codec;
        audio_dst_file = fopen(audio_dst_filename, "wb");
        if (!audio_dst_file) {
            fprintf(stderr, "Could not open destination file %s\n", video_dst_filename);
            ret = 1;
            goto end;
        }

        nb_planes = av_sample_fmt_is_planar(audio_dec_ctx->sample_fmt) ?
            audio_dec_ctx->channels : 1;
        audio_dst_data = av_mallocz(sizeof(uint8_t *) * nb_planes);
        if (!audio_dst_data) {
            fprintf(stderr, "Could not allocate audio data buffers\n");
            ret = AVERROR(ENOMEM);
            goto end;
        }
    }
		    av_dump_format(fmt_ctx, 0, src_filename, 0);

    if (!audio_stream && !video_stream) {
        fprintf(stderr, "Could not find audio or video stream in the input, aborting\n");
        ret = 1;
        goto end;
    }

    frame = avcodec_alloc_frame();
    if (!frame) {
        fprintf(stderr, "Could not allocate frame\n");
        ret = AVERROR(ENOMEM);
        goto end;
    }

    /* initialize packet, set data to NULL, let the demuxer fill it */
    av_init_packet(&pkt);
    pkt.data = NULL;
    pkt.size = 0;

    if (video_stream)
        printf("Demuxing video from file '%s' into '%s'\n", src_filename, video_dst_filename);
    if (audio_stream)
        printf("Demuxing audio from file '%s' into '%s'\n", src_filename, audio_dst_filename);

    /* read frames from the file */
    while (av_read_frame(fmt_ctx, &pkt) >= 0)
        decode_packet(&got_frame, 0);

    /* flush cached frames */
    pkt.data = NULL;
    pkt.size = 0;
    do {
        decode_packet(&got_frame, 1);
    } while (got_frame);

    printf("Demuxing succeeded.\n");

    if (video_stream) {
        printf("Play the output video file with the command:\n"
               "ffplay -f rawvideo -pix_fmt %s -video_size %dx%d %s\n",
               av_get_pix_fmt_name(video_dec_ctx->pix_fmt), video_dec_ctx->width, video_dec_ctx->height,
               video_dst_filename);
    }

    if (audio_stream) {
        const char *fmt;

        if ((ret = get_format_from_sample_fmt(&fmt, audio_dec_ctx->sample_fmt) < 0))
            goto end;
        printf("Play the output audio file with the command:\n"
               "ffplay -f %s -ac %d -ar %d %s\n",
               fmt, audio_dec_ctx->channels, audio_dec_ctx->sample_rate,
               audio_dst_filename);
    }

end:
    if (video_dec_ctx)
        avcodec_close(video_dec_ctx);
    if (audio_dec_ctx)
        avcodec_close(audio_dec_ctx);
    avformat_close_input(&fmt_ctx);
    if (video_dst_file)
        fclose(video_dst_file);
    if (audio_dst_file)
        fclose(audio_dst_file);
    av_free(frame);
    av_free(video_dst_data[0]);
    av_free(audio_dst_data);

    return ret < 0;
}
    //#define INBUF_SIZE 4096
    //#define AUDIO_INBUF_SIZE 20480
    //#define AUDIO_REFILL_THRESH 4096
    //avcodec_register_all();
    //AVCodec *codec;
    //AVCodecContext *c= NULL;
    //int len;
    //FILE *f, *outfile;
    //uint8_t inbuf[AUDIO_INBUF_SIZE + FF_INPUT_BUFFER_PADDING_SIZE];
    //AVPacket avpkt;
    //AVFrame *decoded_frame = NULL;

    //av_init_packet(&avpkt);

    ///* find the mpeg audio decoder */
    //codec = avcodec_find_decoder(AV_CODEC_ID_MP2);
    //if (!codec) {
    //    fprintf(stderr, "Codec not found\n");
    //    exit(1);
    //}

    //c = avcodec_alloc_context3(codec);
    //if (!c) {
    //    fprintf(stderr, "Could not allocate audio codec context\n");
    //    exit(1);
    //}

    ///* open it */
    //if (avcodec_open2(c, codec, NULL) < 0) {
    //    fprintf(stderr, "Could not open codec\n");
    //    exit(1);
    //}

    //fopen_s(&f,filename, "rb");
    //if (!f) {
    //    fprintf(stderr, "Could not open %s\n", filename);
    //    exit(1);
    //}
    //fopen_s(&outfile,outfilename, "wb");
    //if (!outfile) {
    //    av_free(c);
    //    exit(1);
    //}

    ///* decode until eof */
    //avpkt.data = inbuf;
    //avpkt.size = fread(inbuf, 1, AUDIO_INBUF_SIZE, f);

    //while (avpkt.size > 0) {
    //    int got_frame = 0;

    //    if (!decoded_frame) {
    //        if (!(decoded_frame = avcodec_alloc_frame())) {
    //            fprintf(stderr, "Could not allocate audio frame\n");
    //            exit(1);
    //        }
    //    } else
    //        avcodec_get_frame_defaults(decoded_frame);

    //    len = avcodec_decode_audio4(c, decoded_frame, &got_frame, &avpkt);
    //    if (len < 0) {
    //        fprintf(stderr, "Error while decoding\n");
    //        exit(1);
    //    }
    //    if (got_frame) {
    //        /* if a frame has been decoded, output it */
    //        int data_size = av_samples_get_buffer_size(NULL, c->channels,
    //                                                   decoded_frame->nb_samples,
    //                                                   c->sample_fmt, 1);
    //        fwrite(decoded_frame->data[0], 1, data_size, outfile);
    //    }
    //    avpkt.size -= len;
    //    avpkt.data += len;
    //    avpkt.dts =
    //    avpkt.pts = AV_NOPTS_VALUE;
    //    if (avpkt.size < AUDIO_REFILL_THRESH) {
    //        /* Refill the input buffer, to avoid trying to decode
    //         * incomplete frames. Instead of this, one could also use
    //         * a parser, or use a proper container format through
    //         * libavformat. */
    //        memmove(inbuf, avpkt.data, avpkt.size);
    //        avpkt.data = inbuf;
    //        len = fread(avpkt.data + avpkt.size, 1,
    //                    AUDIO_INBUF_SIZE - avpkt.size, f);
    //        if (len > 0)
    //            avpkt.size += len;
    //    }
    //}

    //fclose(outfile);
    //fclose(f);

    //avcodec_close(c);
    //av_free(c);
    //avcodec_free_frame(&decoded_frame);
}

//int Clibtrans::ConverAudio(const char* input_file, const char* output_file, int samples_rate, int channel)
//{
//AVFormatContext *infmt_ctx;
//	int i = 0;
//	//////////////////////// 初始化 ////////////////////////
//	avcodec_register_all();
//    av_register_all();
//	//////////////////////// 输入 ////////////////////////
//	infmt_ctx = av_alloc_format_context();
//	//打开输入文件
//	if(av_open_input_file(&infmt_ctx, input_file, NULL, 0, NULL)!=0)
//	{
//		debug_string("can't open input file/n");
//		return -1;
//	} 
//	//取出流信息
//	if(av_find_stream_info(infmt_ctx) <0)
//	{
//		debug_string("can't find suitable codec parameters/n");
//		return -2;
//	}
//	//dump_format(infmt_ctx, 0, input_file, 0); //列出输入文件的相关流信息
//	// 查找音频流信息
//	int audioindex=-1;
//	for(unsigned int j = 0; j < infmt_ctx->nb_streams; j++)
//	{   
//		if(infmt_ctx->streams[j]->codec->codec_type == CODEC_TYPE_AUDIO)
//		{
//			audioindex=j;
//			break;
//		}
//	}
//	if(audioindex == -1) //没有找到音频流
//	{
//		debug_string("can't find audio stream/n");
//		return -3;
//	}
//	AVCodecContext *incode_ctx;
//	AVCodec *incodec;
//	incode_ctx = infmt_ctx->streams[audioindex]->codec;
//	//找到合适的音频解码器
//	incodec = avcodec_find_decoder(incode_ctx->codec_id);
//	if(incodec == NULL)
//	{
//		debug_string("can't find suitable audio decoder/n");
//		return -4;
//	}
//	//打开该音频解码器
//	if(avcodec_open(incode_ctx, incodec) < 0)
//	{
//		debug_string("can't open the audio decoder/n");
//		return -5;
//	}
//	
//	//////////////////////// 输出 ////////////////////////
//	/* 解析输出文件的格式 */
//	AVOutputFormat *outfmt = guess_format(NULL, output_file, NULL);
//	if (!outfmt) {
//		printf("Could not deduce output format from file extension: using MPEG./n");
//		outfmt = guess_format("mpeg", NULL, NULL);
//	}
//	if (!outfmt) {
//		debug_string("Could not find suitable output format/n");
//		return -6;
//	}
//	outfmt->audio_codec = CODEC_ID_MP3;
//	/* allocate the output media context */
//	AVFormatContext *outfmt_ctx = av_alloc_format_context();
//	if (!outfmt_ctx) {
//		debug_string("Memory error/n");
//		return -7;
//	}
//	/* 保存输出文件的格式 */
//	outfmt_ctx->oformat = outfmt;
//	snprintf(outfmt_ctx->filename, sizeof(outfmt_ctx->filename), "%s", output_file);
//	/* add the audio and video streams using the default format codecs and initialize the codecs */
//	/* 输出文件的视频流编码器ID */
//	AVCodecContext *outcode_ctx;
//	AVCodec *outcodec;
//	AVStream *audio_st = NULL/*, *video_st*/;
//	//double audio_pts, video_pts;
//	//if (outfmt->video_codec != CODEC_ID_NONE) {
//	//	video_st = add_video_stream(outfmt_ctx, outfmt->video_codec);
//	//}
//	/* 输出文件的音频流编码器ID */
//	if (outfmt->audio_codec != CODEC_ID_NONE) {
//		audio_st = av_new_stream(outfmt_ctx, 1);
//		
//		outcode_ctx = audio_st->codec;
//		outcode_ctx->codec_id = outfmt->audio_codec;
//		outcode_ctx->codec_type = CODEC_TYPE_AUDIO;
//		//oAcc->bit_rate =inCodecCtx->bit_rate ;
//		outcode_ctx->sample_rate = samples_rate > 0 ? samples_rate : incode_ctx->sample_rate;
//		//outcode_ctx->time_base.den = 1;
//		//outcode_ctx->time_base.num = outcode_ctx->sample_rate;
//		outcode_ctx->channels    = channel > 0 ? channel : incode_ctx->channels;
//		outcode_ctx->block_align = incode_ctx->block_align;
//		if(outcode_ctx->block_align == 1 && outcode_ctx->codec_id == CODEC_ID_MP3)
//			outcode_ctx->block_align = 0;
//	}
//	/* 设置输出参数 */
//	if (av_set_parameters(outfmt_ctx, NULL) < 0) {
//		debug_string("Invalid output format parameters/n");
//		return -8;
//	}
//	
//	/* 列出输出文件的格式信息 */
//	dump_format(outfmt_ctx, 0, output_file, 1);
//	strcpy(outfmt_ctx->title, infmt_ctx->title);
//	strcpy(outfmt_ctx->author, infmt_ctx->author);
//	strcpy(outfmt_ctx->copyright, infmt_ctx->copyright);
//	strcpy(outfmt_ctx->comment, infmt_ctx->comment);
//	strcpy(outfmt_ctx->album, infmt_ctx->album);
//	outfmt_ctx->year = infmt_ctx->year;
//	outfmt_ctx->track = infmt_ctx->track;
//	strcpy(outfmt_ctx->genre, infmt_ctx->genre);
//	//dump_format(oc,0,output_file_name,1);//列出输出文件的相关流信息
//	//找到合适的音频编码器
//	outcodec = avcodec_find_encoder(outfmt_ctx->oformat->audio_codec);//(CODEC_ID_AAC);
//	if(!outcodec)
//	{
//		debug_string("can't find suitable audio encoder/n");
//		return -9;
//	}
//	if(avcodec_open(outcode_ctx, outcodec) <0)      //////////////////////////////////////////////////
//	{
//		debug_string("can't open the output audio codec");
//		return -10;
//	}
//	/* now that all the parameters are set, we can open the audio and
//	video codecs and allocate the necessary encode buffers */
//	//if (video_st)
//	//	open_video(outfmt_ctx, video_st);
//	/* 打开音频编码器 */
//	/*
//	int audioSize = 128 * 1024 * 4; //AVCODEC_MAX_AUDIO_FRAME_SIZE;//audio_outbuf_size/incode_ctx->channels;
//	if (outcode_ctx->frame_size <= 1)
//	{
//		switch(outcode_ctx->codec_id)
//		{
//			case CODEC_ID_PCM_S32LE:
//			case CODEC_ID_PCM_S32BE:
//			case CODEC_ID_PCM_U32LE:
//			case CODEC_ID_PCM_U32BE:
//				audioSize <<= 1;
//				break;
//			case CODEC_ID_PCM_S24LE:
//			case CODEC_ID_PCM_S24BE:
//			case CODEC_ID_PCM_U24LE:
//			case CODEC_ID_PCM_U24BE:
//			case CODEC_ID_PCM_S24DAUD:
//				audioSize = audioSize / 2 * 3;
//				break;
//			case CODEC_ID_PCM_S16LE:
//			case CODEC_ID_PCM_S16BE:
//			case CODEC_ID_PCM_U16LE:
//			case CODEC_ID_PCM_U16BE:
//				break;
//			default:
//				audioSize >>= 1;
//				break;
//		}
//		outSampleSize = audioSize * 2 * outcode_ctx->channels;
//	}
//	else
//	{
//		outSampleSize = outcode_ctx->frame_size * 2 * outcode_ctx->channels;
//	}
//	*/
//	
//	/* 如果输出文件不存在, 则创建输出文件 */
//	if (!(outfmt->flags & AVFMT_NOFILE)) {
//		if (url_fopen(&outfmt_ctx->pb, output_file, URL_WRONLY) < 0) {
//			debug_string("Could not open '%s'/n", output_file);
//			return -11;
//		}
//	}
//	
//	int ret = 0;
//    
//    uint8_t* outbuf;
//    uint8_t* audiobuf;
//    uint8_t* resamplebuf;
//    uint8_t* databuf;
//	int outbuf_len = AVCODEC_MAX_AUDIO_FRAME_SIZE;;
//    int audiobuf_len = AVCODEC_MAX_AUDIO_FRAME_SIZE;
//    int resamplebuf_len = AVCODEC_MAX_AUDIO_FRAME_SIZE;
//	int64_t pt;
//	outbuf = (uint8_t*)av_malloc(outbuf_len);
//	audiobuf = (uint8_t*)av_malloc(audiobuf_len);
//	resamplebuf = (uint8_t*)av_malloc(resamplebuf_len);
//	databuf = (uint8_t*)av_malloc(AVCODEC_MAX_AUDIO_FRAME_SIZE);
//	AVFifoBuffer fifo;
//	av_fifo_init(&fifo, av_fifo_size(&fifo)+AVCODEC_MAX_AUDIO_FRAME_SIZE);
//	ReSampleContext* resample = audio_resample_init(outcode_ctx->channels, incode_ctx->channels, outcode_ctx->sample_rate, incode_ctx->sample_rate);
//	av_write_header(outfmt_ctx);
//	AVPacket packet;
//	while(av_read_frame(infmt_ctx, &packet) >= 0) {
//		pt = packet.pts;
//		if(outcode_ctx->codec_id != CODEC_ID_NONE && packet.stream_index == audio_st->index) {
//			if(incode_ctx->codec_id == outcode_ctx->codec_id && incode_ctx->sample_rate == outcode_ctx->sample_rate && incode_ctx->channels == incode_ctx->channels) {
//				av_write_frame(outfmt_ctx, &packet);
//			}
//			else {
//				if (packet.size > 0)
//				{
//					avcodec_decode_audio(incode_ctx, (short *)audiobuf, &audiobuf_len, packet.data, packet.size);
//					if(incode_ctx->sample_rate != outcode_ctx->sample_rate || incode_ctx->channels != outcode_ctx->channels) {
//						// resample the audio
//						resamplebuf_len = audio_resample(resample, (short*)resamplebuf, (short*)audiobuf, audiobuf_len / (incode_ctx->channels * 2));
//						//av_fifo_realloc(&fifo, av_fifo_size(&fifo)+audiobuf_len); 
//						av_fifo_write(&fifo, resamplebuf, resamplebuf_len * outcode_ctx->channels * 2);
//					}
//					else {
//						//av_fifo_realloc(&fifo, av_fifo_size(&fifo)+audiobuf_len); 
//						av_fifo_write(&fifo, audiobuf, audiobuf_len);
//					}
//					if(audiobuf_len > 0) {
//						int frame_bytes = outcode_ctx->frame_size * 2 * outcode_ctx->channels;
//						while(av_fifo_read(&fifo, databuf, frame_bytes) == 0) {
//							AVPacket pkt;
//							av_init_packet(&pkt);
//							pkt.size = avcodec_encode_audio(outcode_ctx, outbuf, outbuf_len, (short*)databuf);
//							if (outcode_ctx->coded_frame && outcode_ctx->coded_frame->pts != AV_NOPTS_VALUE)  
//								pkt.pts = av_rescale_q(outcode_ctx->coded_frame->pts, outcode_ctx->time_base, audio_st->time_base);  
//							pkt.stream_index = audio_st->index;
//							pkt.data = outbuf;
//							pkt.flags |= PKT_FLAG_KEY;
//							// write the compressed frame in the media file
//							if (pkt.size > 0)
//							{
//								ret = av_write_frame(outfmt_ctx, &pkt);
//							}
//							av_free_packet(&pkt);
//						}
//					}
//				} //End if
//			}
//		}
//		av_free_packet(&packet);
//	}
//	
//	av_fifo_free(&fifo);
//	/* write the trailer, if any */
//	av_write_trailer(outfmt_ctx);
//	av_free(outbuf);
//	av_free(audiobuf);
//	av_free(resamplebuf);
//	av_free(databuf);
//	
//	audio_resample_close(resample);
//	avcodec_close(outcode_ctx);
//	/* free the streams */
//	for(i = 0; i < outfmt_ctx->nb_streams; i++) {
//		av_freep(&outfmt_ctx->streams[i]->codec);
//		av_freep(&outfmt_ctx->streams[i]);
//	}
//	if (!(outfmt->flags & AVFMT_NOFILE)) {
//		/* close the output file */
//		url_fclose(outfmt_ctx->pb);
//	}
//	av_free(outfmt_ctx);
//	
//	avcodec_close(incode_ctx);
//	av_close_input_file(infmt_ctx);
//	return ret;
//}

int Clibtrans::convertape()
{
/***************************************************************************************

This example illustrates using MACLib.lib to create an APE file by encoding some
random data.

The IAPECompress interface fully supports on-the-fly encoding.  To use on-
the-fly encoding, be sure to tell the encoder to create the proper WAV header on
decompression.  Also, you need to specify the absolute maximum audio bytes that will
get encoded. (trying to encode more than the limit set on Start() will cause failure)
This maximum is used to allocates space in the seek table at the front of the file.  Currently,
it takes around 8k per hour of CD music, so it isn't a big deal to allocate more than
needed.  You can also specify MAX_AUDIO_BYTES_UNKNOWN to allocate as much space as possible. (2 GB)
untime library to "Mutlithreaded"

***************************************************************************************/

	int nAudioBytes = 1048576*10;
	const str_utf16 cOutputFile= (str_utf16)"c:\\Noise.ape";
	
	printf("Creating file: %s\n", cOutputFile);
	
	///////////////////////////////////////////////////////////////////////////////
	// create and start the encoder
	///////////////////////////////////////////////////////////////////////////////
	
	// set the input WAV format
	WAVEFORMATEX wfeAudioFormat; FillWaveFormatEx(&wfeAudioFormat, 44100, 16, 2);
	
	// create the encoder interface
	IAPECompress * pAPECompress = CreateIAPECompress();
	
	// start the encoder
	int nRetVal = pAPECompress->Start(&cOutputFile, &wfeAudioFormat, nAudioBytes, 
		COMPRESSION_LEVEL_HIGH, NULL, CREATE_WAV_HEADER_ON_DECOMPRESSION);

	if (nRetVal != 0)
	{
		SAFE_DELETE(pAPECompress)
		printf("Error starting encoder.\n");
		return -1;
	}
	
	///////////////////////////////////////////////////////////////////////////////
	// pump through and feed the encoder audio data (white noise for the sample)
	///////////////////////////////////////////////////////////////////////////////
	int nAudioBytesLeft = nAudioBytes;
	
	while (nAudioBytesLeft > 0)
	{
		///////////////////////////////////////////////////////////////////////////////
		// NOTE: we're locking the buffer used internally by MAC and copying the data
		//		 directly into it... however, you could also use the AddData(...) command
		//       to avoid the added complexity of locking and unlocking 
		//       the buffer (but it may be a little slower )
		///////////////////////////////////////////////////////////////////////////////
	
		// lock the compression buffer
		int nBufferBytesAvailable = 0;
		unsigned char * pBuffer = pAPECompress->LockBuffer(&nBufferBytesAvailable);
	
		// fill the buffer with white noise
		int nNoiseBytes = min(nBufferBytesAvailable, nAudioBytesLeft);
		for (int z = 0; z < nNoiseBytes; z++)
		{
			pBuffer[z] = rand() % 255;
		}

		// unlock the buffer and let it get processed
		int nRetVal = pAPECompress->UnlockBuffer(nNoiseBytes, TRUE);
		if (nRetVal != 0)
		{
			printf("Error Encoding Frame (error: %d)\n", nRetVal);
			break;
		}

		// update the audio bytes left
		nAudioBytesLeft -= nNoiseBytes;
	}

	///////////////////////////////////////////////////////////////////////////////
	// finalize the file (could append a tag, or WAV terminating data)
	///////////////////////////////////////////////////////////////////////////////
	if (pAPECompress->Finish(NULL, 0, 0) != 0)
	{
		printf("Error finishing encoder.\n");
	}

	///////////////////////////////////////////////////////////////////////////////
	// clean up and quit
	///////////////////////////////////////////////////////////////////////////////
	SAFE_DELETE(pAPECompress)
	printf("Done.\n");
	return 0;

}
