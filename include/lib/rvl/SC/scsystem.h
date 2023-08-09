#ifndef RVL_SDK_SC_SCSYSTEM_H
#define RVL_SDK_SC_SCSYSTEM_H
#include <lib/rvl/FS.h>
#include <lib/rvl/NAND.h>
#include <lib/rvl/OS.h>
#include <lib/rvl/types.h>
#ifdef __cplusplus
extern "C" {
#endif

/**
 * SC/SYSCONF documentation from:
 * https://wiibrew.org/wiki//shared2/sys/SYSCONF
 */

typedef enum {
    SC_STATUS_0,
    SC_STATUS_1,
    SC_STATUS_2,
    SC_STATUS_3,
} SCStatus;

typedef enum {
    SC_CONF_FILE_SYSTEM,  //!< SYSCONF
    SC_CONF_FILE_PRODUCT, //!< setting.txt
    SC_CONF_FILE_MAX
} SCConfFile;

typedef enum {
    SC_ITEM_IPL_CB,
    SC_ITEM_IPL_AR,
    SC_ITEM_IPL_ARN,
    SC_ITEM_IPL_CD,
    SC_ITEM_IPL_CD2,
    SC_ITEM_IPL_DH,
    SC_ITEM_IPL_E60,
    SC_ITEM_IPL_EULA,
    SC_ITEM_IPL_FRC,
    SC_ITEM_IPL_IDL,
    SC_ITEM_IPL_INC,
    SC_ITEM_IPL_LNG,
    SC_ITEM_IPL_NIK,
    SC_ITEM_IPL_PC,
    SC_ITEM_IPL_PGS,
    SC_ITEM_IPL_SSV,
    SC_ITEM_IPL_SADR,
    SC_ITEM_IPL_SND,
    SC_ITEM_IPL_UPT,
    SC_ITEM_NET_CNF,
    SC_ITEM_NET_CTPC,
    SC_ITEM_NET_PROF,
    SC_ITEM_NET_WCPC,
    SC_ITEM_NET_WCFG,
    SC_ITEM_DEV_BTM,
    SC_ITEM_DEV_VIM,
    SC_ITEM_DEV_CTC,
    SC_ITEM_DEV_DSM,
    SC_ITEM_BT_DINF,
    SC_ITEM_BT_SENS,
    SC_ITEM_BT_SPKV,
    SC_ITEM_BT_MOT,
    SC_ITEM_BT_BAR,
    SC_ITEM_DVD_CNF,
    SC_ITEM_WWW_RST,
    SC_ITEM_MAX
} SCItemID;

typedef enum {
    SC_ITEM_BIGARRAY = (1 << 5),
    SC_ITEM_SMALLARRAY = (2 << 5),
    SC_ITEM_BYTE = (3 << 5),
    SC_ITEM_SHORT = (4 << 5),
    SC_ITEM_LONG = (5 << 5),
    SC_ITEM_LONGLONG = (6 << 5),
    SC_ITEM_BOOL = (7 << 5)
} SCItemType;

typedef void (*SCAsyncCallback)(NANDResult result);
typedef void (*SCFlushCallback)(SCStatus status);

typedef struct SCItem {
    char UNK_0x0[0x8];
    u8 primType;      // at 0x8
    u8 arrayType;     // at 0x9
    u32 nameLen;      // at 0xC
    u32 dataLen;      // at 0x10
    const char* name; // at 0x14
    u8* data;         // at 0x18
    u32 itemLen;      // at 0x1C
} SCItem;

typedef struct SCControl {
    OSThreadQueue threadQueue;     // at 0x0
    NANDFileInfo fileInfo;         // at 0x8
    NANDCommandBlock commandBlock; // at 0x94
    NANDStatus fileAttr;           // at 0x14C
    u8 nandCbState;                // at 0x154
    u8 BYTE_0x155;
    u8 openFile; // at 0x156
    u8 BYTE_0x157;
    SCAsyncCallback asyncCallback;           // at 0x158
    NANDResult asyncResult;                  // at 0x15C
    const char* filePaths[SC_CONF_FILE_MAX]; // at 0x160
    u8* fileBuffers[SC_CONF_FILE_MAX];       // at 0x168
    u32 bufferSizes[SC_CONF_FILE_MAX];       // at 0x170
    u32 fileSizes[SC_CONF_FILE_MAX];         // at 0x178
    SCFlushCallback flushCallback;           // at 0x180
    SCStatus flushStatus;                    // at 0x184
    u32 flushSize;                           // at 0x188
} SCControl;

void SCInit(void);
u32 SCCheckStatus(void);

BOOL SCFindByteArrayItem(void* dst, u32 len, SCItemID id);
BOOL SCReplaceByteArrayItem(const void* src, u32 len, SCItemID id);

BOOL SCFindU8Item(u8* dst, SCItemID id);
BOOL SCFindS8Item(s8* dst, SCItemID id);
BOOL SCFindU32Item(u32* dst, SCItemID id);

BOOL SCReplaceU8Item(u8 data, SCItemID id);

void SCFlushAsync(SCFlushCallback callback);

BOOL __SCIsDirty(void);
void __SCSetDirtyFlag(void);
void __SCClearDirtyFlag(void);

u8* __SCGetConfBuf(void);
u32 __SCGetConfBufSize(void);

#ifdef __cplusplus
}
#endif
#endif
