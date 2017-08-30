/*
 * bebopVideo.cpp
 *
 *  Created on: 2017. 8. 17.
 *      Author: lee
 */
#include <iostream>

#include "ARConstant.h"
#include "Bebop.h"

// PARROT Video Frame Format
#pragma pack(push)
#pragma pack(1)
typedef struct {
	unsigned short frameNumber; 		// 16LE(0),
	unsigned char frameFlags;  		// UInt8(2),
	unsigned char fragmentNumber; 	// UInt8(3),
	unsigned char fragmentsPerFrame;	// UInt8(4),
	unsigned char frame[1];	// Variable Length (size is defined in basic header)
} videoframe_t;
#pragma pack(pop)

// video fragment store
typedef struct {
	unsigned short frameNumber;
	int frameFlags;
	bool waitForIframe;

	unsigned char fragmentNumber;
	int rxFragmentCount;
	unsigned char fragments[128][1024];
	int fragmentlens[128];
	unsigned char frameACK[16];  // 16*8 = 1024 fragment is possible
} videostore_t;


//variables
static bool isVideoModuleInited;
static videostore_t videostore;
static bool isFrameMade;
static unsigned char mergedFrames[128 * 1024];  // 2*128 KB, double buffer
static int totalLen = 0;

void* decodeFrame(void* arg){
	while(1){
		if(isFrameMade){
			//TODO : decode and rendering   mergedFrames, isFrameMade
			h264DecoderDecode(mergedFrames, totalLen, false);
			isFrameMade = false;
		}
	}
}

bool mediaStreamingEnable(bool enable) {
	unsigned char _project = ARConstant::ARCOMMANDS_ID_PROJECT_ARDRONE3;
	unsigned char _class = ARConstant::ARCOMMANDS_ID_ARDRONE3_CLASS_MEDIASTREAMING;

	unsigned int size = 12;
	//frame_t pframe;
	unsigned char command[13];
	memset(command, 0, 13);

	command[0] = ARConstant::ARNETWORKAL_FRAME_TYPE_DATA; //pframe.type;
	command[1] = ARConstant::BD_NET_CD_NONACK_ID; //pframe.id;
	command[2] = seq[ARConstant::BD_NET_CD_NONACK_ID]++; //pframe.seq;

	char Byte1 = (size & 0xff000000) >> 24;
	char Byte2 = (size & 0xff0000) >> 16;
	char Byte3 = (size & 0xff00) >> 8;
	char Byte4 = (size & 0xff);

	command[3] = Byte4;
	command[4] = Byte3;
	command[5] = Byte2;
	command[6] = Byte1;

	command[7] = _project;
	command[8] = _class;
	command[9] = 0;
	command[10] = 0;
	if (enable)
		command[11] = 1;
	else
		command[11] = 0;

#ifdef	DEBUG_VIDEO
	if(enable) {
		vidfptr = fopen("bebopvid.h264", "wb");
		if(!vidfptr) {
			perror("Cannot open");
		}
	} else {
		fclose(vidfptr);
	}
#else
	if (enable)
		H264DecoderInit();
	else
		H264DecoderClose();
#endif

	// 2. send
	int n = rio_writen(c2dSock, &command, 12);

	return true;
}
/*
bool mediaStreamingEnable(bool enable) {

	unsigned short _cmd = 0;

	unsigned int size = 11;
	unsigned char command[13];
	memset(command, 0, 13);
	command[0] = ARConstant::ARNETWORKAL_FRAME_TYPE_DATA;
	command[1] = ARConstant::BD_NET_CD_NONACK_ID;
	command[2] = seq[ARConstant::BD_NET_CD_NONACK_ID]++;

	command[3] = 11;
	command[4] = 0;
	command[5] = 0;
	command[6] = 0;
	//-------------------------------------

	command[7] = ARConstant::ARCOMMANDS_ID_PROJECT_ARDRONE3;
	command[8] = ARConstant::ARCOMMANDS_ID_ARDRONE3_CLASS_MEDIASTREAMING;
	command[9] = 0;
	command[10] = 0;

	if (enable)
		command[11] = 1;
	else
		command[11] = 0;

	if (enable)
		H264DecoderInit();
	else
		H264DecoderClose();

	// 2. send
	int n = rio_writen(c2dSock, &command, 12);
	if(n < 0)
		std::cerr << "mediaStreamingEnable write error " << std::endl;

	return true;
}*/

