#ifndef RVL_SDK_AX_SPB_H
#define RVL_SDK_AX_SPB_H
#include <common.h>
#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(push, 1)
typedef struct _AXSTUDIO {
  s32 L;
  s16 dL;

  s32 R;
  s16 dR;

  s32 S;
  s16 dS;

  s32 AuxAL;
  s16 dAuxAL;

  s32 AuxAR;
  s16 dAuxAR;

  s32 AuxAS;
  s16 dAuxAS;

  s32 AuxBL;
  s16 dAuxBL;

  s32 AuxBR;
  s16 dAuxBR;

  s32 AuxBS;
  s16 dAuxBS;

  s32 AuxCL;
  s16 dAuxCL;

  s32 AuxCR;
  s16 dAuxCR;

  s32 AuxCS;
  s16 dAuxCS;

  s32 Main0;
  s16 dMain0;

  s32 Aux0;
  s16 dAux0;

  s32 Main1;
  s16 dMain1;

  s32 Aux1;
  s16 dAux1;

  s32 Main2;
  s16 dMain2;

  s32 Aux2;
  s16 dAux2;

  s32 Main3;
  s16 dMain3;

  s32 Aux3;
  s16 dAux3;
} AXSTUDIO;
#pragma pack(pop)

AXSTUDIO *__AXGetStudio(void);
void __AXDepopFadeMain(s32 *all, s32 *value, s16 *depop) DONT_INLINE;
void __AXDepopFadeRmt(s32 *all, s32 *value, s16 *depop) DONT_INLINE;
void __AXPrintStudio(void);
void __AXSPBInit(void);
void __AXDepopVoice(AXPB *pb);

#ifdef __cplusplus
}
#endif
#endif
