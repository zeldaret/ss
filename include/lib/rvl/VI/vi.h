#ifndef RVL_SDK_VI_H
#define RVL_SDK_VI_H
#include <lib/rvl/GX.h>
#include <lib/rvl/types.h>
#ifdef __cplusplus
extern "C" {
#endif

// Merge format/mode to one value for TV info (see GXRenderModeObj)
#define VI_TV_INFO(format, mode) (((format) << 2) + (mode))
// Get TV format from TV info
#define VI_TV_INFO_FMT(info) ((info) >> 2)
// Get TV scan mode from TV info
#define VI_TV_INFO_MODE(info) ((info)&0b00000011)

typedef enum {
    VI_TV_FMT_NTSC,
    VI_TV_FMT_PAL,
    VI_TV_FMT_MPAL,
    VI_TV_FMT_DEBUG,
    VI_TV_FMT_DEBUG_PAL,
    VI_TV_FMT_EURGB60,
    VI_TV_FMT_6,
    VI_TV_FMT_7,
    VI_TV_FMT_8,
} VITvFormat;

typedef enum VIScanMode {
    VI_SCAN_MODE_NON_INT,
    VI_SCAN_MODE_INT,
    VI_SCAN_MODE_PROG,
    VI_SCAN_MODE_3,
} VIScanMode;

typedef enum VIXfbMode {
    VI_XFB_MODE_SF,
    VI_XFB_MODE_DF,
} VIXfbMode;

typedef void (*VIRetraceCallback)(u32 retraceCount);
typedef void (*VIPositionCallback)(s16 displayX, s16 displayY);

VIRetraceCallback VISetPreRetraceCallback(VIRetraceCallback callback);
VIRetraceCallback VISetPostRetraceCallback(VIRetraceCallback callback);

void VIInit(void);
void VIWaitForRetrace(void);

void VIConfigure(const GXRenderModeObj* rmo);
void VIConfigurePan(u16 x, u16 y, u16 w, u16 h);
void VIFlush(void);
void VISetNextFrameBuffer(void* fb);

void VISetBlack(BOOL black);
s32 VIGetRetraceCount(void);

VITvFormat VIGetTvFormat(void);
VIScanMode VIGetScanMode(void);

#ifdef __cplusplus
}
#endif
#endif
