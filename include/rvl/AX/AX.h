#ifndef RVL_SDK_AX_H
#define RVL_SDK_AX_H
#include "common.h"
#ifdef __cplusplus
extern "C" {
#endif

#define AX_MAX_VOICES					0x60	/* name known from asserts */
#define AX_SAMPLE_RATE					32000
#define AX_ADPCM_FRAME_SIZE				8
#define AX_ADPCM_SAMPLE_BYTES_PER_FRAME	(AX_ADPCM_FRAME_SIZE - 1)
#define AX_ADPCM_SAMPLES_PER_BYTE		2
#define AX_ADPCM_SAMPLES_PER_FRAME		(AX_ADPCM_SAMPLE_BYTES_PER_FRAME * AX_ADPCM_SAMPLES_PER_BYTE)
#define AX_ADPCM_NIBBLES_PER_FRAME		(AX_ADPCM_FRAME_SIZE * 2)
#define AX_FRAME_SIZE					(AX_SAMPLES_PER_FRAME * AX_SAMPLE_DEPTH_BYTES)

#define AX_PRIORITY_STACKS	32

#define AX_PRIORITY_FREE	0
#define AX_PRIORITY_MIN		1

void AXInit(void);
void AXInitEx(u32 mode);

#ifdef __cplusplus
}
#endif
#endif
