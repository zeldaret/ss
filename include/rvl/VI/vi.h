#ifndef RVL_SDK_VI_H
#define RVL_SDK_VI_H
#include "common.h"

#include "rvl/GX.h" // IWYU pragma: export

#ifdef __cplusplus
extern "C" {
#endif

// Merge format/mode to one value for TV info (see GXRenderModeObj)
#define VI_TV_INFO(format, mode) (((format) << 2) + (mode))
// Get TV format from TV info
#define VI_TV_INFO_FMT(info) ((info) >> 2)
// Get TV scan mode from TV info
#define VI_TV_INFO_MODE(info) ((info) & 0b00000011)

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

void getEncoderType();
void OnShutdown();                                                      // ret unk, params unk
void cntlzd();                                                          // ret unk, params unk
void VISetRegs();                                                       // ret unk, params unk
void __VIRetraceHandler();                                              // ret unk,params unk
VIRetraceCallback VISetPreRetraceCallback(VIRetraceCallback callback);  // good
VIRetraceCallback VISetPostRetraceCallback(VIRetraceCallback callback); // good
void __VISetExtraTiming();                                              // ret unk, params unk
void getTiming();                                                       // ret unk, params unk
void __VIInit(u32);                                                     // ret unk, params unk
void AdjustPosition();                                                  // ret unk, params unk
void ImportAdjustingValues();                                           // ret unk, params unk
void VIInit(void);                                                      // good
void VIWaitForRetrace(void);                                            // good
void setInterruptRegs();                                                // ret unk, params unk
void setPicConfig();                                                    // ret unk, params unk
void setBBIntervalRegs();                                               // ret unk, params unk
void setScalingRegs();                                                  // ret unk, params unk
void calcFbbs();                                                        // ret unk, params unk
void setFbbRegs();                                                      // params unk
void setHorizontalRegs();                                               // params unk
void setVerticalRegs();                                                 // params unk
void VIConfigure(const GXRenderModeObj *rmo);                           // good
void VIConfigurePan(u16 x, u16 y, u16 w, u16 h);                        // good
void VIFlush();                                                         // good
void VISetNextFrameBuffer(void *fb);                                    // good
void VIGetNextFrameBuffer();                                            // ret unk, params unk
void *VIGetCurrentFrameBuffer();                                        // good
void VISetNextRightFrameBuffer();                                       // ret unk, params unk
void VISetBlack(BOOL black);                                            // good
void VISet3D();                                                         // ret unk, params unk
s32 VIGetRetraceCount(void);                                            // good
u32 VIGetNextField();                                                   // ret unk
u32 VIGetCurrentLine();                                                 // ret unk
VITvFormat VIGetTvFormat(void);                                         // good
VIScanMode VIGetScanMode(void);                                         // good
int VIGetDTVStatus();                                                   // ret unk
void VIGetVSyncTimingTest();                                            // ret, params unk
void VISetVSyncTimingTest();                                            // ret, params unk
void VIGetDimmingCount();                                               // ret, params unk
void VIGetDVDStopMotorCount();                                          // ret, params unk
int VIEnableDimming(int);                                               // ret unk, params unk
int VISetTimeToDimming(int);                                            // ret unk, params unk
int VIResetDimmingCount();                                              // ret unk
void VIEnableDVDStopMotor();                                            // ret, params unk

#ifdef __cplusplus
}
#endif
#endif
