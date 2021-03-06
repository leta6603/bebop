#ifndef ARCONSTANT_H
#define ARCONSTANT_H
// libARNetworkAL/Includes/libARNetworkAL/ARNETWORKAL_Manager.h
class ARConstant{

public:

static const int

  ARNETWORKAL_MANAGER_DEFAULT_ID_MAX,

  // ARNETWORKAL_Frame_t identifiers
  BD_NET_CD_NONACK_ID,
  BD_NET_CD_ACK_ID,
  BD_NET_CD_EMERGENCY_ID,
  BD_NET_CD_VIDEO_ACK_ID,
  BD_NET_DC_VIDEO_DATA_ID,
  BD_NET_DC_EVENT_ID,
  BD_NET_DC_NAVDATA_ID,

  // eARCOMMANDS_ID_PROJECT
  ARCOMMANDS_ID_PROJECT_COMMON,
  ARCOMMANDS_ID_PROJECT_ARDRONE3,

  // eARCOMMANDS_ID_ARDRONE3_CLASS
  ARCOMMANDS_ID_ARDRONE3_CLASS_PILOTING,
  ARCOMMANDS_ID_ARDRONE3_CLASS_ANIMATIONS,
  ARCOMMANDS_ID_ARDRONE3_CLASS_CAMERA,
  ARCOMMANDS_ID_ARDRONE3_CLASS_MEDIARECORD,
  ARCOMMANDS_ID_ARDRONE3_CLASS_MEDIARECORDSTATE,
  ARCOMMANDS_ID_ARDRONE3_CLASS_MEDIARECORDEVENT,
  ARCOMMANDS_ID_ARDRONE3_CLASS_PILOTINGSTATE,
  ARCOMMANDS_ID_ARDRONE3_CLASS_NETWORK,
  ARCOMMANDS_ID_ARDRONE3_CLASS_NETWORKSTATE,
  ARCOMMANDS_ID_ARDRONE3_CLASS_PILOTINGSETTINGS,
  ARCOMMANDS_ID_ARDRONE3_CLASS_PILOTINGSETTINGSSTATE,
  ARCOMMANDS_ID_ARDRONE3_CLASS_SPEEDSETTINGS,
  ARCOMMANDS_ID_ARDRONE3_CLASS_SPEEDSETTINGSSTATE,
  ARCOMMANDS_ID_ARDRONE3_CLASS_NETWORKSETTINGS,
  ARCOMMANDS_ID_ARDRONE3_CLASS_NETWORKSETTINGSSTATE,
  ARCOMMANDS_ID_ARDRONE3_CLASS_SETTINGS,
  ARCOMMANDS_ID_ARDRONE3_CLASS_SETTINGSSTATE,
  ARCOMMANDS_ID_ARDRONE3_CLASS_DIRECTORMODE,
  ARCOMMANDS_ID_ARDRONE3_CLASS_DIRECTORMODESTATE,
  ARCOMMANDS_ID_ARDRONE3_CLASS_PICTURESETTINGS,
  ARCOMMANDS_ID_ARDRONE3_CLASS_PICTURESETTINGSSTATE,
  ARCOMMANDS_ID_ARDRONE3_CLASS_MEDIASTREAMING,
  ARCOMMANDS_ID_ARDRONE3_CLASS_MEDIASTREAMINGSTATE,
  ARCOMMANDS_ID_ARDRONE3_CLASS_GPSSETTINGS,
  ARCOMMANDS_ID_ARDRONE3_CLASS_GPSSETTINGSSTATE,
  ARCOMMANDS_ID_ARDRONE3_CLASS_CAMERASTATE,
  ARCOMMANDS_ID_ARDRONE3_CLASS_ANTIFLICKERING,
  ARCOMMANDS_ID_ARDRONE3_CLASS_ANTIFLICKERINGSTATE,

  // eARCOMMANDS_ID_ARDRONE3_PILOTINGSTATE_CMD
  ARCOMMANDS_ID_ARDRONE3_PILOTINGSTATE_CMD_FLATTRIMCHANGED,
  ARCOMMANDS_ID_ARDRONE3_PILOTINGSTATE_CMD_FLYINGSTATECHANGED,
  ARCOMMANDS_ID_ARDRONE3_PILOTINGSTATE_CMD_ALERTSTATECHANGED,
  ARCOMMANDS_ID_ARDRONE3_PILOTINGSTATE_CMD_NAVIGATEHOMESTATECHANGED,
  ARCOMMANDS_ID_ARDRONE3_PILOTINGSTATE_CMD_POSITIONCHANGED,
  ARCOMMANDS_ID_ARDRONE3_PILOTINGSTATE_CMD_SPEEDCHANGED,
  ARCOMMANDS_ID_ARDRONE3_PILOTINGSTATE_CMD_ATTITUDECHANGED,
  ARCOMMANDS_ID_ARDRONE3_PILOTINGSTATE_CMD_AUTOTAKEOFFMODECHANGED,
  ARCOMMANDS_ID_ARDRONE3_PILOTINGSTATE_CMD_ALTITUDECHANGED,
  ARCOMMANDS_ID_ARDRONE3_PILOTINGSTATE_CMD_MAX,

