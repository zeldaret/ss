#include "toBeSorted/sound_info.h"

#include "common.h"
#include "d/a/d_a_base.h"
#include "d/col/bg/d_bg_s.h"

SoundInfo::SoundInfo(dAcBase_c *ac) : mpActor(ac) {}

SoundInfo::~SoundInfo() {
    if (mpActor != nullptr) {
        mpActor->removeSoundInfo(this);
        mpActor = nullptr;
    }
}

bool SoundInfo::initSource(s32 sourceType, const char *name, const mVec3_c *posPtr) {
    mpSource = dSoundSourceIf_c::create(sourceType, mpActor, name, 0);
    if (mpSource == nullptr) {
        return false;
    }
    mpActor->mSoundList.append(this);
    mpPosPtr = posPtr;
    return true;
}

bool SoundInfo::startBgHitSound(u32 soundId, const cBgS_PolyInfo &info, const mVec3_c *position) {
    return mpSource->startBgHitSound(
        soundId, dBgS::GetInstance()->GetPolyAtt0(info), dBgS::GetInstance()->GetPolyAtt1(info),
        position != nullptr ? position : &mpActor->mPosition
    );
}
