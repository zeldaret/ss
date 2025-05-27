#ifndef NW4R_SND_SOUND_3D_MANAGER_H
#define NW4R_SND_SOUND_3D_MANAGER_H
#include "nw4r/math.h"
#include "nw4r/snd/snd_BasicSound.h"
#include "nw4r/snd/snd_InstancePool.h"
#include "nw4r/snd/snd_SoundArchive.h"
#include "nw4r/types_nw4r.h"


namespace nw4r {
namespace snd {

struct Sound3DParam
{
    /* 0x00 */ u8 _0x00[0x18];
    /* 0x18 */ UNKWORD field_0x18;
    /* 0x1C */ u8 field_0x1C;
    /* 0x1D */ u8 field_0x1D;
    /* 0x1E */ u8 field_0x1E;
    /* 0x20 */ UNKWORD field_0x20;
    /* 0x24 */ UNKWORD field_0x24;

    Sound3DParam();
};

class Sound3DManager : public detail::BasicSound::AmbientInfo::AmbientParamUpdateCallback,
                       public detail::BasicSound::AmbientInfo::AmbientArgAllocaterCallback {
public:
    struct Sound3DActorParam {
        u32 userParam;                         // at 0x0
        SoundArchive::Sound3DParam soundParam; // at 0x4
        math::VEC3 position;                   // at 0xC

        Sound3DActorParam();
    };

public:
    Sound3DManager();

    virtual void detail_Update(
        SoundParam *pParam, u32 id, detail::BasicSound *pSound, const void *pArg,
        u32 flags
    ); // at 0x8

    virtual void Update(SoundParam *pParam, u32 id, SoundHandle *pHandle, const void *pArg,
                        u32 flags); // at 0x10

    virtual void *detail_AllocAmbientArg(u32 size); // at 0x14

    virtual void detail_FreeAmbientArg(void *pArg,
                                       const detail::BasicSound *pSound); // at 0x18

    u32 GetRequiredMemSize(const SoundArchive *pArchive);
    bool Setup(const SoundArchive *pArchive, void *pBuffer, u32 size);

    Sound3DListener *GetListener() const {
        return mListener;
    }

    int GetMaxPriorityReduction() const {
        return mMaxPriorityReduction;
    }
    void SetMaxPriorityReduction(int max) {
        mMaxPriorityReduction = max;
    }

private:
    enum ParamDecayCurve {
        DECAY_CURVE_NONE,
        DECAY_CURVE_LOGARITHMIC,
        DECAY_CURVE_LINEAR,
    };

private:
    detail::InstancePool<Sound3DParam> mParamPool; // at 0x8
    Sound3DListener *mListener;                         // at 0xC
    u8 _0x10[0x1C - 0x10];
    s32 mMaxPriorityReduction;                          // at 0x1C
};

} // namespace snd
} // namespace nw4r

#endif
