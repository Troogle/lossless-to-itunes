// libtrans.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "libtrans.h"
static TTAwchar *myname = NULL;
TICK_COUNT_TYPE g_nInitialTickCount = 0;
//tta
void tta_strerror(TTA_CODEC_STATUS e) {
	using namespace tta;
	switch(e) {
		case TTA_OPEN_ERROR: tta_print("\r%s: can't open file\n", myname); break;
		case TTA_FORMAT_ERROR: tta_print("\r%s: not compatible file format\n", myname); break;
		case TTA_FILE_ERROR: tta_print("\r%s: file is corrupted\n", myname); break;
		case TTA_READ_ERROR: tta_print("\r%s: can't read from input file\n", myname); break;
		case TTA_WRITE_ERROR: tta_print("\r%s: can't write to output file\n", myname); break;
		case TTA_MEMORY_ERROR: tta_print("\r%s: insufficient memory available\n", myname); break;
		case TTA_SEEK_ERROR: tta_print("\r%s: file seek error\n", myname); break;
		case TTA_PASSWORD_ERROR: tta_print("\r%s: password protected file\n", myname); break;
		case TTA_NOT_SUPPORTED: tta_print("\r%s: unsupported architecture type\n", myname); break;
		default: tta_print("\rUnknown error\n"); break;
	}
}
int write_wav_hdr(HANDLE outfile, WAVE_hdr *wave_hdr, TTAuint32 data_size) {
	TTAuint32 result;
	WAVE_subchunk_hdr subchunk_hdr;

	subchunk_hdr.subchunk_id = data_SIGN;
	subchunk_hdr.subchunk_size = data_size;

	// Write WAVE header
	if (!tta_write(outfile, wave_hdr, sizeof(WAVE_hdr), result) ||
		result != sizeof(WAVE_hdr)) return -1;

	// Write Subchunk header
	if (!tta_write(outfile, &subchunk_hdr, sizeof(WAVE_subchunk_hdr), result) ||
		result != sizeof(WAVE_subchunk_hdr)) return -1;

	return 0;
} // write_wav_hdr
void CALLBACK tta_callback(TTAuint32 rate, TTAuint32 fnum, TTAuint32 frames) {
	TTAuint32 pcnt = (TTAuint32)(fnum * 100. / frames);
	if (!(pcnt % 10))
		tta_print("\rProgress: %02d%%", pcnt);
} // tta_callback
TTAint32 CALLBACK read_callback(TTA_io_callback *io, TTAuint8 *buffer, TTAuint32 size) {
	TTA_io_callback_wrapper *iocb = (TTA_io_callback_wrapper *)io; 
	TTAint32 result;
	if (tta_read(iocb->handle, buffer, size, result))
		return result;
	return 0;
} // read_callback
TTAint64 CALLBACK seek_callback(TTA_io_callback *io, TTAint64 offset) {
	TTA_io_callback_wrapper *iocb = (TTA_io_callback_wrapper *)io; 
	return tta_seek(iocb->handle, offset);
} // seek_callback
int ttadecompressW(HANDLE infile, HANDLE outfile) {
	using namespace tta;
	WAVE_hdr wave_hdr;
	tta_decoder *TTA;
	TTA_io_callback_wrapper io;
	TTAuint8 *buffer = NULL;
	TTAuint32 buf_size, smp_size, data_size, res;
	TTAint32 len;
	TTA_info info;
	int ret = -1;

	io.iocb.read = &read_callback;
	io.iocb.seek = &seek_callback;
	io.iocb.write = NULL;
	io.handle = infile;

	TTA = new tta_decoder((TTA_io_callback *) &io);

	try {
		TTA->init_get_info(&info, 0);
	} catch (tta_exception ex) {
		tta_strerror(ex.code());
		goto done;
	}

	smp_size = info.nch * ((info.bps + 7) / 8);
	buf_size = PCM_BUFFER_LENGTH * smp_size;

	// allocate memory for PCM buffer
	buffer = (TTAuint8 *) tta_malloc(buf_size + 4); // +4 for WRITE_BUFFER macro
	if (buffer == NULL) {
		tta_strerror(TTA_MEMORY_ERROR);
		goto done;
	}

	// Fill in WAV header
	data_size = info.samples * smp_size;
	tta_memclear(&wave_hdr, sizeof (wave_hdr));
	wave_hdr.chunk_id = RIFF_SIGN;
	wave_hdr.chunk_size = data_size + 36;
	wave_hdr.format = WAVE_SIGN;
	wave_hdr.subchunk_id = fmt_SIGN;
	wave_hdr.subchunk_size = 16;
	wave_hdr.audio_format = 1;
	wave_hdr.num_channels = (TTAuint16) info.nch;
	wave_hdr.sample_rate = info.sps;
	wave_hdr.bits_per_sample = info.bps;
	wave_hdr.byte_rate = info.sps * smp_size;
	wave_hdr.block_align = (TTAuint16) smp_size;

	// Write WAVE header
	if (write_wav_hdr(outfile, &wave_hdr, data_size)){
		tta_strerror(TTA_WRITE_ERROR);
		goto done;
	}

	try {
		while (1) {
			len = TTA->process_stream(buffer, buf_size, tta_callback);
			if (len) {
				if (!tta_write(outfile, buffer, len * smp_size, res) || !res)
					throw tta_exception(TTA_WRITE_ERROR);
			} else break;
		}
		ret = 0;
	} catch (tta_exception ex) {
		tta_strerror(ex.code());
	}

done:
	delete TTA;
	if (buffer) tta_free(buffer);

	return ret;
} // decompress
void usage() {
	tta_print("\rUsage:\tlibtrans [htkfa] input_file output_file\n\n");

	tta_print("\t-h\tprint help\n");
	tta_print("\t-t\tdecode tta file\n");
	tta_print("\t-k\tdecode tak file\n");
	tta_print("\t-f\tdecode flac file\n");
	tta_print("\t-a\tencode ape file\n");
	tta_print("Project site: NULL for now\n");
} // usage
int ttadecompress(int argc, _TCHAR* argv[]){
	using namespace tta;
    TTAwchar *fname_in, *fname_out;
	HANDLE infile = INVALID_HANDLE_VALUE;
	HANDLE outfile = INVALID_HANDLE_VALUE;
	TTAuint32 start, end;
	int ret=0;
	myname = argv[0];
	if (argc > 3) {
	fname_in = argv[argc - 2];
	fname_out = argv[argc - 1];
		argc -= 2;
	} else {
		usage();
		goto done;
	}

	infile = tta_open_read(fname_in);
	if (infile == INVALID_HANDLE_VALUE) {
		tta_strerror(TTA_OPEN_ERROR);
		goto done;
	}
	outfile = tta_open_write(fname_out);
	if (outfile == INVALID_HANDLE_VALUE) {
		tta_strerror(TTA_OPEN_ERROR);
		goto done;
	}
	start = GetTickCount();
	tta_print("\rDecoding: \"%s\" to \"%s\"\n", fname_in, fname_out);
	ret = ttadecompressW(infile, outfile);
	if (!ret) {
	end = GetTickCount();
	tta_print("\rTime: %.3f sec.\n",
			(end - start) / 1000.);
     	}
	if (infile != STDIN_FILENO) tta_close(infile);
	if (outfile != STDOUT_FILENO) {
	tta_close(outfile);
	if (ret) tta_unlink(fname_out);
	}
	done:
	return ret;
}
//ape
bool FileExists(wchar_t * pFilename)
{    
    if (0 == wcscmp(pFilename, L"-")  ||  0 == wcscmp(pFilename, L"/dev/stdin"))
        return true;
    bool bFound = false;
    WIN32_FIND_DATA WFD;
    HANDLE hFind = FindFirstFile(pFilename, &WFD);
    if (hFind != INVALID_HANDLE_VALUE)
    {
        bFound = true;
        FindClose(hFind);
    }
    return bFound;
}
void CALLBACK ProgressCallback(int nPercentageDone)
{
	using namespace APE;
    // get the current tick count
	TICK_COUNT_TYPE  nTickCount;
	TICK_COUNT_READ(nTickCount);

	// calculate the progress
	double dProgress = nPercentageDone / 1.e5;											// [0...1]
	double dElapsed = (double) (nTickCount - g_nInitialTickCount) / TICK_COUNT_FREQ;	// seconds
	double dRemaining = dElapsed * ((1.0 / dProgress) - 1.0);							// seconds

	// output the progress
	_ftprintf(stderr, _T("Progress: %.1f%% (%.1f seconds remaining, %.1f seconds total)          \r"), 
		dProgress * 100, dRemaining, dElapsed);
}
int apecompress(int argc, TCHAR * argv[]){
	using namespace APE;
	CSmartPtr<wchar_t> spInputFilename; CSmartPtr<wchar_t> spOutputFilename;
	int nRetVal = ERROR_UNDEFINED;
	int nCompressionLevel;
	int nPercentageDone;
	spInputFilename.Assign(argv[2], TRUE, FALSE);
	spOutputFilename.Assign(argv[3], TRUE, FALSE);
	// verify that the input file exists
	if (!FileExists(spInputFilename))
	{
		_ftprintf(stderr, _T("Input File Not Found...\n\n"));
		exit(-1);
	}
	nCompressionLevel =3000;
	// set the initial tick count
	TICK_COUNT_READ(g_nInitialTickCount);
	// process
	int nKillFlag = 0;
	_ftprintf(stderr, _T("Compressing (%s)...\n"), _T("high"));
	nRetVal = CompressFileW(spInputFilename, spOutputFilename, nCompressionLevel, &nPercentageDone, ProgressCallback, &nKillFlag);

	if (nRetVal == ERROR_SUCCESS) 
		_ftprintf(stderr, _T("\nSuccess...\n"));
	else 
		_ftprintf(stderr, _T("\nError: %i\n"), nRetVal);
	return nRetVal;
}


int _tmain(int argc, _TCHAR* argv[])
{
	setlocale(LC_ALL, LOCALE);
	int act = 0;
	int ret = -1;
	tta_print("\r\nGoogle's lossless audio encoder/decoder \n TTAversion %s\n", TTA_VERSION);
	tta_print(" APEversion %s\n", MAC_VERSION_STRING);
	switch (argv[1][0]) {
		case 'h': // print help
			usage();
			goto done;
		case 't': // decode tta
			act = 1;
			break;
		case 'k': // decode tak
			act = 2;
			break;
		case 'f': // decode flac
			act = 3;
			break;
		case 'a': // encode ape
			act=4;
			break;
		case '?':usage();
		default:
			goto done;
	}
		if (!act) {
		tta_print("\r%s: commandline options invivlid\n", myname);
		goto done;
	}

	// process
	switch (act) {
	case 1:
		ret=ttadecompress(argc,argv);
		break;
	case 4:
		ret=apecompress(argc,argv);
     	break;
	}

done:
	return ret;
}



