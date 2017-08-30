/*
 * bebopControl.cpp
 *
 *  Created on: 2017. 8. 10.
 *      Author: lee
 */

#include <errno.h>
#include <iostream>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "ARConstant.h"
#include "Bebop.h"

#define MAX_FRAME_SIZE 1025

extern FILE* fileGPS;
extern FILE* fileSpeed;
extern timeval* globalClock;

double latitude, longitude, altitude;

typedef enum
{
	FlyingStateChanged = 1,
	PositionStateChanged = 4,
	SpeedChanged = 5,
	AttitudeChanged = 6,
	AltitudeChanged = 8
} PILOTING_STATE;

float readFloat(unsigned char *buf){
	float returnValue = -1;
	memcpy(&returnValue, buf, 4);
	return returnValue;
}

double readDouble(unsigned char *buf){
	double returnValue = -1;
	memcpy(&returnValue, buf, 8);
	return returnValue;
}

ssize_t rio_writen(int fd, void *usrbuf, size_t n){
    size_t nleft = n;
    ssize_t nwritten;
    char *bufp = (char *)usrbuf;

    while (nleft > 0) {
	if ((nwritten = write(fd, bufp, nleft)) <= 0) {
	    if (errno == EINTR)  /* interrupted by sig handler return */
		nwritten = 0;    /* and call write() again */
	    else
		return -1;       /* errorno set by write() */
	}
	nleft -= nwritten;
	bufp += nwritten;
    }
    return n;
}

bool logGPSInfo(double latitude, double longitude){
	if(fileGPS == NULL){
		const char* GPS_FILE_NAME = "SaveData/gps.txt";
		fileGPS = fopen(GPS_FILE_NAME, "w");
		if(fileGPS == NULL){
			std::cout << "Can not open file" << std::endl;
			return false;
		}
	}

	if(fileGPS != NULL){
		fprintf(fileGPS, "%.6f	%.6f \n", latitude, longitude);
		fflush(fileGPS);
	}
	return true;
}

bool logSpeedInfo(float speedx, float speedy, float speedz){
	if(fileSpeed == NULL){
		const char* SPEED_FILE_NAME = "SaveData/speed.txt";
		fileSpeed = fopen(SPEED_FILE_NAME, "w");
		if(fileSpeed == NULL){
			std::cout << "Can not open file" << std::endl;
			return false;
		}
	}

	if(fileSpeed != NULL){
		//if(globalClock != NULL)
			fprintf(fileSpeed, "%f	%f	%f	%f \n", calTime(globalClock)/1000, speedx, speedy, speedz);
		//else
		//	fprintf(fileSpeed, "      	%.4f	%.4f	%.4f \n", speedX, speedY, speedZ);
		fflush(fileSpeed);
	}
	return true;
}
void onC2DEventframe(unsigned char *buf){

	unsigned char commandProject = *buf;
	unsigned char commandClass = *(buf+1);
	unsigned short commandId = *(buf+2);

	PILOTING_STATE state = (PILOTING_STATE)commandId;

	if(commandProject == 0x01)
	{
		if (commandClass == 0x04)			//KH. class=4?   ARConstant::ARCOMMANDS_ID_ARDRONE3_CLASS_PILOTINGSTATE = 4
		{
			//std::cout << "Pilot state" << std::endl;
			switch(state)
			{
			case FlyingStateChanged:
				//Read drone's state float
				mDroneState = (DRONE_STATE) *(buf+4);
				std::cout << "Drone status:" << mDroneState << std::endl;

				if(mDroneState == FLYING || mDroneState == HOVERING || mDroneState == TAKINGOFF)
				{
					std::cout << "Drone is flying:" << mDroneState << std::endl;
				}
				break;
			case PositionStateChanged:
				//Read latitude (double) longitude (double) altitude (double)
				latitude = readDouble(buf+4);
				longitude = readDouble(buf+12);
				altitude = readDouble(buf+20);
				//std::cout << "Latitude: " << latitude << std::endl;
				//std::cout << "Longitude: " << longitude << std::endl;

				//Don't write default value;
				//if(latitude != 500 && longitude != 500)
					logGPSInfo(latitude, longitude);
				//Write coordinate into file
				//Read value when latitude, longitude >= 0
				break;
			case SpeedChanged:
				//Read SpeedX (float) SpeedY(float) SpeedZ(float)
				speedX = readFloat(buf+4);
				speedY = readFloat(buf+8);
				speedZ = readFloat(buf+12);
				if(speedX != 500 && speedY != 500 && speedZ != 500)
					logSpeedInfo(speedX, speedY, speedZ);
				//std::cout << "Speed:" << speedX << "," << speedY << "," << speedZ << std::endl;
				break;
			/*
			case AttitudeChanged:
				//Read Roll (float) Pitch (float) Yaw (float)
				roll = readFloat(buf+4);
				pitch = readFloat(buf+8);
				yaw = readFloat(buf+12);

				//std::cout << "Roll: " << roll << std::endl;
				//std::cout << "Pitch: " << pitch << std::endl;
				//std::cout << "Yaw: " << yaw << std::endl;
				break;
			case AltitudeChanged:
				//Read Altitude (double)
				altitude = readDouble(buf+4);
				break;
			*/
			 default:
				break;
			}
		}
	}
}


