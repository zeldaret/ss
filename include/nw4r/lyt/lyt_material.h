#ifndef NW4R_LYT_MATERIAL_H
#define NW4R_LYT_MATERIAL_H
#include "nw4r/lyt/lyt_animation.h"
#include "nw4r/lyt/lyt_resources.h"
#include "nw4r/lyt/lyt_texMap.h"
#include "nw4r/ut/ut_Color.h"
#include "nw4r/ut/ut_LinkList.h"
#include "rvl/GX.h" // IWYU pragma: export

namespace nw4r {
namespace lyt {

class Material {
public:
    Material();
    Material(const res::Material *pRes, const ResBlockSet &resBlockSet);

    void Init();
    static void InitBitGXNums(detail::BitGXNums *ptr);
    void ReserveGXMem(
        u8 texMapNum, u8 texSRTNum, u8 texCoordGenNum, u8 tevStageNum, bool allocTevSwap, u8 indStageNum, u8 indSRTNum,
        bool allocChanCtrl, bool allocMatCol, bool allocAlpComp, bool allocBlendMode
    );

    const TexMap *GetTexMapAry() const;
    TexMap *GetTexMapAry();
    const TexSRT *GetTexSRTAry() const;
    TexSRT *GetTexSRTAry();
    const TexCoordGen *GetTexCoordGenAry() const;
    TexCoordGen *GetTexCoordGenAry();
    const ChanCtrl *GetChanCtrlAry() const;
    ChanCtrl *GetChanCtrlAry();
    const ut::Color *GetMatColAry() const;
    ut::Color *GetMatColAry();
    const TevSwapMode *GetTevSwapAry() const;
    TevSwapMode *GetTevSwapAry();
    const AlphaCompare *GetAlphaComparePtr() const;
    AlphaCompare *GetAlphaComparePtr();
    const BlendMode *GetBlendModePtr() const;
    BlendMode *GetBlendModePtr();
    const IndirectStage *GetIndirectStageAry() const;
    IndirectStage *GetIndirectStageAry();
    const TexSRT *GetIndTexSRTAry() const;
    TexSRT *GetIndTexSRTAry();
    const TevStage *GetTevStageAry() const;
    TevStage *GetTevStageAry();

    void SetName(const char *name);
    void SetTextureNum(u8 val);
    void SetTexCoordGenNum(u8 val);
    void SetTevStageNum(u8 val);
    void SetIndStageNum(u8 val);
    s16 GetColorElement(u32 colorType);
    void SetColorElement(u32 colorType, s16 value);
    void AddAnimationLink(AnimationLink *pAnimationLink);

    // IsBlendModeCap__Q34nw4r3lyt8MaterialCFv
    bool IsBlendModeCap() const {
        return mGXMemCap.blendMode;
    }

    // IsAlphaCompareCap__Q34nw4r3lyt8MaterialCFv
    bool IsAlphaCompareCap() const {
        return mGXMemCap.alpComp;
    }

    // IsTevSwapCap__Q34nw4r3lyt8MaterialCFv
    bool IsTevSwapCap() const {
        return mGXMemCap.tevSwap;
    }

    // IsMatColorCap__Q34nw4r3lyt8MaterialCFv
    bool IsMatColorCap() const {
        return mGXMemCap.matCol;
    }

    // IsChanCtrlCap__Q34nw4r3lyt8MaterialCFv
    bool IsChanCtrlCap() const {
        return mGXMemCap.chanCtrl;
    }

    // SetTexSRTElement__Q34nw4r3lyt8MaterialFUlUlf
    void SetTexSRTElement(u32 texSRTIdx, u32 eleIdx, f32 value) {
        f32 *srtAry = (f32 *)&(GetTexSRTAry()[texSRTIdx]);
        srtAry[eleIdx] = value;
    }

    // GetTexturePtr__Q34nw4r3lyt8MaterialFUc
    TexMap *GetTexturePtr(u8 idx) {
        return &GetTexMapAry()[idx];
    }

    // GetTexSRTCap__Q34nw4r3lyt8MaterialCFv
    u8 GetTexSRTCap() const {
        return mGXMemCap.texSRT;
    }

    // GetIndTexSRTCap__Q34nw4r3lyt8MaterialCFv
    u8 GetIndTexSRTCap() const {
        return mGXMemCap.indSRT;
    }

