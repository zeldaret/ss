#ifndef D_SND_BGM_SOUND_HARP_MGR_H
#define D_SND_BGM_SOUND_HARP_MGR_H

#include "common.h"
#include "d/snd/d_snd_bgm_harp_data.h"

class dSndBgmSoundHarpMgr_c : public dSndBgmHarpData_c {
public:
    dSndBgmSoundHarpMgr_c();
    ~dSndBgmSoundHarpMgr_c() {}

    void setSoundId(u32 soundId);
    void setLoaded();
    void reset();

    void setPlaySamplePosition(s32 position);

    const dSndBgmDataHarpVarSetBase_c *getCurrentVarSet();

    bool isLoaded() const {
        return mIsLoaded;
    }

private:
    /* 0x0C */ dSndBgmDataHarpVarSetBase_c *mpCurrVarSet;
    /* 0x10 */ u32 mSoundId;
    /* 0x14 */ UNKWORD field_0x14;
    /* 0x18 */ s32 mPrevIdx;
    /* 0x1C */ u8 field_0x1C;
    /* 0x1D */ bool mIsLoaded;
};

#endif
