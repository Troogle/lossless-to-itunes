// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             //  �� Windows ͷ�ļ����ų�����ʹ�õ���Ϣ
// Windows ͷ�ļ�:

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
// TODO: �ڴ˴����ó�����Ҫ������ͷ�ļ�
