#ifndef SOUND_INFO_H
#define SOUND_INFO_H

#include "m/m_vec.h"
#include "d/snd/d_snd_source_if.h"
#include "toBeSorted/raii_ptr.h"
#include "toBeSorted/tlist.h"

class dAcBase_c;
class cBgS_PolyInfo;

class SoundInfo {
private:
    /* 0x00 */ dAcBase_c *mpActor;
    /* 0x04 */ RaiiPtr<dSoundSourceIf_c> mpSource;
    /* 0x08 */ const mVec3_c *mpPosPtr;
    /* 0x0C */ TListNode<SoundInfo> mLink;
    
public:
    SoundInfo(dAcBase_c *ac);
    /* vtable at 0x14 */
    virtual ~SoundInfo();

    dSoundSourceIf_c *getSource() {
        return mpSource.get();
    }

    void calc() {
        mpSource->calc(*mpPosPtr);
    }

    bool initSource(u8 sourceType, const char *name, const mVec3_c *posPtr);
    bool startBgHitSound(u32 soundId, const cBgS_PolyInfo &info, const mVec3_c *position);
};

#endif
