#ifndef D_SND_MGR_EFFECT_H
#define D_SND_MGR_EFFECT_H

#include <egg/core/eggDisposer.h>
#include <nw4r/snd/snd_SoundHandle.h>

class dSndMgrEffectBase_c : EGG::Disposer {
public:
    static dSndMgrEffectBase_c* create();
    static dSndMgrEffectBase_c *sInstance;

    dSndMgrEffectBase_c(): field_0x10(0), field_0x14(0), field_0x18(0) {

    }
    virtual ~dSndMgrEffectBase_c();

    static void deref();
    static dSndMgrEffectBase_c *sInstanceSrc;


    s32 field_0x10;
    s32 field_0x14;
    s32 field_0x18;
};

class dSndMgrEffect_c {
public:
    dSndMgrEffect_c();

    dSndMgrEffectBase_c* base() {
        return &mBase;
    }
private:
    dSndMgrEffectBase_c mBase;
    /* 0x1C */ nw4r::snd::SoundHandle mHandles[3];
    s32 field_0x28;
    s32 field_0x2C;
    s32 field_0x30;
    s32 field_0x34;
    s32 field_0x38;
    s32 field_0x3C;
    s16 field_0x40;
    s16 field_0x42;
    s32 field_0x44;
};

#endif
