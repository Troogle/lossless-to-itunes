#pragma once
//tta
#include <stdio.h>
#include <tchar.h>
#include "libtta.h"
#include "config.h"
#include "tta.h"
#define TTA_VERSION L(VERSION)
#define RIFF_SIGN (0x46464952)
#define WAVE_SIGN (0x45564157)
#define fmt_SIGN  (0x20746D66)
#define data_SIGN (0x61746164)
#define WAVE_FORMAT_PCM 1
#define WAVE_FORMAT_EXTENSIBLE 0xFFFE 
#define PCM_BUFFER_LENGTH 5120
typedef struct {
	TTAuint32 chunk_id;
	TTAuint32 chunk_size;
	TTAuint32 format;
	TTAuint32 subchunk_id;
	TTAuint32 subchunk_size;
	TTAuint16 audio_format;
	TTAuint16 num_channels;
	TTAuint32 sample_rate;
	TTAuint32 byte_rate;
	TTAuint16 block_align;
	TTAuint16 bits_per_sample;
} WAVE_hdr;
typedef struct {
	TTAuint32 subchunk_id;
	TTAuint32 subchunk_size;
} WAVE_subchunk_hdr;
typedef struct {
	TTAuint32 f1;
	TTAuint16 f2;
	TTAuint16 f3;
	TTAuint8 f4[8];
} WAVE_subformat;
typedef struct {
	TTAuint16 cb_size;
	TTAuint16 valid_bits;
	TTAuint32 ch_mask;
	WAVE_subformat est;
} WAVE_ext_hdr;
typedef struct {
	TTA_io_callback iocb;
	HANDLE handle;
} TTA_io_callback_wrapper;
//ape
#include "All.h"
#include <stdio.h>
#include "MACLib.h"
//tak
//flac
