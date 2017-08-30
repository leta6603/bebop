/*
 * h264Decoder.cpp
 *
 *  Created on: 2017. 8. 17.
 *      Author: lee
 */

#include <iostream>
#include <fstream>
#include <string>
#include <sys/time.h>
#include <pthread.h>
//to use C library
extern "C" {
#ifndef UINT64_C
#define UINT64_C(c) (c ## ULL)
#endif
#include "libavutil/imgutils.h"
#include "libavutil/opt.h"
#include "libavcodec/avcodec.h"
#include "libavutil/mathematics.h"
#include "libavutil/samplefmt.h"
#include "libavformat/avformat.h"
#include "unistd.h"
}

#include "Bebop.h"
#include "streamWindow.h"

struct frameData {
	int _width;
	int _height;
	AVFrame _picture;
	unsigned char _pGray[8];
	int _linesize;
};

extern FILE* videoDecode;//videoDecode.txt //
static bool isCodecActive;
static int nframe = 0;
static AVCodecContext *codecCtx = NULL;	// codec instance
static AVPacket avpkt;			// wrapper for encoded data handshaking
static AVFrame *picture = NULL;		// output picture

static void pgm_save(unsigned char *buf, int wrap, int xsize, int ysize,
		char *filename) {
	FILE *f;
	int i;

	f = fopen(filename, "w");
	fprintf(f, "P5\n%d %d\n%d\n", xsize, ysize, 255);
	for (i = 0; i < ysize; i++)
		fwrite(buf + i * wrap, 1, xsize, f);
	fclose(f);
}

bool logVideoDecodeStep(double timeDecode)
{
    if(videoDecode == NULL)
    {
        const char* VIDEO_DECODE_FILE_NAME = "SaveData/videoDecode.txt";
        videoDecode = fopen(VIDEO_DECODE_FILE_NAME, "w");
        if(videoDecode == NULL)
        {
                std::cout << "Can not open file" << std::endl;
                return false;
        }
    }

    if(videoDecode != NULL)
    {
        fprintf(videoDecode, "%f\n", timeDecode);
        fflush(videoDecode);
    }
    return true;
}

int h264DecoderDecode(unsigned char *inbuf, int len, bool toSave){

	int gotPicture, res;
	nframe++;
	// set up AVPacket with input data
	avpkt.data = inbuf;
	avpkt.size = len;

	//decode
	res = avcodec_decode_video2(codecCtx, picture, &gotPicture, &avpkt);
	if (res < 0) {
		std::cerr << "H264DecoderDecode decode error" << std::endl;
		fprintf(stderr, "", nframe);
		return -1;
	}
	if(gotPicture){
		std::cout << "decode success!!" << std::endl;
		presentTexture(picture->data[0], picture->linesize[0],
						 picture->data[1], picture->linesize[1],
						 picture->data[2], picture->linesize[2]);
	}

	return 0;
}


int H264DecoderInit() {
	static bool inited = false;
	AVCodec *codec;

	if (!inited) {     	// only once for program
		inited = true;
		av_register_all();
		avcodec_register_all();
	}

	av_init_packet(&avpkt);
	/* set end of buffer to 0 (this ensures that no overreading happens for damaged mpeg streams) */
	//memset(inbuf + INBUF_SIZE, 0, FF_INPUT_BUFFER_PADDING_SIZE);
	/* find the mpeg1 video decoder */
	codec = avcodec_find_decoder(AV_CODEC_ID_H264);
	if (!codec) {
		fprintf(stderr, "codec not found\n");
		exit(1);
	}

	codecCtx = avcodec_alloc_context3(codec);

#if LIBAVCODEC_VERSION_INT >= AV_VERSION_INT(55,28,1)
	picture = av_frame_alloc();
#else
	picture = avcodec_alloc_frame();

#endif
	if (codec->capabilities & CODEC_CAP_TRUNCATED)
		codecCtx->flags |= CODEC_FLAG_TRUNCATED; /* we do not send complete frames */

	/* open it */
	if (avcodec_open2(codecCtx, codec, NULL) < 0) {
		fprintf(stderr, "could not open codec\n");
		exit(1);
	}

	isCodecActive = true;

	return 0; // OK

}

int H264DecoderClose() {
	if (!isCodecActive) {
		std::cerr << "Codec Close Request in inactive" << std::endl;
		return -1;
	}
	isCodecActive = false;
	avcodec_close(codecCtx);
	av_free(codecCtx);
	av_free(picture);

	return 0; // ok
}
