#ifndef D_SND_BGM_SOUND_H
#define D_SND_BGM_SOUND_H

#include "d/snd/d_snd_sound.h"
#include "nw4r/snd/snd_SoundStartable.h"
#include "nw4r/ut/ut_list.h"

class dSndBgmSound_c : public dSndSound_c {
public:
    dSndBgmSound_c();

    virtual void init(nw4r::snd::SoundStartable &startable, s32 idx); // vt 0x28

    bool isPreparedSoundId(u32 soundId) const {
        return IsPrepared() && !mIsRunning && GetId() == soundId;
    }

private:
    /* 0x0F0 */ nw4r::ut::Node mBgmMgrNode;
};

class dSndBgmBattleSound_c : public dSndBgmSound_c {
public:
    dSndBgmBattleSound_c();
};

#endif