static int sendVideoAck() {
	unsigned char buf[1024];
	// 1. build frame
	frame_t *pAck = (frame_t *) buf;
	pAck->type = ARConstant::ARNETWORKAL_FRAME_TYPE_DATA;
	pAck->id = ARConstant::BD_NET_CD_VIDEO_ACK_ID;
	pAck->seq = seq[pAck->id]++;
	pAck->size = 7 + 18; // header (7) + frame number (2) + 16 fragment ack list

	memcpy(pAck->data, &videostore.frameNumber, 2);  // frame number
	// switch consequence of low 8bytes and high 8bytes
	memcpy(&pAck->data[2], &videostore.frameACK[8], 8);  // copy low 8 bytes
	memcpy(&pAck->data[10], videostore.frameACK, 8);     // copy high 8 bytes

	// 3. send
	int n = rio_writen(c2dSock, buf, 25);
	if(n < 0){
		std::cerr << "sendVideoAck write error" << std::endl;
	}
	return 0;
}


int onVideoFrame(unsigned char *data, int size){
	bool emit = false;
	if(!isVideoModuleInited){
		videostore.frameNumber = 0;
		videostore.waitForIframe = true;
		videostore.rxFragmentCount = 0;

		for (int i = 0; i < 128; i++)
			videostore.fragmentlens[i] = 0;		//length of each fragments
		for (int i = 0; i < 16; i++)
			videostore.frameACK[16] = 0;  // 16*8 = 128 fragment is possible
	}
	videoframe_t *newFrame = (videoframe_t *) data;

	//If a new picture frame arrived, Then handle the stored one
	if (newFrame->frameNumber != videostore.frameNumber) {

		//Check wheather we have stored picture or not
		if (videostore.rxFragmentCount > 0) {
			emit = false; // default

			// if missing frames, we have to wait for I-frame
			if (newFrame->frameNumber != videostore.frameNumber + 1)
				videostore.waitForIframe = true;

			// if it's an I frame
			if (videostore.frameFlags != 0) {
				videostore.waitForIframe = false;
				emit = true;
			}
			else if (!videostore.waitForIframe)
				emit = false;

			// check if all fragments
			for (int i = 0; i < videostore.rxFragmentCount; i++) {
				if (videostore.fragmentlens[i] <= 0) {  // not rxed, discard all
					printf("Found missing fragment at %d\n", i);
					emit = false;
					break;
				}
			}

			// 2. use the fragments
			if (emit){
			//	if (!isBusyVision){
					// merge frames
					int prevTotalLen = 0;
					totalLen = 0;
					for (int i = 0; i < videostore.rxFragmentCount; i++) {
						prevTotalLen = totalLen;
						totalLen += videostore.fragmentlens[i];
						if (totalLen > 128 * 1024) {
							std::cerr << "Too large frame : length =" << totalLen << std::endl;
							return false;
						}
						if (videostore.fragmentlens[i] <= 0) {
							std::cerr << "missing fragment: " << i << ":"
									<< videostore.rxFragmentCount << std::endl;
							return false;
						}
						memcpy(&mergedFrames[prevTotalLen], videostore.fragments[i], videostore.fragmentlens[i]);
					}

					isFrameMade = true;
			//	}
			}
		}

		// 3. setup the frame store
		memset(videostore.frameACK, 0, 16); // clear
		videostore.frameNumber = newFrame->frameNumber; // getting a new frame
		videostore.frameFlags = newFrame->frameFlags;
		for (int i = 0; i < 128; i++)
			videostore.fragmentlens[i] = 0;
		videostore.rxFragmentCount = 0;
	} // if new framenumber

	// 2. copy new frame
	memcpy(&videostore.fragments[newFrame->fragmentNumber], &data[5], size - 5);
	// mark the RXed fragments
		// each bit in the highPacketsAck and lowPacketsAck correspond to the
		// fragmentsPerFrame which have been received per frameNumber, so time to
		// flip some bits!
		//
		//
		// 0 1 2 3 4 5 6 7   0 1 2 3 4 5 6 7
		// *
		// fragmentNumber = 0
		// Sending time
		// 0 1 2 3 4 5 6 7  => 0 1 2 3 4 5 6 7
		//                     *

	int bufferPosition = newFrame->fragmentNumber / 8; // | 0;
	unsigned char tmp = videostore.frameACK[bufferPosition];
	tmp |= 1 << (newFrame->fragmentNumber % 8);
	videostore.frameACK[bufferPosition] = tmp;

	if (videostore.fragmentlens[newFrame->fragmentNumber] == 0)  // first arrival
		videostore.rxFragmentCount++;
	videostore.fragmentlens[newFrame->fragmentNumber] = size - 5;

	// 3. ACK
	sendVideoAck();

	return 0;
}



