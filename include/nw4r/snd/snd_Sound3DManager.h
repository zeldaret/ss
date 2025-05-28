#ifndef NW4R_SND_SOUND_3D_MANAGER_H
#define NW4R_SND_SOUND_3D_MANAGER_H
#include "nw4r/snd/snd_BasicSound.h"
#include "nw4r/snd/snd_InstancePool.h"
#include "nw4r/snd/snd_Sound3DEngine.h"
#include "nw4r/snd/snd_SoundArchive.h"
#include "nw4r/types_nw4r.h"
#include "nw4r/ut/ut_LinkList.h"

#include "nw4r/math.h"

namespace nw4r {
namespace snd {

struct Sound3DParam {
    /* 0x00 */ math::VEC3 position;
    /* 0x0C */ math::VEC3 field_0x0C;
    /* 0x18 */ UNKWORD field_0x18;
    /* 0x1C */ u8 decayCurve;
    /* 0x1D */ u8 decayRatio;
    /* 0x1E */ u8 field_0x1E;
    /* 0x20 */ UNKWORD field_0x20;
    /* 0x24 */ UNKWORD field_0x24;

    Sound3DParam();
};

class Sound3DManager : public detail::BasicSound::AmbientInfo::AmbientParamUpdateCallback,
                       public detail::BasicSound::AmbientInfo::AmbientArgAllocaterCallback {
public:
    typedef ut::LinkList<Sound3DListener, 0x64> ListenerList;

    Sound3DManager();

    virtual void detail_UpdateAmbientParam(const void*, u32, int, SoundAmbientParam*) override;
    virtual int detail_GetAmbientPriority(const void*, u32) override;
    virtual int detail_GetRequiredVoiceOutCount(const void*, u32) override;

    virtual void *detail_AllocAmbientArg(u32 size) override;

    virtual void detail_FreeAmbientArg(void *pArg,
                                       const detail::BasicSound *pSound) override;

    u32 GetRequiredMemSize(const SoundArchive *pArchive);
    bool Setup(const SoundArchive *pArchive, void *pBuffer, u32 size);

    const ListenerList &GetListenerList() const {
        return mListenerList;
    }

    ListenerList &GetListenerList() {
        return mListenerList;
    }

    int GetMaxPriorityReduction() const {
        return mMaxPriorityReduction;
    }
    void SetMaxPriorityReduction(int max) {
        mMaxPriorityReduction = max;
    }

    int GetBiquadFilterType() const {
        return mBiquadFilterType;
    }
    void SetBiquadFilterType(int type);

    f32 GetField0x20() const {
        return field_0x20;
    }

    f32 GetField0x24() const {
        return field_0x24;
    }

    void SetEngine(Sound3DEngine *engine);

    enum ParamDecayCurve {
        DECAY_CURVE_NONE,
        DECAY_CURVE_LOGARITHMIC,
        DECAY_CURVE_LINEAR,
    };

private:
    detail::InstancePool<Sound3DParam> mParamPool; // at 0x8
    ListenerList mListenerList;                    // at 0x0C
    Sound3DEngine *mpEngine;                       // at 0x18
    s32 mMaxPriorityReduction;                     // at 0x1C
    f32 field_0x20;                                // at 0x20
    f32 field_0x24;                                // at 0x24
    int mBiquadFilterType;                          // at 0x28
};

} // namespace snd
} // namespace nw4r

#endif
