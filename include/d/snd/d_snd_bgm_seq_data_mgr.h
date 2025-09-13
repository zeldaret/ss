#ifndef D_SND_BGM_SEQ_DATA_MGR_H
#define D_SND_BGM_SEQ_DATA_MGR_H

#include "common.h"
#include "d/snd/d_snd_types.h"
#include "d/snd/d_snd_util.h"

SND_DISPOSER_FORWARD_DECL(dSndBgmSeqDataMgr_c);

// Previous Ghidra name: SomeUnusedSoundMgr
class dSndBgmSeqDataMgr_c {
public:
    SND_DISPOSER_MEMBERS(dSndBgmSeqDataMgr_c);

public:
    dSndBgmSeqDataMgr_c();

    static const char *getDataPrefix();
    static u32 getDataPrefixLength();

    dSndBgmSoundHarpMgr_c *getHarpMgrForSoundId(u32 soundId);
    void setup();
    void setupState0();

private:
    static const s32 NUM_SOUNDS = 2;

    /* 0x10 */ dSndBgmSoundHarpMgr_c *mpMgrs;
    /* 0x14 */ bool mInitialized;
};

#endif
