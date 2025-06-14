#ifndef D_SND_SE_SOUND_POOL_H
#define D_SND_SE_SOUND_POOL_H

#include "common.h"
#include "d/snd/d_snd_se_sound_1.h"
#include "d/snd/d_snd_se_sound_2.h"
#include "d/snd/d_snd_util.h"
#include "nw4r/ut/ut_list.h"

SND_DISPOSER_FORWARD_DECL(dSndSeSoundPool_c);

class dSndSeSoundPool_c {
    SND_DISPOSER_MEMBERS(dSndSeSoundPool_c)

    static const u32 POOL_SIZE = 64;

public:
    dSndSeSoundPool_c();
    void calc();

    dSndSeSound1_c *acquireSoundType1(dSoundSource_c *source, u32 soundId);
    dSndSeSound2_c *acquireSoundType2(dSoundSource_c *source, u32 soundId);

    void releaseSoundType1(dSndSeSound1_c *sound, bool stop);
    void releaseSoundType2(dSndSeSound2_c *sound);

private:
    void removeSoundType1(dSndSeSound1_c *sound);
    void removeSoundType2(dSndSeSound2_c *sound);

    /* 0x0010 */ nw4r::ut::List mFreeType1List;
    /* 0x001C */ nw4r::ut::List mActiveType1List;
    /* 0x0028 */ nw4r::ut::List mFreeType2List;
    /* 0x0034 */ dSndSeSound1_c mSoundsType1[POOL_SIZE];
    /* 0x4A34 */ dSndSeSound2_c mSoundsType2[POOL_SIZE];
};

#endif