/** --------------------------------------------------------------------------------------
  send simple ack frame to drone

  for which type frames?
------------------------------------------------------------------------------------------*/

static int sendAck(frame_t *pinframe){
	char txbuf[1024];

	frame_t *pAck = (frame_t *)txbuf;

	// 1. build frame
	pAck->type = ARConstant::ARNETWORKAL_FRAME_TYPE_ACK;
	pAck->id   = pinframe->id + (ARConstant::ARNETWORKAL_MANAGER_DEFAULT_ID_MAX / 2);
	pAck->seq  = seq[pAck->id]++;
	pAck->size = 7 + 1;
	pAck->data[0] = pinframe->seq;

	// 2. send
	rio_writen(c2dSock, txbuf, pAck->size);

	return 0;
}

/** --------------------------------------------------------------------------------------
     Keep-alive: reply ping with poing

------------------------------------------------------------------------------------------ */

static int sendPong(frame_t *pping){

	//char txbuf[1024];

	//frame_t *ppong = (frame_t *)txbuf;
	frame_t ppong;

	// 1. build frame
	ppong.type = ARConstant::ARNETWORKAL_FRAME_TYPE_DATA;
	ppong.id   = ARConstant::ARNETWORK_MANAGER_INTERNAL_BUFFER_ID_PONG;
	ppong.seq  = seq[pping->id]++;
	ppong.size = pping->size;  // must be same
	//ppong.data = new unsigned char[pping->size - 7 + 1];
	memcpy(ppong.data,pping->data, pping->size -7); // copy the payload

	// 2. send
	rio_writen(c2dSock, &ppong, ppong.size);

	//ppong.data = NULL;

	return 0;
}

int readData(int fd, void *usrbuf, size_t n)
{
    int nleft = n;
    int nread;
    unsigned char *bufp = (unsigned char *)usrbuf;

	int len = 0;
	bool bHasLen = false;

    while (nleft > 0) {
	if ((nread = read(fd, bufp, nleft)) < 0) {
	    if (errno == EINTR) /* interrupted by sig handler return */
			nread = 0;      /* and call read() again */
	    else
			return -1;      /* errno set by read() */
	}
	else if (nread == 0)
	    break;              /* EOF */

	nleft -= nread;

	if(n - nleft >= 7 && !bHasLen)
	{
		len = *(bufp +3) | (*(bufp +4) << 8) | (*(bufp +5) << 16) | (*(bufp +6) << 24) ;
		nleft += len;
		nleft -= n;
		bHasLen = true;
	}

	bufp += nread;
    }
    return len;         /* return >= 0 */
}

void flattrim(){
	unsigned char _project = ARConstant::ARCOMMANDS_ID_PROJECT_ARDRONE3;
	unsigned char _class = ARConstant::ARCOMMANDS_ID_ARDRONE3_CLASS_PILOTING;
	unsigned char _cmd = ARConstant::ARCOMMANDS_ID_ARDRONE3_PILOTING_CMD_FLATTRIM;

	//Flatrim
	controlDroneCmdGenerator(_project, _class, _cmd);
}

void takeoff(){
	unsigned char _project = ARConstant::ARCOMMANDS_ID_PROJECT_ARDRONE3;
	unsigned char _class = ARConstant::ARCOMMANDS_ID_ARDRONE3_CLASS_PILOTING;
	unsigned char _cmd = ARConstant::ARCOMMANDS_ID_ARDRONE3_PILOTING_CMD_TAKEOFF;

	//Take off
	controlDroneCmdGenerator(_project, _class, _cmd);
}
void findAllState(){
	unsigned char _project = ARConstant::ARCOMMANDS_ID_PROJECT_COMMON;
	unsigned char _class = ARConstant::ARCOMMANDS_ID_COMMON_CLASS_COMMON;
	unsigned char _cmd = ARConstant::ARCOMMANDS_ID_COMMON_COMMON_CMD_ALLSTATES;
	controlDroneCmdGenerator(_project,_class,_cmd);
}