    // SetIndTexSRTElement__Q34nw4r3lyt8MaterialFUlUlf
    void SetIndTexSRTElement(u32 texSRTIdx, u32 eleIdx, f32 value) {
        f32 *srtAry = (f32 *)&(GetIndTexSRTAry()[texSRTIdx]);
        srtAry[eleIdx] = value;
    }

    ut::LinkList<AnimationLink, 0> *GetAnimationList() {
        return &mAnimList;
    }
    bool IsUserAllocated() const {
        return mbUserAllocated;
    }
    const char *GetName() const {
        return mName;
    }

    u8 GetTexCoordGenCap() const {
        return mGXMemCap.texCoordGen;
    }
    u8 GetTexCoordGenNum() const {
        return mGXMemNum.texCoordGen;
    }
    u8 GetTextureCap() const {
        return mGXMemCap.texMap;
    }

    u8 GetTextureNum() const {
        return mGXMemNum.texMap;
    }

    GXColorS10 GetTevColor(u32 idx) const {
        return mTevCols[idx];
    }

    ut::Color GetTevKColor(u32 idx) const {
        return mTevKCols[idx];
    }

    void SetTexture(u32 texMapIdx, const TexMap &texMap) {
        GetTexMapAry()[texMapIdx] = texMap;
    }

    void SetTexCoordGen(u32 idx, TexCoordGen value) {
        GetTexCoordGenAry()[idx] = value;
    }

    // Begin Functions not found in DWARF, but assumed from BBA map and required for weak func ordering
    void SetTevColor(u32 idx, const GXColorS10 &color) {
        mTevCols[idx] = color;
    }

    void SetAlphaCompare(const AlphaCompare &alphaComp) {
        *GetAlphaComparePtr() = alphaComp;
    }

    void SetChanControl(u32 idx, const ChanCtrl &chanCtrl) {
        GetChanCtrlAry()[idx] = chanCtrl;
    }

    void SetTevKColor(u32 idx, const ut::Color &color) {
        mTevKCols[idx] = color;
    }

    void SetTevSwapMode(u32 idx, const TevSwapMode &tevSwapMode) {
        GetTevSwapAry()[idx] = tevSwapMode;
    }

    void SetTexSRT(u32 idx, const TexSRT &texSRT) {
        GetTexSRTAry()[idx] = texSRT;
    }

    void SetIndirectStage(u32 idx, const IndirectStage &indStage) {
        GetIndirectStageAry()[idx] = indStage;
    }

    void SetBlendMode(const BlendMode &blendMode) {
        *GetBlendModePtr() = blendMode;
    }

    void SetTevStage(u32 idx, const TevStage &tevStage) {
        GetTevStageAry()[idx] = tevStage;
    }
    // End Functions not found in DWARF, but assumed from BBA map and required for weak func ordering

    virtual ~Material();                                                        // at 0x08
    virtual bool SetupGX(bool bModVtxCol, u8 alpha);                            // at 0x0C
    virtual void BindAnimation(AnimTransform *pAnimTrans);                      // at 0x10
    virtual void UnbindAnimation(AnimTransform *pAnimTrans);                    // at 0x14
    virtual void UnbindAllAnimation();                                          // at 0x18
    virtual void Animate();                                                     // at 0x1C
    virtual AnimationLink *FindAnimationLink(AnimTransform *pAnimTrans);        // at 0x20
    virtual AnimationLink *FindAnimationLink(const AnimResource &animRes);      // at 0x24
    virtual void SetAnimationEnable(AnimTransform *pAnimTrans, bool bEnable);   // at 0x28
    virtual void SetAnimationEnable(const AnimResource &animRes, bool bEnable); // at 0x2C

private:
    ut::LinkList<AnimationLink, 0> mAnimList; // at 0x4
    GXColorS10 mTevCols[3];                   // at 0x10
    ut::Color mTevKCols[4];                   // at 0x28
    detail::BitGXNums mGXMemCap;              // at 0x38
    detail::BitGXNums mGXMemNum;              // at 0x3C
    void *mpGXMem;                            // at 0x40
    char mName[MATERIAL_NAME_SIZE + 1];       // at 0x44
    bool mbUserAllocated;                     // at 0x59
    u8 mPadding[2];                           // at 0x5A
};

namespace detail {

Size GetTextureSize(Material *pMaterial, u8 texMapIdx);

} // namespace detail

} // namespace lyt
} // namespace nw4r

#endif
