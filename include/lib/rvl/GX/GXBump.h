#ifndef RVL_SDK_GX_BUMP_H
#define RVL_SDK_GX_BUMP_H
#include <lib/rvl/GX/GXTypes.h>
#include <lib/rvl/types.h>
#ifdef __cplusplus
extern "C" {
#endif

void GXSetTevIndirect(GXTevStageID tevStage, GXIndTexStageID texStage,
                      GXIndTexFormat texFmt, GXIndTexBiasSel biasSel,
                      GXIndTexMtxID mtxId, GXIndTexWrap wrapS,
                      GXIndTexWrap wrapT, GXBool addPrev, GXBool utcLod,
                      GXIndTexAlphaSel alphaSel);
void GXSetIndTexMtx(GXIndTexMtxID id, const f32 offset[2][3], s8 scaleExp);
void GXSetIndTexCoordScale(GXIndTexStageID stage, GXIndTexScale scaleS,
                           GXIndTexScale scaleT);
void GXSetIndTexOrder(GXIndTexStageID stage, GXTexCoordID coord,
                      GXTexMapID map);
void GXSetNumIndStages(u8 num);
void GXSetTevDirect(GXTevStageID stage);

void __GXUpdateBPMask(void);
void __GXSetIndirectMask(u32 mask);
void __GXFlushTextureState(void);

#ifdef __cplusplus
}
#endif
#endif