  // eARCOMMANDS_ID_ARDRONE3_ANIMATIONS_CMD;
  ARCOMMANDS_ID_ARDRONE3_ANIMATIONS_CMD_FLIP,
  ARCOMMANDS_ID_ARDRONE3_ANIMATIONS_CMD_MAX,

  // eARCOMMANDS_ARDRONE3_PILOTINGSTATE_FLYINGSTATECHANGED_STATE;
  ARCOMMANDS_ARDRONE3_PILOTINGSTATE_FLYINGSTATECHANGED_STATE_LANDED,
  ARCOMMANDS_ARDRONE3_PILOTINGSTATE_FLYINGSTATECHANGED_STATE_TAKINGOFF,
  ARCOMMANDS_ARDRONE3_PILOTINGSTATE_FLYINGSTATECHANGED_STATE_HOVERING,
  ARCOMMANDS_ARDRONE3_PILOTINGSTATE_FLYINGSTATECHANGED_STATE_FLYING,
  ARCOMMANDS_ARDRONE3_PILOTINGSTATE_FLYINGSTATECHANGED_STATE_LANDING,
  ARCOMMANDS_ARDRONE3_PILOTINGSTATE_FLYINGSTATECHANGED_STATE_EMERGENCY,
  ARCOMMANDS_ARDRONE3_PILOTINGSTATE_FLYINGSTATECHANGED_STATE_MAX,

  // eARCOMMANDS_ARDRONE3_ANIMATIONS_FLIP_DIRECTION;
  ARCOMMANDS_ARDRONE3_ANIMATIONS_FLIP_DIRECTION_FRONT,
  ARCOMMANDS_ARDRONE3_ANIMATIONS_FLIP_DIRECTION_BACK,
  ARCOMMANDS_ARDRONE3_ANIMATIONS_FLIP_DIRECTION_RIGHT,
  ARCOMMANDS_ARDRONE3_ANIMATIONS_FLIP_DIRECTION_LEFT,
  ARCOMMANDS_ARDRONE3_ANIMATIONS_FLIP_DIRECTION_MAX,

  // eARCOMMANDS_ID_COMMON_CLASS
  ARCOMMANDS_ID_COMMON_CLASS_NETWORK,
  ARCOMMANDS_ID_COMMON_CLASS_NETWORKEVENT,
  ARCOMMANDS_ID_COMMON_CLASS_SETTINGS,
  ARCOMMANDS_ID_COMMON_CLASS_SETTINGSSTATE,
  ARCOMMANDS_ID_COMMON_CLASS_COMMON,
  ARCOMMANDS_ID_COMMON_CLASS_COMMONSTATE,
  ARCOMMANDS_ID_COMMON_CLASS_OVERHEAT,
  ARCOMMANDS_ID_COMMON_CLASS_OVERHEATSTATE,
  ARCOMMANDS_ID_COMMON_CLASS_CONTROLLERSTATE,
  ARCOMMANDS_ID_COMMON_CLASS_WIFISETTINGS,
  ARCOMMANDS_ID_COMMON_CLASS_WIFISETTINGSSTATE,
  ARCOMMANDS_ID_COMMON_CLASS_MAVLINK,
  ARCOMMANDS_ID_COMMON_CLASS_MAVLINKSTATE,
  ARCOMMANDS_ID_COMMON_CLASS_CALIBRATION,
  ARCOMMANDS_ID_COMMON_CLASS_CALIBRATIONSTATE,
  ARCOMMANDS_ID_COMMON_CLASS_CAMERASETTINGSSTATE,
  ARCOMMANDS_ID_COMMON_CLASS_GPS,
  ARCOMMANDS_ID_COMMON_CLASS_FLIGHTPLANSTATE,
  ARCOMMANDS_ID_COMMON_CLASS_FLIGHTPLANEVENT,
  ARCOMMANDS_ID_COMMON_CLASS_ARLIBSVERSIONSSTATE,

  // eARCOMMANDS_ID_ARDRONE3_PILOTING_CMD
  ARCOMMANDS_ID_ARDRONE3_PILOTING_CMD_FLATTRIM,
  ARCOMMANDS_ID_ARDRONE3_PILOTING_CMD_TAKEOFF,
  ARCOMMANDS_ID_ARDRONE3_PILOTING_CMD_PCMD,
  ARCOMMANDS_ID_ARDRONE3_PILOTING_CMD_LANDING,
  ARCOMMANDS_ID_ARDRONE3_PILOTING_CMD_EMERGENCY,
  ARCOMMANDS_ID_ARDRONE3_PILOTING_CMD_NAVIGATEHOME,
  ARCOMMANDS_ID_ARDRONE3_PILOTING_CMD_AUTOTAKEOFFMODE,
  ARCOMMANDS_ID_ARDRONE3_PILOTING_CMD_MAX,

