// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             //  从 Windows 头文件中排除极少使用的信息
// Windows 头文件:

#include <windows.h>

#include "stdio.h"
#include "all.h"
#include "MACLib.h"

#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/imgutils.h>
#include <libavutil/samplefmt.h>

static AVFormatContext *fmt_ctx = NULL;
static AVCodecContext  *audio_dec_ctx;
static AVStream  *audio_stream = NULL;
static uint8_t **audio_dst_data = NULL;
static int       audio_dst_linesize;
static int audio_dst_bufsize;
static const char *src_filename = NULL;
static FILE *audio_dst_file=NULL;
static int  audio_stream_idx = -1;
static AVFrame *frame = NULL;
static AVPacket pkt;
static int audio_frame_count = 0;
// TODO: 在此处引用程序需要的其他头文件
