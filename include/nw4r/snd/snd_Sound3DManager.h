#ifndef NW4R_SND_SOUND_3D_MANAGER_H
#define NW4R_SND_SOUND_3D_MANAGER_H
#include <nw4r/types_nw4r.h>

#include <nw4r/snd/snd_BasicSound.h>
#include <nw4r/snd/snd_InstancePool.h>
#include <nw4r/snd/snd_SoundArchive.h>

#include <nw4r/math.h>

namespace nw4r {
namespace snd {

class Sound3DManager : public detail::BasicSound::AmbientParamUpdateCallback,
                       public detail::BasicSound::AmbientArgAllocaterCallback {
public:
    struct Sound3DActorParam {
        u32 userParam;                         // at 0x0
        SoundArchive::Sound3DParam soundParam; // at 0x4
        math::VEC3 position;                   // at 0xC

        Sound3DActorParam();
    };

public:
    Sound3DManager();

    virtual void detail_Update(SoundParam* pParam, u32 id,
                               detail::BasicSound* pSound, const void* pArg,
                               u32 flags); // at 0x8

    virtual void Update(SoundParam* pParam, u32 id, SoundHandle* pHandle,
                        const void* pArg,
                        u32 flags); // at 0x10

    virtual void* detail_AllocAmbientArg(u32 size); // at 0x14

    virtual void
    detail_FreeAmbientArg(void* pArg,
                          const detail::BasicSound* pSound); // at 0x18

    u32 GetRequiredMemSize(const SoundArchive* pArchive);
    bool Setup(const SoundArchive* pArchive, void* pBuffer, u32 size);

    Sound3DListener* GetListener() const {
        return mListener;
    }

    int GetMaxPriorityReduction() const {
        return mMaxPriorityReduction;
    }
    void SetMaxPriorityReduction(int max) {
        mMaxPriorityReduction = max;
    }

    f32 GetPanRange() const {
        return mPanRange;
    }

private:
    enum ParamDecayCurve {
        DECAY_CURVE_NONE,
        DECAY_CURVE_LOGARITHMIC,
        DECAY_CURVE_LINEAR,
    };

private:
    detail::InstancePool<Sound3DActorParam> mParamPool; // at 0x8
    Sound3DListener* mListener;                         // at 0xC
    s32 mMaxPriorityReduction;                          // at 0x10

    f32 mSpeakerAngleStereo;    // at 0x14
    f32 mFrontSpeakerAngleDpl2; // at 0x18
    f32 mRearSpeakerAngleDpl2;  // at 0x1C

    f32 mInitPan;  // at 0x20
    f32 mPanRange; // at 0x24
};

} // namespace snd
} // namespace nw4r

#endif
