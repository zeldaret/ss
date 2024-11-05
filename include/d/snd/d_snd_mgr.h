#ifndef D_SND_MGR_H
#define D_SND_MGR_H

#include <egg/audio/eggAudioMgr.h>

class dSndMgr_c : EGG::SimpleAudioMgr {
public:
    dSndMgr_c();

    virtual void calc() override;
    void initialize(EGG::Heap* heap, u32 size);
    void initHbm(u32 frame);

    static void restoreEffectsCallback();

    static dSndMgr_c *sInstance;
};


#endif