void land(){
	unsigned char _project = ARConstant::ARCOMMANDS_ID_PROJECT_ARDRONE3;
	unsigned char _class = ARConstant::ARCOMMANDS_ID_ARDRONE3_CLASS_PILOTING;
	unsigned char _cmd = ARConstant::ARCOMMANDS_ID_ARDRONE3_PILOTING_CMD_LANDING;

	//Land
	controlDroneCmdGenerator(_project, _class, _cmd);
}

void emergency(){
	unsigned char _project = ARConstant::ARCOMMANDS_ID_PROJECT_ARDRONE3;
	unsigned char _class = ARConstant::ARCOMMANDS_ID_ARDRONE3_CLASS_PILOTING;
	unsigned char _cmd = ARConstant::ARCOMMANDS_ID_ARDRONE3_PILOTING_CMD_EMERGENCY;

	//Emergency mode
	controlDroneCmdGenerator(_project, _class, _cmd);
}

unsigned char validatePitch(unsigned char value){
	if(value > 100)
		return 100;
	else if (value < 0)
		return 0;
	return value | 0;
}

navcmd_t upCMD(unsigned char val){
	navcmd_t navcmd;
	navcmd.gaz = validatePitch(val);
	//navcmd.flag = 1;
	return navcmd;
}

navcmd_t downCMD(unsigned char val){
	navcmd_t navcmd;
	navcmd.gaz = validatePitch(val)*(-1);
	//navcmd.flag = 1;
	return navcmd;
}

navcmd_t forwardCMD(unsigned char val){
	navcmd_t navcmd;
	navcmd.pitch = validatePitch(val);
	navcmd.flag = 1;
	return navcmd;
}

navcmd_t backwardCMD(unsigned char val){
	navcmd_t navcmd;
	navcmd.pitch = validatePitch(val)*(-1);
	navcmd.flag = 1;
	return navcmd;
}

navcmd_t rightCMD(unsigned char val){
	navcmd_t navcmd;
	navcmd.roll = validatePitch(val);
	navcmd.flag = 1;
	return navcmd;
}

navcmd_t leftCMD(unsigned char val){
	navcmd_t navcmd;
	navcmd.roll = validatePitch(val)*(-1);
	navcmd.flag = 1;
	return navcmd;
}

navcmd_t clockwiseCMD(unsigned char val){
	navcmd_t navcmd;
	navcmd.yaw = validatePitch(val);
	//navcmd.flag = 1; 0x00 or 0x01???
	return navcmd;
}

navcmd_t counterClockwiseCMD(unsigned char val){
	navcmd_t navcmd;
	navcmd.yaw = validatePitch(val)*(-1);
	//navcmd.flag = 1; 0x00 or 0x01???
	return navcmd;
}


void controlDroneCmdGenerator(unsigned char _project, unsigned char _class, unsigned char _command){
	unsigned int size = 11;
	frame_t pframe;

	pframe.type = ARConstant::ARNETWORKAL_FRAME_TYPE_DATA; //pframe.type;
	pframe.id = ARConstant::BD_NET_CD_NONACK_ID; //pframe.id;
	pframe.seq = seq[ARConstant::BD_NET_CD_NONACK_ID]++; //pframe.seq;
	pframe.size = size;

	pframe.data[0] = _project;
	pframe.data[1] = _class;

	/*
	char hByte = (_command & 0xff00) >> 8;
	char lByte = (_command & 0xff);

	pframe.data[2] = hByte;
	pframe.data[3] = lByte;
	*/
	pframe.data[2] = _command;
	pframe.data[3] = 0;
	//2.Send
	rio_writen(c2dSock, &pframe, size);
	return;
}



