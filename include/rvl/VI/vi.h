#ifndef RVL_SDK_VI_H
#define RVL_SDK_VI_H
#include "rvl/GX.h"
#include "rvl/types.h"
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

/*          */ void getEncoderType();
/* 803b34a0 */ void OnShutdown();                                                      // ret unk, params unk
/*          */ void cntlzd();                                                          // ret unk, params unk
/*          */ void VISetRegs();                                                       // ret unk, params unk
/* 803b3640 */ void __VIRetraceHandler();                                              // ret unk,params unk
/* 803b3e50 */ VIRetraceCallback VISetPreRetraceCallback(VIRetraceCallback callback);  // good
/* 803b3ea0 */ VIRetraceCallback VISetPostRetraceCallback(VIRetraceCallback callback); // good
/*          */ void __VISetExtraTiming();                                              // ret unk, params unk
/* 803b3ef0 */ void getTiming();                                                       // ret unk, params unk
/* 803b4010 */ void __VIInit(u32);                                                     // ret unk, params unk
/*          */ void AdjustPosition();                                                  // ret unk, params unk
/*          */ void ImportAdjustingValues();                                           // ret unk, params unk
/* 803b4210 */ void VIInit(void);                                                      // good
/* 803b4760 */ void VIWaitForRetrace(void);                                            // good
/*           */ void setInterruptRegs();                                               // ret unk, params unk
/*           */ void setPicConfig();                                                   // ret unk, params unk
/*           */ void setBBIntervalRegs();                                              // ret unk, params unk
/*           */ void setScalingRegs();                                                 // ret unk, params unk
/*           */ void calcFbbs();                                                       // ret unk, params unk
/* 803b47c0? */ void setFbbRegs();                                                     // params unk
/* 803b4a70? */ void setHorizontalRegs();                                              // params unk
/* 803b4b50? */ void setVerticalRegs();                                                // params unk
/* 803b4cf0 */ void VIConfigure(const GXRenderModeObj *rmo);                           // good
/* 803b53c0 */ void VIConfigurePan(u16 x, u16 y, u16 w, u16 h);                        // good
/* 803b5720 */ void VIFlush();                                                         // good
/* 803b5840 */ void VISetNextFrameBuffer(void *fb);                                    // good
/*          */ void VIGetNextFrameBuffer();                                            // ret unk, params unk
/* 803b58b0 */ void *VIGetCurrentFrameBuffer();                                        // good
/*          */ void VISetNextRightFrameBuffer();                                       // ret unk, params unk
/* 803b58c0 */ void VISetBlack(BOOL black);                                            // good
/*          */ void VISet3D();                                                         // ret unk, params unk
/* 803b5940 */ s32 VIGetRetraceCount(void);                                            // good
/* 803b5950 */ u32 VIGetNextField();                                                   // ret unk
/* 803b59f0 */ u32 VIGetCurrentLine();                                                 // ret unk
/* 803b5a80 */ VITvFormat VIGetTvFormat(void);                                         // good
/*          */ VIScanMode VIGetScanMode(void);                                         // good
/* 803b5ae0 */ int VIGetDTVStatus();                                                   // ret unk
/*          */ void VIGetVSyncTimingTest();                                            // ret, params unk
/*          */ void VISetVSyncTimingTest();                                            // ret, params unk
/*          */ void VIGetDimmingCount();                                               // ret, params unk
/*          */ void VIGetDVDStopMotorCount();                                          // ret, params unk
/* 803b5d60 */ int VIEnableDimming(int);                                               // ret unk, params unk
/* 803b5dc0 */ int VISetTimeToDimming(int);                                            // ret unk, params unk
/* 803b5ec0 */ int VIResetDimmingCount();                                              // ret unk
/*          */ void VIEnableDVDStopMotor();                                            // ret, params unk

#ifdef __cplusplus
}
#endif
#endif