  // eARCOMMANDS_ID_ARDRONE3_MEDIARECORD_CMD
  ARCOMMANDS_ID_ARDRONE3_MEDIARECORD_CMD_PICTURE,
  ARCOMMANDS_ID_ARDRONE3_MEDIARECORD_CMD_VIDEO,
  ARCOMMANDS_ID_ARDRONE3_MEDIARECORD_CMD_PICTUREV2,
  ARCOMMANDS_ID_ARDRONE3_MEDIARECORD_CMD_VIDEOV2,
  ARCOMMANDS_ID_ARDRONE3_MEDIARECORD_CMD_MAX,

  // eARCOMMANDS_ARDRONE3_MEDIARECORD_VIDEO_RECORD
  ARCOMMANDS_ARDRONE3_MEDIARECORD_VIDEO_RECORD_STOP,
  ARCOMMANDS_ARDRONE3_MEDIARECORD_VIDEO_RECORD_START,
  ARCOMMANDS_ARDRONE3_MEDIARECORD_VIDEO_RECORD_MAX,

  // eARCOMMANDS_ID_COMMON_COMMON_CMD
  ARCOMMANDS_ID_COMMON_COMMON_CMD_ALLSTATES,
  ARCOMMANDS_ID_COMMON_COMMON_CMD_CURRENTDATE,
  ARCOMMANDS_ID_COMMON_COMMON_CMD_CURRENTTIME,
  ARCOMMANDS_ID_COMMON_COMMON_CMD_REBOOT,
  ARCOMMANDS_ID_COMMON_COMMON_CMD_MAX,

  // eARCOMMANDS_ID_COMMON_COMMONSTATE_CMD;
  ARCOMMANDS_ID_COMMON_COMMONSTATE_CMD_ALLSTATESCHANGED,
  ARCOMMANDS_ID_COMMON_COMMONSTATE_CMD_BATTERYSTATECHANGED,
  ARCOMMANDS_ID_COMMON_COMMONSTATE_CMD_MASSSTORAGESTATELISTCHANGED,
  ARCOMMANDS_ID_COMMON_COMMONSTATE_CMD_MASSSTORAGEINFOSTATELISTCHANGED,
  ARCOMMANDS_ID_COMMON_COMMONSTATE_CMD_CURRENTDATECHANGED,
  ARCOMMANDS_ID_COMMON_COMMONSTATE_CMD_CURRENTTIMECHANGED,
  ARCOMMANDS_ID_COMMON_COMMONSTATE_CMD_MASSSTORAGEINFOREMAININGLISTCHANGED,
  ARCOMMANDS_ID_COMMON_COMMONSTATE_CMD_WIFISIGNALCHANGED,
  ARCOMMANDS_ID_COMMON_COMMONSTATE_CMD_SENSORSSTATESLISTCHANGED,
  ARCOMMANDS_ID_COMMON_COMMONSTATE_CMD_MAX,

  // eARMEDIA_ENCAPSULER_CODEC
  CODEC_UNKNNOWN,
  CODEC_VLIB,
  CODEC_P264,
  CODEC_MPEG4_VISUAL,
  CODEC_MPEG4_AVC,
  CODEC_MOTION_JPEG,

  // eARMEDIA_ENCAPSULER_FRAME_TYPE;
  ARMEDIA_ENCAPSULER_FRAME_TYPE_UNKNNOWN,
  ARMEDIA_ENCAPSULER_FRAME_TYPE_I_FRAME,
  ARMEDIA_ENCAPSULER_FRAME_TYPE_P_FRAME,
  ARMEDIA_ENCAPSULER_FRAME_TYPE_JPEG,
  ARMEDIA_ENCAPSULER_FRAME_TYPE_MAX,

  // eARNETWORK_MANAGER_INTERNAL_BUFFER_ID
  ARNETWORK_MANAGER_INTERNAL_BUFFER_ID_PING,
  ARNETWORK_MANAGER_INTERNAL_BUFFER_ID_PONG,
  ARNETWORK_MANAGER_INTERNAL_BUFFER_ID_MAX,

  // eARNETWORKAL_FRAME_TYPE
  ARNETWORKAL_FRAME_TYPE_UNINITIALIZED,
  ARNETWORKAL_FRAME_TYPE_ACK,
  ARNETWORKAL_FRAME_TYPE_DATA,
  ARNETWORKAL_FRAME_TYPE_DATA_LOW_LATENCY,
  ARNETWORKAL_FRAME_TYPE_DATA_WITH_ACK,
  ARNETWORKAL_FRAME_TYPE_MAX;
};

#endif
