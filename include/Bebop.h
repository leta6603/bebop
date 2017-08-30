/*
 * bebop.h
 *
 *  Created on: 2017. 8. 8.
 *      Author: lee
 */

#ifndef BEBOP_H_
#define BEBOP_H_

#ifndef EXTERN
#define EXTERN extern
#endif

//Pack memory
#pragma pack(push)
#pragma pack(1)
typedef
struct {
  unsigned char type;  	// 1B
  unsigned char id;   		// 1B
  unsigned char seq;   	// 1B
  unsigned int  size;  	// 4B
  unsigned char data[1024];
} frame_t;
#pragma  pack(pop)

// CMD Header 2
//Pack memory
#pragma pack(push)
#pragma pack(1)
typedef struct {
		unsigned char project;	// 1B
		unsigned char clazz;	// 1B
		unsigned short cmd;		// 2B
		unsigned char params[4];// dummy
} cmd_t;
#pragma  pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct navcmd_t{
   unsigned char flag; // Boolean flag to activate roll/pitch movement
   unsigned char roll; // roll Roll consign for the drone [-100;100]
   unsigned char pitch;// Pitch consign for the drone [-100;100]
   unsigned char yaw;  // yaw Yaw consign for the drone [-100;100]
   unsigned char gaz;  // gaz Gaz consign for the drone [-100;100]
   float psi;          //  [NOT USED] - Magnetic north heading of the
  		               //  controlling device (deg) [-180;180]
   navcmd_t()
   {
	   flag = 0;
	   roll = 0;
	   pitch = 0;
	   yaw = 0;
	   gaz = 0;
	   psi = 0;
   }
};
#pragma  pack(pop)

typedef enum
{
	LANDED,
	TAKINGOFF,
	HOVERING,
	FLYING,
	LANDING,
	EMERGENCY,
	MAX
} DRONE_STATE;

typedef enum {
	Takeoff = 0,
	Land, //1
	Stop,//2
	Emergency,//3
	Auto,//4
	Forward,//5
	Clockwise,//6
	Right,//7
	Manual,//8
	Backward,//9
	CounterClockwise,//10
	Left,//11
	Up,//12
	Down,//13
	ForceStop, //14
	ForceClockwise,
	ForceCounterClockwise,
	NONE//15
} NAV_CMD_TYPE;

double  calTime(timeval* start);

EXTERN unsigned char seq[256];
EXTERN bool isBusyVision;
int H264DecoderInit();
int H264DecoderClose();
void* decodeFrame(void* arg);
EXTERN DRONE_STATE mDroneState;

EXTERN volatile float speedX, speedY, speedZ;
EXTERN volatile NAV_CMD_TYPE mNavigationCommand;
EXTERN volatile unsigned char inputParam;
EXTERN int discSock, d2cSock, c2dSock;

ssize_t rio_writen(int fd, void *usrbuf, size_t n);
int onVideoFrame(unsigned char *data, int size);
int h264DecoderDecode(unsigned char *inbuf, int len, bool toSave);
int readData(void *usrbuf, size_t n);
bool mediaStreamingEnable(bool enable);

void controlDroneCmdGenerator(unsigned char _project, unsigned char _class, unsigned char _command);
void takeoff();
void land();
void emergency();

//Drone navigation command
navcmd_t upCMD(unsigned char val);
navcmd_t downCMD(unsigned char val);
navcmd_t forwardCMD(unsigned char val);
navcmd_t backwardCMD(unsigned char val);
navcmd_t rightCMD(unsigned char val);
navcmd_t leftCMD(unsigned char val);
navcmd_t clockwiseCMD(unsigned char val);
navcmd_t counterClockwiseCMD(unsigned char val);
void generateCMD(navcmd_t navcmd, unsigned char size, unsigned char *returnBuf);
void sendNavCmd(NAV_CMD_TYPE cmdType, unsigned char value);

void findAllState();
void flattrim();

//received data
void onD2CData();

#endif /* BEBOP_H_ */
