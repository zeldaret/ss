#include "d/snd/d_snd_se_sound.h"

#include "common.h"
#include "d/snd/d_snd_source.h"

dSndSeSound_c::dSndSeSound_c()
    : mpSource(nullptr),
      mpSoundHandle(&mSound),
      mHandleType(SE_HANDLE_TYPE_1),
      mSoundId(-1),
      field_0x114(-1),
      field_0x11C(1),
      field_0x11D(0),
      field_0x11E(0),
      mpStartCallback(nullptr),
      mpCalcCallback(nullptr) {
    mpUnk = new dSndSoundCtxParam[10];
}

dSndSeSound_c::~dSndSeSound_c() {
    delete[] mpUnk;
}

void dSndSeSound_c::setSource(dSoundSource_c *source, u32 soundId) {
    clear();
    mSoundId = soundId;
    field_0x114 = 0;
    dSndSoundCtxParam* s = mpUnk;
    while (s < &mpUnk[10]) {
        s->intParam = 0;
        s->floatParam = 0.0f;
        s++;
    }

    mpCalcCallback = nullptr;
    mpStartCallback = nullptr;
    field_0x11D = 0;
    field_0x11E = 0;
}

void dSndSeSound_c::clear() {
    field_0x114 = -1;
    mpSource = nullptr;
    mpCalcCallback = nullptr;
    mpStartCallback = nullptr;
    mpSoundHandle = &mSound;
    field_0x11C = 1;
    field_0x11D = 0;
    mSound.cancel();
}

void dSndSeSound_c::calc(dSoundSource_c *source) {
    onCalc(source);
    if (mpSoundHandle->IsPause() && mpSoundHandle->GetRemainingFadeFrames() == 0) {
        return;
    }
    field_0x114++;
}

void dSndSeSound_c::shiftParam(const dSndSoundCtxParam *pUnk) {
    if (pUnk == nullptr) {
        return;
    }

    for (int i = 9; i > 0; i--) {
        mpUnk[i] = mpUnk[i - 1];
    }
    mpUnk[0] = *pUnk;
}

void dSndSeSound_c::shiftFloatParam(f32 value) {
    for (int i = 9; i > 0; i--) {
        mpUnk[i].floatParam = mpUnk[i - 1].floatParam;
    }
    mpUnk[0].floatParam = value;
}

void dSndSeSound_c::onStart(dSoundSource_c *source) {
    if (mpStartCallback != nullptr) {
        (mpStartCallback)(this, source, *mpSoundHandle);
    }
}

void dSndSeSound_c::onCalc(dSoundSource_c *source) {
    if (mpCalcCallback != nullptr) {
        (mpCalcCallback)(this, source, *mpSoundHandle);
    }
}
