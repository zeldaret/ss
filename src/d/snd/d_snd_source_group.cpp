
#include "d/snd/d_snd_source_group.h"

#include "common.h"
#include "d/snd/d_snd_source.h"
#include "nw4r/ut/ut_list.h"

#include <cmath>

extern "C" bool isInStage(const char *stageName);

void dSndSourceGroup_c::set(s32 sourceType, const char *name) {
    // TODO enums
    resetSoundSourceParam();
    bool assignedParam = false;

    switch (mSourceCategory) {
        case 0: {
            assignedParam = true;
            mParam.field_0x10 = INFINITY;
            break;
        }
        case 1:
            if (sourceType == 7) {
                assignedParam = true;
                mParam.field_0x00 = 1500.0f;
                mParam.field_0x10 = INFINITY;
            }
            break;
    }

    if (!assignedParam && (sourceType == 44 || sourceType == 58)) {
        assignedParam = true;
        mParam.field_0x10 = INFINITY;
    }

    if (!assignedParam) {
        assignedParam = setParamFromName(name);
    }

    if (!assignedParam) {
        switch (sourceType) {
            case 51: {
                mParam.field_0x00 = 300.0f;
                mParam.field_0x04 = 800.0;
                mParam.field_0x10 = 2200.0;
                break;
            }
            case 48: {
                if (isInStage("F401")) {
                    mParam.field_0x00 = 500.0f;
                    mParam.field_0x04 = 4000.0;
                } else if (isInStage("F402")) {
                    mParam.field_0x00 = 400.0f;
                    mParam.field_0x04 = 3500.0;
                } else {
                    mParam.field_0x00 = 300.0f;
                    mParam.field_0x04 = 800.0;
                    mParam.field_0x10 = 2200.0;
                }

                mParam.field_0x10 = INFINITY;
            }
        }
    }
}

void dSndSourceGroup_c::resetSoundSourceParam() {
    mParam.reset();
}

dSndSourceGroup_c::dSndSourceGroup_c()
    : mSubtype(0),
      field_0x1C(0),
      field_0x1D(0),
      mName(""),
      field_0x40(0),
      mpCachedClosestSourceToListener(nullptr),
      mpCachedClosestSourceToPlayer(nullptr),
      mCalculatedClosestToListener(false),
      mCalculatedClosestToPlayer(false) {
    // TODO offsetof
    nw4r::ut::List_Init(&mSourceList, 0x138);
    resetSoundSourceParam();
}

dSndSourceGroup_c::~dSndSourceGroup_c() {
    clearList();
}

void dSndSourceGroup_c::calc() {
    // Reset freshness flags, computed on-demand
    mCalculatedClosestToPlayer = false;
    mCalculatedClosestToListener = false;
}

s32 dSndSourceGroup_c::getNumSources() const {
    return nw4r::ut::List_GetSize(&mSourceList);
}

void dSndSourceGroup_c::registerSource(dSoundSource_c *source) {
    if (source != nullptr) {
        nw4r::ut::List_Append(&mSourceList, source);
    }
}

void dSndSourceGroup_c::unregisterSource(dSoundSource_c *source) {
    if (source != nullptr) {
        nw4r::ut::List_Remove(&mSourceList, source);
    }
}

void dSndSourceGroup_c::clearList() {
    dSoundSource_c *source = static_cast<dSoundSource_c *>(nw4r::ut::List_GetFirst(&mSourceList));
    while (source != nullptr) {
        nw4r::ut::List_Remove(&mSourceList, source);
        source = static_cast<dSoundSource_c *>(nw4r::ut::List_GetFirst(&mSourceList));
    }
}

dSoundSource_c *dSndSourceGroup_c::getSourceClosestToListener() {
    if (mCalculatedClosestToListener) {
        return mpCachedClosestSourceToListener;
    }

    mpCachedClosestSourceToListener = nullptr;
    f32 closest = INFINITY;
    for (dSoundSource_c *source = static_cast<dSoundSource_c *>(nw4r::ut::List_GetFirst(&mSourceList));
         source != nullptr; source = static_cast<dSoundSource_c *>(nw4r::ut::List_GetNext(&mSourceList, source))) {
        if (source->getActorType() != 1 && source->getActorType() != 48) {
            f32 dist = source->getDistanceToListener();
            if (dist < closest) {
                closest = dist;
                mpCachedClosestSourceToListener = source;
            }
        }
    }
    mCalculatedClosestToListener = true;
    return mpCachedClosestSourceToListener;
}

dSoundSource_c *dSndSourceGroup_c::getSourceClosestToPlayer() {
    if (mCalculatedClosestToPlayer) {
        return mpCachedClosestSourceToPlayer;
    }

    if ((s32)nw4r::ut::List_GetSize(&mSourceList) <= 1) {
        return static_cast<dSoundSource_c *>(nw4r::ut::List_GetFirst(&mSourceList));
    }

    mpCachedClosestSourceToPlayer = nullptr;
    f32 closest = INFINITY;
    for (dSoundSource_c *source = static_cast<dSoundSource_c *>(nw4r::ut::List_GetFirst(&mSourceList));
         source != nullptr; source = static_cast<dSoundSource_c *>(nw4r::ut::List_GetNext(&mSourceList, source))) {
        if (source->getActorType() != 1 && source->getActorType() != 48) {
            f32 dist = source->getDistanceToPlayer();
            if (dist < closest) {
                closest = dist;
                mpCachedClosestSourceToPlayer = source;
            }
        }
    }
    mCalculatedClosestToPlayer = true;
    return mpCachedClosestSourceToPlayer;
}

bool dSndSourceGroup_c::setParamFromName(const char*) {
    // TODO
    return false;
}
