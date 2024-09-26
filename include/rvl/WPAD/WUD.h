#ifndef RVL_SDK_WPAD_WUD_H
#define RVL_SDK_WPAD_WUD_H

#include <common.h>

// TODO: Fix when possible
#include "context_bte.h"

#define WUD_MAX_DEV_ENTRY_FOR_STD 10 // name known from asserts
#define WUD_MAX_DEV_ENTRY_FOR_SMP 6  // name known from asserts

#define WUD_MAX_DEV_ENTRY 16 // name known from asserts

#ifdef __cplusplus
extern "C" {
#endif

typedef s8 WUDDevHandle;
#define WUD_DEV_HANDLE_INVALID (-1)

typedef s32 WUDChannel;
enum WUDChannel_et {
    WUD_CHAN0,
    WUD_CHAN1,
    WUD_CHAN2,
    WUD_CHAN3,

    WUD_MAX_CHANNELS,
    WUD_CHAN_INVALID = -1
};

typedef s32 WUDLibStatus;
enum WUDLibStatus_et {
    WUD_LIB_STATUS_0,
    WUD_LIB_STATUS_1,
    WUD_LIB_STATUS_2,
    WUD_LIB_STATUS_3,
    WUD_LIB_STATUS_4,
};

typedef u8 WUDSyncType;
enum WUDSyncType_et {
    WUD_SYNC_TYPE_STANDARD,
    WUD_SYNC_TYPE_SIMPLE,
};

typedef void *WUDAllocFunc(size_t size);
typedef int WUDFreeFunc(void *ptr); // signature from petari. not sure what the int is, though

typedef void WUDSyncDeviceCallback(s32 result, s32 num); // what is result?
typedef void WUDClearDeviceCallback(signed);

typedef struct WUDDevInfo WUDDevInfo;
typedef void WUDHidConnectCallback(WUDDevInfo *devInfo, u8 isOpenEvent);
typedef void WUDHidReceiveCallback(UINT8 dev_handle, UINT8 *p_rpt, UINT16 len);

struct WUDDevInfo {
    struct small_dev_info small;  // size 0x40, offset 0x00
    BD_ADDR devAddr;              // size 0x06, offset 0x40
    LINK_KEY linkKey;             // size 0x10, offset 0x46
    UINT8 devHandle;              // size 0x01, offset 0x56
    UINT8 subclass;               // size 0x01, offset 0x57
    UINT8 appID;                  // size 0x01, offset 0x58
    u8 at_0x59;                   // size 0x01, offset 0x59
    char signed at_0x5a;          // size 0x01, offset 0x5a	/* unknown */
    u8 at_0x5b;                   // size 0x01, offset 0x5b
    u8 at_0x5c;                   // size 0x01, offset 0x5c
    byte_t unk_0x5d[1];           // padding?
    tBTA_HH_ATTR_MASK hhAttrMask; // size 0x02, offset 0x5e
};                                // size 0x60

/*******************************************************************************
 * functions
 */

BOOL WUDIsLinkedWBC(void);

BOOL WUDInit(void);
void WUDRegisterAllocator(WUDAllocFunc *alloc, WUDFreeFunc *free);
void WUDShutdown(BOOL saveSimpleDevs);
WUDLibStatus WUDGetStatus(void);
u8 WUDGetBufferStatus(void);
void WUDSetSniffMode(BD_ADDR addr, int interval);
WUDSyncDeviceCallback *WUDSetSyncSimpleCallback(WUDSyncDeviceCallback *cb);

BOOL WUDStartSyncDevice(void);
BOOL WUDStartFastSyncSimple(void);

BOOL WUDCancelSyncDevice(void);
BOOL WUDStopSyncSimple(void);
BOOL WUDStartClearDevice(void);
BOOL WUDSetDisableChannel(s8 afhChannel);
WUDHidConnectCallback *WUDSetHidConnCallback(WUDHidConnectCallback *hidConn);
WUDHidReceiveCallback *WUDSetHidRecvCallback(WUDHidReceiveCallback *hidRecv);
void WUDSetVisibility(u8 discoverable, u8 connectable);
u8 WUDGetConnectable(void);

BOOL WUDIsBusy(void);

void WUDSetDeviceHistory(WUDChannel chan, BD_ADDR addr);
BOOL WUDIsLatestDevice(WUDChannel chan, BD_ADDR addr);
void WUDUpdateSCSetting(void);

#ifdef __cplusplus
}
#endif

#endif // RVL_SDK_WPAD_WUD_H
