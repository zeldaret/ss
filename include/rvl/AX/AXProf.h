#ifndef RVL_SDK_AX_PROF_H
#define RVL_SDK_AX_PROF_H
#include <common.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef struct _AXPROFILE {
    s64 timeBegin;           // at 0x0
    s64 timeProcessAuxBegin; // at 0x8
    s64 timeProcessAuxEnd;   // at 0x10
    s64 timeUserFrameBegin;  // at 0x18
    s64 timeUserFrameEnd;    // at 0x20
    s64 timeEnd;             // at 0x28
    s32 numVoices;           // at 0x30
    UNKWORD WORD_0x34;
} AXPROFILE;

AXPROFILE *__AXGetCurrentProfile(void);

#ifdef __cplusplus
}
#endif
#endif
