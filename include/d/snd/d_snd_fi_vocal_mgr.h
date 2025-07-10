#ifndef D_SND_FI_VOCAL_MGR_H
#define D_SND_FI_VOCAL_MGR_H

#include "d/snd/d_snd_util.h"
#include "nw4r/snd/snd_SoundHandle.h"

SND_DISPOSER_FORWARD_DECL(dSndFiVocalMgr_c);

class dSndFiVocalMgr_c {
    SND_DISPOSER_MEMBERS(dSndFiVocalMgr_c)

public:
    dSndFiVocalMgr_c();

    void fiSpeak(const char *, u16, s32);
    void executeOutputText(u16, u16);

private:
    /* 0x10 */ nw4r::snd::SoundHandle mHandle;
};

#endif
