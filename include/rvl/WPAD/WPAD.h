#ifndef RVL_SDK_WPAD_H
#define RVL_SDK_WPAD_H

// From
// https://github.com/muff1n1634/wpad_11dec2009/blob/3c3aa5416d15ccc5867517d9ed4aaf6d25a70e9c/include/revolution/WPAD/WPAD.h

#include <common.h>
#include <rvl/SC/scsystem.h>
#include <rvl/WPAD/WUD.h>


/*******************************************************************************
 * macros
 */

#define WPAD_MAX_DPD_OBJECTS 4

#define WPAD_MIN_DPD_SENS 1
#define WPAD_MAX_DPD_SENS 5

/*******************************************************************************
 * types
 */

#ifdef __cplusplus
extern "C" {
#endif

// for ease of use in ternaries
typedef s32 WPADResult;
enum WPADResult_et {
    WPAD_ERR_OK = 0,

    WPAD_ERR_NO_CONTROLLER = -1,       /* name known from asserts */
    WPAD_ERR_COMMUNICATION_ERROR = -2, // [RT3P54] has this as WPAD_ERR_BUSY
    WPAD_ERR_3 = -3,                   // [RT3P54] has this as WPAD_ERR_TRANSFER
    WPAD_ERR_INVALID = -4,             /* name comes from [R89JEL] */
                                       //	WPAD_ERR_5						= -5,	/* unknown */
                                       //	WPAD_ERR_6						= -6,	/* unknown */
    WPAD_ERR_CORRUPTED = -7,           /* name comes from [R89JEL] */

#define WPAD_ESUCCESS WPAD_ERR_OK
#define WPAD_ENODEV WPAD_ERR_NO_CONTROLLER
#define WPAD_ECOMM WPAD_ERR_COMMUNICATION_ERROR
// #define WPAD_E3
#define WPAD_EINVAL WPAD_ERR_INVALID

/* maybe? i was scouring the internet for explanation and usage of this error
 * macro, and it seems close enough
 *
 * it's not ENODATA, that's more like if you got no data, but here it's that we
 * did get the data but it wasn't good
 */
#define WPAD_EBADE WPAD_ERR_CORRUPTED

// apparently enum vs literal is a thing. cool
#define WPAD_CESUCCESS (WPAD_ESUCCESS + 0)
#define WPAD_CENODEV (WPAD_ENODEV + 0)
#define WPAD_CECOMM (WPAD_ECOMM + 0)
#define WPAD_CERR_3 (WPAD_ERR_3 + 0)
#define WPAD_CEINVAL (WPAD_EINVAL + 0)
#define WPAD_CEBADE (WPAD_EBADE + 0)
};

typedef WUDLibStatus WPADLibStatus;
enum WPADLibStatus_et {
    WPAD_LIB_STATUS_0 = WUD_LIB_STATUS_0,
    WPAD_LIB_STATUS_1 = WUD_LIB_STATUS_1,
    WPAD_LIB_STATUS_2 = WUD_LIB_STATUS_2,
    WPAD_LIB_STATUS_3 = WUD_LIB_STATUS_3,
    WPAD_LIB_STATUS_4 = WUD_LIB_STATUS_4,
};

// for ease of use in for loops
typedef s32 WPADChannel;
enum WPADChannel_et {
    WPAD_CHAN0,
    WPAD_CHAN1,
    WPAD_CHAN2,
    WPAD_CHAN3, /* name known from asserts */

    WPAD_MAX_CONTROLLERS, /* name known from asserts */
    WPAD_CHAN_INVALID = -1,
};

typedef s32 WPADDeviceType;
enum WPADDeviceType_et {
    WPAD_DEV_CORE = 0,
    WPAD_DEV_FS = 1,
    WPAD_DEV_CLASSIC = 2, /* name known from asserts, but not value */
    WPAD_DEV_BALANCE_CHECKER = 3,
    WPAD_DEV_VSM = 4,

    WPAD_DEV_MOTION_PLUS = 5,
    WPAD_DEV_MPLS_PT_FS = 6,
    WPAD_DEV_MPLS_PT_CLASSIC = 7,

    WPAD_DEV_TRAIN = 16,
    WPAD_DEV_GUITAR = 17,
    WPAD_DEV_DRUM = 18,
    WPAD_DEV_TAIKO = 19,
    WPAD_DEV_TURNTABLE = 20,

    // seems to be like maybe general purpose non-specific device types
    // maybe this was for testing or something? idk
    WPAD_DEV_BULK_1 = 21,
    WPAD_DEV_BULK_2 = 22,
    WPAD_DEV_BULK_3 = 23,
    WPAD_DEV_BULK_4 = 24,
    WPAD_DEV_BULK_5 = 25,
    WPAD_DEV_BULK_6 = 26,
    WPAD_DEV_BULK_7 = 27,
    WPAD_DEV_BULK_8 = 28,

    WPAD_DEV_MPLS_PT_UNKNOWN = 250,

    WPAD_DEV_251 = 251,
    WPAD_DEV_252 = 252, // invalid device mode?
    WPAD_DEV_NONE =
        253,                     // sort of like WPAD_ENODEV (see __wpadAbortInitExtension in WPADHIDParser.c)
                                 //	WPAD_DEV_254				= 254,	/* unknown, doesn't seem to be used anywhere */
    WPAD_DEV_INITIALIZING = 255, // see __a1_20_status_report
};

typedef u32 WPADDataFormat;
enum WPADDataFormat_et {
    // see WPADiSendSetReportType and __wpadFmt2Size
    WPAD_FMT_CORE_BTN = 0,
    WPAD_FMT_CORE_BTN_ACC = 1,
    WPAD_FMT_CORE_BTN_ACC_DPD = 2,

    WPAD_FMT_FS_BTN = 3,
    WPAD_FMT_FS_BTN_ACC = 4,
    WPAD_FMT_FS_BTN_ACC_DPD = 5,

    WPAD_FMT_CLASSIC_BTN = 6,
    WPAD_FMT_CLASSIC_BTN_ACC = 7,
    WPAD_FMT_CLASSIC_BTN_ACC_DPD = 8,

    WPAD_FMT_BTN_ACC_DPD_EXTENDED = 9,

    // extensions
    WPAD_FMT_TRAIN = 10,           /* name known from asserts */
    WPAD_FMT_GUITAR = 11,          /* name known from asserts */
    WPAD_FMT_BALANCE_CHECKER = 12, /* name known from asserts */
    WPAD_FMT_VSM = 13,             /* name known from asserts */
                                   //	WPAD_FMT_14						= 14,	/* unknown */
    WPAD_FMT_DRUM = 15,            /* name known from asserts */
    WPAD_FMT_MOTION_PLUS = 16,     // see __wpadFmt2Size
    WPAD_FMT_TAIKO = 17,           /* name known from asserts */
    WPAD_FMT_TURNTABLE = 18,       /* name known from asserts */
    WPAD_FMT_BULK = 19,            /* name known from asserts */
};

typedef s32 WPADDeviceMode;
enum WPADDeviceMode_et {
    WPAD_DEV_MODE_NORMAL = 0,

    WPAD_DEV_MODE_CLASSIC_REDUCED = 1,
    WPAD_DEV_MODE_CLASSIC_EXTENDED = 2,
    WPAD_DEV_MODE_CLASSIC_STANDARD = 3,

    WPAD_DEV_MODE_MPLS_PT_MAIN = 4,
    WPAD_DEV_MODE_MPLS_PT_FS = 5,
    //	WPAD_DEV_MODE_6					= 6,	/* unknown */
    WPAD_DEV_MODE_MPLS_PT_CLASSIC = 7,
};

// names from wiiuse/wpad.h
typedef u16 WPADButton;
enum WPADButton_et {
    // H..-AB12 ...+^v><
    /* NOTE: the bytes from the report are swapped when being placed into this
     * format (see MAKE_BUTTON in WPADHIDParser.c)
     */
    WPAD_BUTTON_LEFT = (1 << 0),
    WPAD_BUTTON_RIGHT = (1 << 1),
    WPAD_BUTTON_DOWN = (1 << 2),
    WPAD_BUTTON_UP = (1 << 3),
    WPAD_BUTTON_PLUS = (1 << 4),
    //					  (1 <<  5),
    //					  (1 <<  6),
    //					  (1 <<  7),
    WPAD_BUTTON_2 = (1 << 8),
    WPAD_BUTTON_1 = (1 << 9),
    WPAD_BUTTON_B = (1 << 10),
    WPAD_BUTTON_A = (1 << 11),
    WPAD_BUTTON_MINUS = (1 << 12),
    //					  (1 << 13),	// WPAD_BUTTON_FS_Z
    //					  (1 << 14),	// WPAD_BUTTON_FS_C
    WPAD_BUTTON_HOME = (1 << 15),

    WPAD_BUTTON_ALL = 0x9f1f
};

// names from wiiuse/wpad.h
typedef u16 WPADExtButton;
enum WPADExtButton_et {
    // Nunchuk (fight stick?)
    // .CZ..... ........
    /* NOTE: these bits are actually in the normal button variable, but are
     * enumerated here because they are buttons for an extension
     */
    WPAD_BUTTON_FS_Z = (1 << 13),
    WPAD_BUTTON_FS_C = (1 << 14),

    WPAD_BUTTON_FS_ALL = 0x6000,

    // Classic Controller
    // >vL-H+R. lBYAXr<^
    WPAD_BUTTON_CL_UP = (1 << 0),
    WPAD_BUTTON_CL_LEFT = (1 << 1),
    WPAD_BUTTON_CL_ZR = (1 << 2),
    WPAD_BUTTON_CL_X = (1 << 3),
    WPAD_BUTTON_CL_A = (1 << 4),
    WPAD_BUTTON_CL_Y = (1 << 5),
    WPAD_BUTTON_CL_B = (1 << 6),
    WPAD_BUTTON_CL_ZL = (1 << 7),
    //						  (1 <<  8),
    WPAD_BUTTON_CL_FULL_R = (1 << 9),
    WPAD_BUTTON_CL_PLUS = (1 << 10),
    WPAD_BUTTON_CL_HOME = (1 << 11),
    WPAD_BUTTON_CL_MINUS = (1 << 12),
    WPAD_BUTTON_CL_FULL_L = (1 << 13),
    WPAD_BUTTON_CL_DOWN = (1 << 14),
    WPAD_BUTTON_CL_RIGHT = (1 << 15),

    WPAD_BUTTON_CL_ALL = 0xfeff,

    // Shinkansen train controller
    // shares the same format as the Classic Controller
    // (see WPADiExcludeButton)
    WPAD_BUTTON_TR_UP = WPAD_BUTTON_CL_UP,
    WPAD_BUTTON_TR_LEFT = WPAD_BUTTON_CL_LEFT,
    WPAD_BUTTON_TR_ZR = WPAD_BUTTON_CL_ZR,
    WPAD_BUTTON_TR_X = WPAD_BUTTON_CL_X,
    WPAD_BUTTON_TR_A = WPAD_BUTTON_CL_A,
    WPAD_BUTTON_TR_Y = WPAD_BUTTON_CL_Y,
    WPAD_BUTTON_TR_B = WPAD_BUTTON_CL_B,
    WPAD_BUTTON_TR_ZL = WPAD_BUTTON_CL_ZL,
    //						= WPAD_BUTTON_CL_8,
    WPAD_BUTTON_TR_FULL_R = WPAD_BUTTON_CL_FULL_R,
    WPAD_BUTTON_TR_PLUS = WPAD_BUTTON_CL_PLUS,
    WPAD_BUTTON_TR_HOME = WPAD_BUTTON_CL_HOME,
    WPAD_BUTTON_TR_MINUS = WPAD_BUTTON_CL_MINUS,
    WPAD_BUTTON_TR_FULL_L = WPAD_BUTTON_CL_FULL_L,
    WPAD_BUTTON_TR_DOWN = WPAD_BUTTON_CL_DOWN,
    WPAD_BUTTON_TR_RIGHT = WPAD_BUTTON_CL_RIGHT,

    WPAD_BUTTON_TR_ALL = WPAD_BUTTON_CL_ALL,
};

// WPADControlMotor
typedef u32 WPADMotorCommand;
enum WPADMotorCommand_et {
    WPAD_MOTOR_STOP = 0,   /* name known from asserts */
    WPAD_MOTOR_RUMBLE = 1, /* name known from asserts */
};

// WPADControlSpeaker
typedef u32 WPADSpeakerCommand;
enum WPADSpeakerCommand_et {
    WPAD_SPEAKER_DISABLE = 0,
    WPAD_SPEAKER_ENABLE = 1, // might be ON? see HBMRemoteSpk.cpp
    WPAD_SPEAKER_MUTE = 2,
    WPAD_SPEAKER_UNMUTE = 3,
    WPAD_SPEAKER_PLAY = 4, // figured out from HBM usage

    // does the same thing as ENABLE? unless i'm missing something
    // not used so i don't know the context
    WPAD_SPEAKER_CMD_05 = 5,
};

// WPADControlDpd
typedef u32 WPADDpdCommand;
enum WPADDpdCommand_et {
    WPAD_DPD_DISABLE = 0x00,

    // the command here is actually the data format
    WPAD_DPD_BASIC = 0x01,
    WPAD_DPD_STANDARD = 0x03,
    WPAD_DPD_EXTENDED = 0x05,
};

// WPADControlBLC
typedef u8 WPADBLCCommand;
enum WPADBLCCommand_et {
    WPAD_BLC_DISABLE = 0x00,
    WPAD_BLC_CMD_55 = 0x55,

    /* NOTE: maybe? the code on this case in WPADControlBLC seems to follow what
     * WiiBrew describes as the initialization sequence after the generic one
     * (https://wiibrew.org/wiki/Wii_Balance_Board#Wii_Initialisation_Sequence).
     */
    WPAD_BLC_ENABLE = 0xaa,
};

// WPADMPStatus::stat
typedef u8 WPADMPStatusFlags;
enum WPADMPStatusFlags_et {
    WPAD_MPLS_STATUS_DATA_VALID = (1 << 7),
    WPAD_MPLS_STATUS_EXTENSION_DATA_VALID = (1 << 6),

    WPAD_MPLS_STATUS_YAW_SLOW_MODE = (1 << 3),
    WPAD_MPLS_STATUS_PITCH_SLOW_MODE = (1 << 2),
    WPAD_MPLS_STATUS_ROLL_SLOW_MODE = (1 << 1),
    WPAD_MPLS_STATUS_EXTENSION_CONNECTED = (1 << 0), // just that it's present; not that it's actually being used
};

// WPADGetAccGravityUnit
typedef u32 WPADAccGravityUnitType;
enum WPADAccGravityUnitType_et {
    WPAD_ACC_GRAVITY_UNIT_CORE,
    WPAD_ACC_GRAVITY_UNIT_FS,
};

typedef WUDAllocFunc WPADAllocFunc;
typedef WUDFreeFunc WPADFreeFunc;

typedef void WPADInitFunc(void); // see __wpadInitSub in WPAD.c

typedef void WPADCallback(WPADChannel chan, WPADResult result);

typedef void WPADSamplingCallback(WPADChannel chan);
typedef void WPADConnectCallback(WPADChannel chan, s32 result);
typedef void WPADExtensionCallback(WPADChannel chan, WPADDeviceType devType);

typedef WUDSyncDeviceCallback WPADSimpleSyncCallback;

// [SGLEA4]/GormitiDebug.elf:.debug_info::0x79f49
typedef struct DPDObject {
    s16 x;      // size 0x02, offset 0x00
    s16 y;      // size 0x02, offset 0x02
    u16 size;   // size 0x02, offset 0x04
    u8 traceId; // size 0x01, offset 0x06
                // 1 byte padding
} DPDObject;    // size 0x08

// TODO: get a specific source (name, game id)
// TODO: check library version of game that has this
// dwarfv1_megadump.txt:21551-21559
typedef struct DPDObjEx {
    s16 range_x1; // size 0x02, offset 0x00
    s16 range_y1; // size 0x02, offset 0x02
    s16 range_x2; // size 0x02, offset 0x04
    s16 range_y2; // size 0x02, offset 0x06
    u16 pixel;    // size 0x02, offset 0x08
    s8 radius;    // size 0x01, offset 0x0a
                  // 1 byte padding
} DPDObjEx;       // size 0x0c

// [SPQE7T]/ISpyD.elf:.debug_info::0xd675b
typedef struct WPADInfo {
    BOOL dpd;       // size 0x04, offset 0x00
    BOOL speaker;   // size 0x04, offset 0x04
    BOOL attach;    // size 0x04, offset 0x08
    BOOL lowBat;    // size 0x04, offset 0x0c
    BOOL nearempty; // size 0x04, offset 0x10
    u8 battery;     // size 0x01, offset 0x14
    u8 led;         // size 0x01, offset 0x15
    u8 protocol;    // size 0x01, offset 0x16
    u8 firmware;    // size 0x01, offset 0x17
} WPADInfo;         // size 0x18

// [SGLEA4]/GormitiDebug.elf:.debug_info::0x7ab19
typedef struct WPADStatus {
    WPADButton button;                   // size 0x02, offset 0x00
    s16 accX;                            // size 0x02, offset 0x02
    s16 accY;                            // size 0x02, offset 0x04
    s16 accZ;                            // size 0x02, offset 0x06
    DPDObject obj[WPAD_MAX_DPD_OBJECTS]; // size 0x20, offset 0x08
    u8 dev;                              // size 0x01, offset 0x28
    s8 err;                              // size 0x01, offset 0x29
} WPADStatus;                            // size 0x2a

/* The following take their initial members from WPADStatus instead of including
 * an instance of it. For ease of use?
 */

// TODO: get a specific source (name, game id)
// TODO: check library version of game that has this
// dwarfv1_megadump.txt:21560-21570
typedef struct WPADStatusEx {
    WPADButton button;                   // size 0x02, offset 0x00
    s16 accX;                            // size 0x02, offset 0x02
    s16 accY;                            // size 0x02, offset 0x04
    s16 accZ;                            // size 0x02, offset 0x06
    DPDObject obj[WPAD_MAX_DPD_OBJECTS]; // size 0x20, offset 0x08
    u8 dev;                              // size 0x01, offset 0x28
    s8 err;                              // size 0x01, offset 0x29

    DPDObjEx exp[WPAD_MAX_DPD_OBJECTS]; // size 0x30, offset 0x2a
} WPADStatusEx;                         // size 0x5a

// [SGLEA4]/GormitiDebug.elf:.debug_info::0x79e65
typedef struct WPADFSStatus {
    WPADButton button;                   // size 0x02, offset 0x00
    s16 accX;                            // size 0x02, offset 0x02
    s16 accY;                            // size 0x02, offset 0x04
    s16 accZ;                            // size 0x02, offset 0x06
    DPDObject obj[WPAD_MAX_DPD_OBJECTS]; // size 0x20, offset 0x08
    u8 dev;                              // size 0x01, offset 0x28
    s8 err;                              // size 0x01, offset 0x29

    s16 fsAccX;  // size 0x02, offset 0x2a
    s16 fsAccY;  // size 0x02, offset 0x2c
    s16 fsAccZ;  // size 0x02, offset 0x2e
    s8 fsStickX; // size 0x01, offset 0x30
    s8 fsStickY; // size 0x01, offset 0x31
} WPADFSStatus;  // size 0x32

// [SGLEA4]/GormitiDebug.elf:.debug_info::0x7aba2
typedef struct WPADCLStatus {
    WPADButton button;                   // size 0x02, offset 0x00
    s16 accX;                            // size 0x02, offset 0x02
    s16 accY;                            // size 0x02, offset 0x04
    s16 accZ;                            // size 0x02, offset 0x06
    DPDObject obj[WPAD_MAX_DPD_OBJECTS]; // size 0x20, offset 0x08
    u8 dev;                              // size 0x01, offset 0x28
    s8 err;                              // size 0x01, offset 0x29

    WPADExtButton clButton; // size 0x02, offset 0x2a
    s16 clLStickX;          // size 0x02, offset 0x2c
    s16 clLStickY;          // size 0x02, offset 0x2e
    s16 clRStickX;          // size 0x02, offset 0x30
    s16 clRStickY;          // size 0x02, offset 0x32
    u8 clTriggerL;          // size 0x01, offset 0x34
    u8 clTriggerR;          // size 0x01, offset 0x35
} WPADCLStatus;             // size 0x36

// [SGLEA4]/GormitiDebug.elf:.debug_info::0x7acba
typedef struct WPADTRStatus {
    WPADButton button;                   // size 0x02, offset 0x00
    s16 accX;                            // size 0x02, offset 0x02
    s16 accY;                            // size 0x02, offset 0x04
    s16 accZ;                            // size 0x02, offset 0x06
    DPDObject obj[WPAD_MAX_DPD_OBJECTS]; // size 0x20, offset 0x08
    u8 dev;                              // size 0x01, offset 0x28
    s8 err;                              // size 0x01, offset 0x29

    WPADExtButton trButton; // size 0x02, offset 0x2a
    u8 brake;               // size 0x01, offset 0x2c
    u8 mascon;              // size 0x01, offset 0x2d
} WPADTRStatus;             // size 0x2e

// [SGLEA4]/GormitiDebug.elf:.debug_info::0x7ad79
typedef struct WPADBLStatus {
    WPADButton button;                   // size 0x02, offset 0x00
    s16 accX;                            // size 0x02, offset 0x02
    s16 accY;                            // size 0x02, offset 0x04
    s16 accZ;                            // size 0x02, offset 0x06
    DPDObject obj[WPAD_MAX_DPD_OBJECTS]; // size 0x20, offset 0x08
    u8 dev;                              // size 0x01, offset 0x28
    s8 err;                              // size 0x01, offset 0x29

    u16 press[4]; // size 0x08, offset 0x2a
    s8 temp;      // size 0x01, offset 0x32
    u8 battery;   // size 0x01, offset 0x33
} WPADBLStatus;   // size 0x34

// [SGLEA4]/GormitiDebug.elf:.debug_info::0x7ae44
typedef struct WPADMPStatus {
    WPADButton button;                   // size 0x02, offset 0x00
    s16 accX;                            // size 0x02, offset 0x02
    s16 accY;                            // size 0x02, offset 0x04
    s16 accZ;                            // size 0x02, offset 0x06
    DPDObject obj[WPAD_MAX_DPD_OBJECTS]; // size 0x20, offset 0x08
    u8 dev;                              // size 0x01, offset 0x28
    s8 err;                              // size 0x01, offset 0x29

    // [SGLEA4]/GormitiDebug.elf:.debug_info::0x7af2c
    union /* explicitly untagged */
    {
        // Takes its members from WPADFSStatus
        // [SGLEA4]/GormitiDebug.elf:.debug_info::0x7af54
        struct /* explicitly untagged */
        {
            s16 fsAccX;  // size 0x02, offset 0x00
            s16 fsAccY;  // size 0x02, offset 0x02
            s16 fsAccZ;  // size 0x02, offset 0x04
            s8 fsStickX; // size 0x01, offset 0x06
            s8 fsStickY; // size 0x01, offset 0x07
        } fs;            // size 0x08

        // Takes its members from WPADCLStatus
        // [SGLEA4]/GormitiDebug.elf:.debug_info::0x7afbb
        struct /* explicitly untagged */
        {
            WPADExtButton clButton; // size 0x02, offset 0x00
            s16 clLStickX;          // size 0x02, offset 0x02
            s16 clLStickY;          // size 0x02, offset 0x04
            s16 clRStickX;          // size 0x02, offset 0x06
            s16 clRStickY;          // size 0x02, offset 0x08
            u8 clTriggerL;          // size 0x01, offset 0x0a
            u8 clTriggerR;          // size 0x01, offset 0x0b
        } cl;                       // size 0x0c
    } ext;                          // size 0x0c, offset 0x2a

    WPADMPStatusFlags stat; // size 0x01, offset 0x36
    u8 reserved;            // size 0x01, offset 0x37

    s16 pitch;  // size 0x02, offset 0x38
    s16 yaw;    // size 0x02, offset 0x3a
    s16 roll;   // size 0x02, offset 0x3c
} WPADMPStatus; // size 0x3e

// [SGLEA4]/GormitiDebug.elf:.debug_info::0x7b056
typedef struct WPADBKStatus {
    WPADButton button;                   // size 0x02, offset 0x00
    s16 accX;                            // size 0x02, offset 0x02
    s16 accY;                            // size 0x02, offset 0x04
    s16 accZ;                            // size 0x02, offset 0x06
    DPDObject obj[WPAD_MAX_DPD_OBJECTS]; // size 0x20, offset 0x08
    u8 dev;                              // size 0x01, offset 0x28
    s8 err;                              // size 0x01, offset 0x29

    byte_t bulk[21];   // size 0x15, offset 0x2a
    byte_t padding[1]; // size 0x01, offset 0x3f
} WPADBKStatus;        // size 0x40

typedef struct WPADVSStatus {
    WPADButton button;                   // size 0x02, offset 0x00
    s16 accX;                            // size 0x02, offset 0x02
    s16 accY;                            // size 0x02, offset 0x04
    s16 accZ;                            // size 0x02, offset 0x06
    DPDObject obj[WPAD_MAX_DPD_OBJECTS]; // size 0x20, offset 0x08
    u8 dev;                              // size 0x01, offset 0x28
    s8 err;                              // size 0x01, offset 0x29

    u16 at_0x2a[5]; // size 0x0a, offset 0x2a
    u8 at_0x34;     // size 0x02, offset 0x34
    // 1 byte padding (alignment? probably)
    u16 at_0x36[5]; // size 0x0a, offset 0x36
    u8 at_0x40;     // size 0x02, offset 0x40
    // 1 byte padding (alignment? probably)
    u16 at_0x42; // size 0x02, offset 0x42
    u8 at_0x44;  // size 0x01, offset 0x44
                 // 1 byte padding (alignment? probably)
} WPADVSStatus;  // size 0x46 (see __wpadFmt2Size)

typedef struct WPADAccGravityUnit {
    // probably maybe
    s16 x;            // size 0x02, offset 0x00
    s16 y;            // size 0x02, offset 0x02
    s16 z;            // size 0x02, offset 0x04
} WPADAccGravityUnit; // size 0x06

/*******************************************************************************
 * functions
 */

BOOL WPADIsEnabledVSM(void);
BOOL WPADIsEnabledTRN(void);
BOOL WPADIsEnabledGTR(void);
BOOL WPADIsEnabledDRM(void);
BOOL WPADIsEnabledTKO(void);
BOOL WPADIsEnabledTBL(void);
BOOL WPADIsEnabledBLK(void);

// Likely overridden by a WBC-specific library
__attribute__((weak)) WPADResult WBCSetupCalibration(void);
__attribute__((weak)) signed WBCGetCalibrationStatus(void);
__attribute__((weak)) signed WBCGetBatteryLevel(u8);
__attribute__((weak)) WPADResult WBCRead(WPADBLStatus *, f64 *, int);
__attribute__((weak)) WPADResult WBCSetZEROPoint(f64 *, int);
__attribute__((weak)) WPADResult WBCGetTGCWeight(f64, f64 *, WPADBLStatus *);

void WPADInit(void);
void WPADShutdown(void);

BOOL WPADStartFastSimpleSync(void);
BOOL WPADStopSimpleSync(void);
BOOL WPADCancelSyncDevice(void);
WPADSimpleSyncCallback *WPADSetSimpleSyncCallback(WPADSimpleSyncCallback *cb);
void WPADRegisterAllocator(WPADAllocFunc *alloc, WPADFreeFunc *free);
WPADLibStatus WPADGetStatus(void);
u8 WPADGetRadioSensitivity(WPADChannel chan);
void WPADGetAddress(WPADChannel chan, BD_ADDR_PTR addr);
u8 WPADGetSensorBarPosition(void);

void WPADGetAccGravityUnit(WPADChannel chan, WPADAccGravityUnitType type, WPADAccGravityUnit *acc);

void WPADDisconnect(WPADChannel chan);
WPADResult WPADProbe(WPADChannel chan, WPADDeviceType *devTypeOut);
WPADSamplingCallback *WPADSetSamplingCallback(WPADChannel chan, WPADSamplingCallback *cb);
WPADConnectCallback *WPADSetConnectCallback(WPADChannel chan, WPADConnectCallback *cb);
WPADExtensionCallback *WPADSetExtensionCallback(WPADChannel chan, WPADExtensionCallback *cb);
WPADDataFormat WPADGetDataFormat(WPADChannel chan);
WPADResult WPADSetDataFormat(WPADChannel chan, WPADDataFormat fmt);

WPADResult WPADGetInfoAsync(WPADChannel chan, WPADInfo *info, WPADCallback *cb);
void WPADControlMotor(WPADChannel chan, WPADMotorCommand command);
void WPADEnableMotor(BOOL enabled);
BOOL WPADIsMotorEnabled(void);
WPADResult WPADControlLed(WPADChannel chan, u8 ledFlags, WPADCallback cb);
BOOL WPADSaveConfig(SCFlushCallback *cb);

/* NOTE: status should match the WPADStatus type for the channel; a check
 * against the currently assigned device type is made to know how much to copy
 *
 * for example, if devType[chan] == WPAD_DEV_CLASSIC, status should be a
 * pointer to WPADCLStatus cast to a pointer to WPADStatus
 */
void WPADRead(WPADChannel chan, WPADStatus *status);
void WPADSetAutoSamplingBuf(WPADChannel chan, void *buf, u32 length);

BOOL WPADIsSpeakerEnabled(WPADChannel chan);
WPADResult WPADControlSpeaker(WPADChannel chan, WPADSpeakerCommand command, WPADCallback *cb);
u8 WPADGetSpeakerVolume(void);
void WPADSetSpeakerVolume(u8 vol);

BOOL WPADCanSendStreamData(WPADChannel chan);
WPADResult WPADSendStreamData(WPADChannel chan, void *p_buf, u16 len);
u8 WPADGetDpdSensitivity(void);
BOOL WPADIsDpdEnabled(WPADChannel chan);

WPADResult WPADControlDpd(WPADChannel chan, WPADDpdCommand command, WPADCallback *cb);
WPADResult WPADControlBLC(WPADChannel chan, WPADBLCCommand command, WPADCallback *cb);

void WPADRecalibrate(WPADChannel chan);
BOOL WPADIsUsedCallbackByKPAD(void);
void WPADSetCallbackByKPAD(BOOL isKPAD);

#ifdef __cplusplus
}
#endif

#endif // RVL_SDK_WPAD_H