void generateCMD(navcmd_t navcmd, unsigned char size, unsigned char *returnBuf){
	extern unsigned char seq[256];
    frame_t *pframe = (frame_t *)returnBuf;
    // basic header
    pframe->type = ARConstant::ARNETWORKAL_FRAME_TYPE_DATA;
    pframe->id = ARConstant::BD_NET_CD_NONACK_ID;
    pframe->seq = seq[ARConstant::BD_NET_CD_NONACK_ID]++;
    pframe->size = size;

    // cmd header
    cmd_t *pcmd = (cmd_t *)pframe->data;
    pcmd->project = ARConstant::ARCOMMANDS_ID_PROJECT_ARDRONE3;
    pcmd->clazz = ARConstant::ARCOMMANDS_ID_ARDRONE3_CLASS_PILOTING;
    pcmd->cmd = ARConstant::ARCOMMANDS_ID_ARDRONE3_PILOTING_CMD_PCMD;

    // params
    pcmd->params[0] = navcmd.flag;
    pcmd->params[1] = navcmd.roll;
    pcmd->params[2] = navcmd.pitch;
    pcmd->params[3] = navcmd.yaw;
    pcmd->params[4] = navcmd.gaz;

    memcpy(&pcmd->params[5],(void *)&navcmd.psi, sizeof(float)); // 4 bytes
    return;
}

void sendNavCmd(NAV_CMD_TYPE cmdType, unsigned char value){
	//
  // ARCOMMANDS_Generator_GenerateARDrone3PilotingPCMD
  //
  // uint8 - flag Boolean flag to activate roll/pitch movement
  // int8  - roll Roll consign for the drone [-100;100]
  // int8  - pitch Pitch consign for the drone [-100;100]
  // int8  - yaw Yaw consign for the drone [-100;100]
  // int8  - gaz Gaz consign for the drone [-100;100]
  // float - psi [NOT USED] - Magnetic north heading of the
  //         controlling device (deg) [-180;180]
  //
	navcmd_t navcmd;
	switch(cmdType)
	{
	case Up:
		navcmd = upCMD(value);
		//std::cout << "UP";

		break;
	case Down:
		navcmd = downCMD(value);
		break;
	case Forward:
		navcmd = forwardCMD(value);
		break;
	case Backward:
		navcmd = backwardCMD(value);
		break;
	case Right:
		navcmd = rightCMD(value);
		break;
	case Left:
		navcmd = leftCMD(value);
		break;
	case Clockwise:
	case ForceClockwise:
		navcmd = clockwiseCMD(value);
		break;
	case CounterClockwise:
	case ForceCounterClockwise:
		navcmd = counterClockwiseCMD(value);
		//std::cout << "COUNTERCLOCKWISE: ";
		break;
	case Takeoff:
		takeoff();
		//std::cout<< "Take off ";
		return;
	case Land:
		land();
		return;
	case Emergency:
		emergency();
		return;
	default:
		//std::cout << "STOP: ";
		break;
	}
	//Generate command
	unsigned char commandSize = 20;
	unsigned char buf[commandSize + 1];
	generateCMD(navcmd, commandSize, buf);

	//Send command
	int n = rio_writen(c2dSock, buf, commandSize);
	if(n < 0){
		std::cerr << "sendNavCmd write error" << std::endl;
	}

	if(cmdType == ForceClockwise || cmdType == ForceCounterClockwise)
		mNavigationCommand = Stop;

  return;
}

unsigned char rxBuff[MAX_FRAME_SIZE];
void onD2CData(){
	//std::cout << "On D2C data received." << std::endl;

	//Read data and return len of message
	int len = readData(d2cSock, (void *)&rxBuff, MAX_FRAME_SIZE);  // 7 = header size

	//Can not read data
	if(len <= 0)
	{
		return ;
	}

	//Copy data to frame variable
	frame_t pframe;
	memcpy(&pframe, &rxBuff, 7);
	memcpy(pframe.data, &rxBuff[7], len - 7);
	pframe.data[len+1] = 0;

	if (pframe.type == ARConstant::ARNETWORKAL_FRAME_TYPE_DATA_LOW_LATENCY
	 && pframe.id == ARConstant::BD_NET_DC_VIDEO_DATA_ID) {
		std::cout << "rx : VideoFrame" << std::endl;
		//onVideoFrame(&rxBuff[7], len - 7);
	}

	if (pframe.type ==  ARConstant::ARNETWORKAL_FRAME_TYPE_DATA_WITH_ACK){ // 4,
		sendAck(&pframe);
	}

	/*
	if (pframe.id == ARConstant::BD_NET_DC_EVENT_ID
	 || pframe.id == ARConstant::BD_NET_DC_NAVDATA_ID){
		std::cout << "rx : event frame" << std::endl;
		//[Tuan] Check existing data
		if(pframe.size > 0)
			onC2DEventframe(pframe.data);
	}
	*/
	if(pframe.id == ARConstant::ARNETWORK_MANAGER_INTERNAL_BUFFER_ID_PING)
	{
		unsigned int *p = (unsigned int*)(&pframe.data);
		sendPong(&pframe);
		p = NULL;
		delete []p;
	}
}

