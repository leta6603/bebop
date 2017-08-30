/*
 * bebop.cpp
 *
 *  Created on: 2017. 7. 26.
 *      Author: lee
 */

#include <iostream>
#include <sys/time.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <pthread.h>

#define EXTERN
#include "Bebop.h"
#include "streamWindow.h"
//extern variables
FILE* videoDecode;//videoDecode.txt // Save the video decode time
FILE* visionProcessFile;//visionProcess.txt // Save the time of Video decoding, Canndy detection, Hough line, Vanishing , Tracking, SendCMD2Control
FILE* lineSegmentFile;//lineSegment.txt
FILE* visionFile;//resultVision.txt
//Logs GPS
FILE* fileGPS;
FILE* fileSpeed;
timeval* globalClock;

//Variables
const char* SAVE_DATA_PATH = "SaveData";
const char* GPS_FILE_NAME = "SaveData/gps.txt";
const char* SPEED_FILE_NAME = "SaveData/speed.txt";
const char* LINESEGMENT_FILE_NAME = "SaveData/resultVision.txt";
const char* VISION_PROCESS_FILE_NAME = "SaveData/visionProcess.txt";
const char* VIDEO_DECODE_FILE_NAME = "SaveData/videoDecode.txt";



//static variable
volatile static bool isConnectionRunning;
volatile static bool isConnected;

//Declaration of functions
static int discoverDrone(const char* destIP);
double calculatePeriodOfTime(timeval startTime);
static void* laneKeepingThreadStart(void* arg);
int setupd2cSocket(short controlPort);
int setupc2dSocket(const char* droneIP, int dronePort);

int main(int argc, char *argv[]){
	bool isRunning = true;
	isConnected = false;
	mNavigationCommand = Stop;
	inputParam = 5;

	double controlPeriod;
	timeval sendCmd;
	pthread_t laneKeeping;
	SDL_Event event;
	init();

	while(isRunning){
		if(!isConnected){
			//TODO : I failed to make timeout part. try if you can.
			if(!isConnectionRunning){
				pthread_create(&laneKeeping, NULL, &laneKeepingThreadStart, NULL);
				gettimeofday(&sendCmd, 0);
				isConnectionRunning = true;
			}
		}
		else{
			if(SDL_PollEvent(&event)){
				eventHandler(event);
				if(event.type == SDL_QUIT){
					//TODO : send land CMD and then quit
					//test what happens if connection lost during drone is flying
					isRunning = false;
					break;
				}
			}
			controlPeriod = calculatePeriodOfTime(sendCmd);
			if(controlPeriod <0){
				std::cout << "calTime Error" << std::endl;
			}
			else if(controlPeriod>=25){
			sendNavCmd(mNavigationCommand, inputParam);
			gettimeofday(&sendCmd, 0);
			}
		}
	}
	return 0;
}

static int discoverDrone(const char* destIP){
// make a TCP connection to drone and send json message
	short destPort = 44444;
	struct hostent* hp;
	struct sockaddr_in droneIP;

	int clientSock;
	clientSock = socket(PF_INET, SOCK_STREAM, 0);
	if(clientSock == -1){
		std::cerr << "disc socket socket() error" << std::endl;
		return -2;
	}

	memset(&droneIP, 0, sizeof(droneIP));
	if(inet_aton(destIP, &(droneIP.sin_addr)) < 0){
		std::cerr << "disc socket inet_aton() error" << std::endl;
	}
	droneIP.sin_port = htons(destPort);
	droneIP.sin_family = AF_INET;

	if (connect(clientSock, (struct sockaddr*) &droneIP, sizeof(struct sockaddr_in)) < 0){
		std::cerr << "disc socket connect() error" << std::endl;
		close(clientSock);
		return -3;
	}

	const char *jsoninfo = "{\"controller_type\":\"computer\",\"controller_name\":\"node-bebop\",\"d2c_port\":\"43210\"}";
	int n = strlen(jsoninfo);

	if(write(clientSock, jsoninfo, n)<0){
		std::cerr << "jsoninfo write error" << std::endl;
		close(clientSock);
		return -4;
	}
	return clientSock;
}

double calculatePeriodOfTime(timeval startTime) //return ms
{
	timeval currentTime;
	gettimeofday(&currentTime, 0);
	return (currentTime.tv_sec + 0.000001*currentTime.tv_usec - startTime.tv_sec - 0.000001*startTime.tv_usec)*1000;
}


double  calTime(timeval* start){
	timeval* curr = NULL;
	gettimeofday(curr, NULL);

	return (curr->tv_sec + 0.000001 * curr->tv_usec - start->tv_sec - 0.000001 * start->tv_usec);
}

