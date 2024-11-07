#include <d/snd/mgr/d_snd_mgr_effect.h>

dSndMgrEffectBase_c::~dSndMgrEffectBase_c() {
    if (this == sInstanceSrc) {
        deref();
    }
}

dSndMgrEffectBase_c *dSndMgrEffectBase_c::create() {
    if (sInstance == nullptr) {
        dSndMgrEffect_c *mgr = new dSndMgrEffect_c();
        sInstance = mgr->base();
        sInstanceSrc = mgr->base();
    }

    return sInstance;
}

void dSndMgrEffectBase_c::deref() {
    sInstance = nullptr;
    sInstanceSrc = nullptr;
}

dSndMgrEffect_c::dSndMgrEffect_c() {
    field_0x38 = -1;
    field_0x3C = 0;
    field_0x40 = 0;
    field_0x42 = 0;
    field_0x44 = 0;
    field_0x28 = -1;
    field_0x30 = 0;
    field_0x2C = -1;
    field_0x34 = 0;
}
