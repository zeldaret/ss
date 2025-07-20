#ifndef D_SND_BGM_SOUND_HARP_MGR_H
#define D_SND_BGM_SOUND_HARP_MGR_H

#include "common.h"
#include "d/snd/d_snd_bgm_harp_data.h"

class dSndBgmSoundHarpMgr_c {
public:
    dSndBgmSoundHarpMgr_c();
    ~dSndBgmSoundHarpMgr_c() {}

    void setSoundId(u32 soundId);
    void setLoaded();
    void reset();

    void setPlaySamplePosition(s32 position);

    dSndBgmDataHarpVarSetBase_c *getCurrentVarSet();

    bool isLoaded() const {
        return mIsLoaded;
    }

    void setField_0x08(s32 value) {
        mHarpData.setField_0x08(value);
    }

    void setTempo(s32 tempo) {
        mTempo = tempo;
    }

    s32 getTempo() const {
        return mTempo;
    }

    void addVar(int wait, int key, int velocity) {
        mHarpData.addVar(wait, key, velocity);
    }

    void resetPrevIdx() {
        mPrevIdx = -1;
    }

    u32 getSoundId() const {
        return mSoundId;
    }

private:
    /* 0x00 */ dSndBgmHarpData_c mHarpData;
    /* 0x0C */ dSndBgmDataHarpVarSetBase_c *mpCurrVarSet;
    /* 0x10 */ u32 mSoundId;
    /* 0x14 */ s32 mTempo;
    /* 0x18 */ s32 mPrevIdx;
    /* 0x1C */ u8 field_0x1C;
    /* 0x1D */ bool mIsLoaded;
};

#endif
