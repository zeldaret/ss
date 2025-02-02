#ifndef NW4R_G3D_ANM_OBJ_H
#define NW4R_G3D_ANM_OBJ_H
#include <nw4r/types_nw4r.h>

#include <nw4r/g3d/g3d_obj.h>
#include <nw4r/g3d/res/g3d_resanm.h>

namespace nw4r {
namespace g3d {

/******************************************************************************
 *
 * AnmObj
 *
 ******************************************************************************/
class AnmObj : public G3dObj {
    NW4R_G3D_RTTI_DECL_DERIVED(AnmObj, G3dObj);

public:
    enum AnmFlag {
        FLAG_NEED_UPDATE = (1 << 0),
        FLAG_CACHE_OBSOLETE = (1 << 1),
        FLAG_ANM_BOUND = (1 << 2),
        FLAG_USE_QUATERNION_ROTATION_BLEND = (1 << 3),
        FLAG_USE_ACCURATE_SCALE_BLEND = (1 << 4)
    };

public:
    AnmObj(MEMAllocator *pAllocator, G3dObj *pParent) : G3dObj(pAllocator, pParent), mFlags(0) {}

    virtual void G3dProc(u32 task, u32 param, void *pInfo) = 0; // at 0xC
    virtual ~AnmObj() {}                                        // at 0x10

    virtual void SetFrame(f32 frame) = 0; // at 0x1C
    virtual f32 GetFrame() const = 0;     // at 0x20
    virtual void UpdateFrame() = 0;       // at 0x24

    virtual void SetUpdateRate(f32 rate) = 0; // at 0x28
    virtual f32 GetUpdateRate() const = 0;    // at 0x2C

    virtual bool Bind(const ResMdl mdl) = 0; // at 0x30
    virtual void Release();                  // at 0x34

    void SetAnmFlag(AnmFlag flag, bool value);
    bool TestAnmFlag(AnmFlag flag) const;

    bool IsBound() const {
        return TestAnmFlag(FLAG_ANM_BOUND);
    }

private:
    u32 mFlags; // at 0xC
};

/******************************************************************************
 *
 * PlayPolicy
 *
 ******************************************************************************/
f32 PlayPolicy_Onetime(f32 start, f32 end, f32 frame);
f32 PlayPolicy_Loop(f32 start, f32 end, f32 frame);

typedef f32 (*PlayPolicyFunc)(f32 start, f32 end, f32 frame);

inline PlayPolicyFunc GetAnmPlayPolicy(AnmPolicy policy) {
    static PlayPolicyFunc policyTable[ANM_POLICY_MAX] = {PlayPolicy_Onetime, PlayPolicy_Loop};
    return policyTable[policy];
}

/******************************************************************************
 *
 * FrameCtrl
 *
 ******************************************************************************/
class FrameCtrl {
public:
    FrameCtrl(f32 start, f32 end, PlayPolicyFunc pPolicy)
        : mFrame(0.0f), mUpdateRate(1.0f), mStartFrame(start), mEndFrame(end), mpPlayPolicy(pPolicy) {}

    f32 GetFrm() const {
        return mFrame;
    }
    void SetFrm(f32 frame) {
        mFrame = mpPlayPolicy(mStartFrame, mEndFrame, frame);
    }
    void UpdateFrm() {
        SetFrm(mUpdateRate * smBaseUpdateRate + mFrame);
    }

    f32 GetRate() const {
        return mUpdateRate;
    }
    void SetRate(f32 rate) {
        mUpdateRate = rate;
    }

    void SetPlayPolicy(PlayPolicyFunc func) {
        mpPlayPolicy = func;
    }

private:
    f32 mFrame;                  // at 0x0
    f32 mUpdateRate;             // at 0x4
    f32 mStartFrame;             // at 0x8
    f32 mEndFrame;               // at 0xC
    PlayPolicyFunc mpPlayPolicy; // at 0x10

    static f32 smBaseUpdateRate;
};

} // namespace g3d
} // namespace nw4r

#endif