int setupd2cSocket(short controlPort){
	int sock = socket(AF_INET, SOCK_DGRAM, 0);
	if(socket < 0){
		std::cerr << "d2csocket socket() error" << std::endl;
		return -1;
	}

	struct sockaddr_in myaddr;
	memset(&myaddr, 0, sizeof(myaddr));
	myaddr.sin_family = AF_INET;
	myaddr.sin_port = htons(controlPort);
	myaddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if(bind(sock, (struct sockaddr*)&myaddr, sizeof(myaddr)) < 0){
		std::cerr << "d2csocket bind() error" << std::endl;
		return -2;
	}
	return sock;
}

int setupc2dSocket(const char* droneIP, int dronePort){
	int sock = socket(AF_INET, SOCK_DGRAM, 0);
	if(socket < 0){
		std::cerr << "c2dsocket socket() error" << std::endl;
		return -1;
	}

	struct sockaddr_in droneAddr;
	memset(&droneAddr, 0, sizeof(droneAddr));
	droneAddr.sin_family = AF_INET;
	droneAddr.sin_port = htons(dronePort);
	if(inet_aton(droneIP, &droneAddr.sin_addr) == 0){
		std::cerr << "drone ip addr translation error" << std::endl;
		close(sock);
		return -2;
	}

	if(connect(sock, (struct sockaddr*)&droneAddr, sizeof(droneAddr))<0){
		std::cerr << "c2dsocket connect() error" << std::endl;
		return -3;
	}

	return sock;
}

static void* laneKeepingThreadStart(void* arg){
	struct stat st = {0};
	const char* destIP = "192.168.42.1";
	//TODO : erase controlerIP if don't need it
	//const char* controlerIP = "192.168.42.2";
	pthread_t decoder;

	discSock = discoverDrone(destIP);
	if(discSock < 0){
		std::cerr << "discovery failed" << std::endl;
	}
	else
		std::cout << "discovery success" << std::endl;

	if (stat(SAVE_DATA_PATH, &st) == -1)
		mkdir(SAVE_DATA_PATH, 0700);
	fileGPS = fopen(GPS_FILE_NAME, "w"); // w for deleting all previous contents.
	if(fileGPS == NULL){
		std::cout << "Can not open file" << std::endl;
		return 0;
	}
	else
		fprintf(fileGPS, " Latitude	 Longitude\n");
        //Save the execute time of Canny, houghLine, Ransac, tracking
	visionProcessFile = fopen(VISION_PROCESS_FILE_NAME, "w");
	if(visionProcessFile == NULL){
		std::cout << "Can not open result vision process file" << std::endl;
		return 0;
	}
	else
		fprintf(visionProcessFile, "   No Edge    Hough   RANSAC  TRACKING    SendCMD\n");
	videoDecode = fopen(VIDEO_DECODE_FILE_NAME, "w");
	if(videoDecode == NULL){
		std::cout << "Can not open result video decode process file" << std::endl;
		return 0;
	}
	else
		fprintf(videoDecode, "Decode Time\n");

	visionFile = fopen("SaveData/resultVision.txt", "w"); // w for deleting all previous contents.
	if(visionFile == NULL){
		std::cout << "Can not open result vision file" << std::endl;
		return 0;
	}
	else
		fprintf(visionFile, "No	Left	Right	Pos	Cmd	Ptime	RCmd	Angle   Speed\n");

	fileSpeed = fopen(SPEED_FILE_NAME, "w");
	if(fileSpeed == NULL){
		std::cout << "Can not open file" << std::endl;
		return 0;
	}
	else
		fprintf(fileSpeed, "Time	SpeedX	SpeedY	SpeedZ\n");

	//Print line segment info
	lineSegmentFile = fopen(LINESEGMENT_FILE_NAME, "w"); // w for deleting all previous contents.
	if(lineSegmentFile == NULL){
		std::cout << "Can not open line segment file" << std::endl;
		return 0;
	}
	else
		fprintf(lineSegmentFile, " Frame	LineId	Point1		Point2	Intersection2Bottom\n");

	bool isStreaming;
	while(isConnectionRunning){
		if(!isConnected){
			// 1. check discovery data
			char buftemp[1024];
			std::cout << "discovery Acked"  << std::endl;
			int n = read(discSock, (void*) buftemp, 1024);
			buftemp[n] = 0;
			std::cout << buftemp << std::endl;

			std::cout << "connected" << std::endl;

			// 2. make UDP connections (Upstream, Downstream)
			d2cSock = setupd2cSocket(43210); // my port
			c2dSock = setupc2dSocket(destIP, 54321); // drone ip, port

			close(discSock);
			findAllState();
			flattrim();
			if(!isStreaming){
				isStreaming = true;
				mediaStreamingEnable(isStreaming);
				pthread_create(&decoder, NULL, &decodeFrame, NULL);
			}
			isConnected = true;
		}
		else{
			//TODO : receive messages, process received data and send response messages
			onD2CData();
		}
	}
}



