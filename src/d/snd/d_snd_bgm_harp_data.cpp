#include "d/snd/d_snd_bgm_harp_data.h"

#include "common.h"
#include "nw4r/snd/snd_SeqSoundHandle.h"

dSndBgmDataHarpVarSetBase_c::dSndBgmDataHarpVarSetBase_c(s32 count) {
    mPosition = -1;
    mMax = count;
    mCount = 0;
    mpVars = new dSndBgmDataHarpVar_c[count];
    resetVars();
}

dSndBgmDataHarpVarSetBase_c::~dSndBgmDataHarpVarSetBase_c() {
    delete[] mpVars;
}

void dSndBgmDataHarpVarSetBase_c::resetVars() {
    mCount = 0;
    mPosition = -1;
    for (int i = 0; i < mMax; i++) {
        mpVars[i].reset();
    }
}

void dSndBgmDataHarpVarSetBase_c::addVar(u8 value) {
    if (mCount < mMax) {
        mpVars[mCount].field_0x01 = value;
        mCount++;
    }
}

dSndBgmHarpDataBase_c::dSndBgmHarpDataBase_c() : mMax(0), mCount(0), field_0x08(0) {}

void dSndBgmHarpDataBase_c::resetVars() {
    mCount = 0;
    field_0x08 = 0;
    for (int i = 0; i < mMax; i++) {
        mpVarSets[i].resetVars();
    }
}

s32 dSndBgmHarpDataBase_c::getIdxForPosition(s32 position) {
    if (position < 0) {
        return 0;
    }
    if (mpVarSets[0].getPosition() > position) {
        return 0;
    }

    for (int i = mCount - 1; i > 0; i--) {
        if (mpVarSets[i].getPosition() <= position) {
            return i;
        }
    }

    return 0;
}

void dSndBgmHarpDataBase_c::addVar(s32 position, s32 value, s32 unk) {
    if (position < 0) {
        return;
    }

    if (value <= 0) {
        return;
    }

    if (field_0x08 > 0) {
        return;
    }

    if (mCount > mMax) {
        return;
    }

    // Look at the set that might not be full yet
    dSndBgmDataHarpVarSetBase_c *set = getVarSet(mCount);
    if (set->getPosition() >= 0 && set->getPosition() < position) {
        // If it's not full but it doesn't match the position,
        // advance either way
        set++;
        mCount++;
    }

    // Set position for new set
    if (set->getPosition() < 0) {
        // Make sure set positions are strictly monotonically increasing (1),
        // do not record e.g. a fifth value for the same set
        if (mCount > 0 && set[-1].getPosition() >= position) {
            return;
        }
        set->setPosition(position);
    }

    dSndBgmDataHarpVar_c *var = set->getUnusedVar();
    if (var != nullptr) {
        if (unk == 0x7F) {
            var->onFlag();
        }
        set->addVar(value);

        if (set->getCount() >= set->getMax()) {
            // If the set is full, continue with the next set.
            // Condition (1) above will make sure that the next
            // set gets a strictly higher position, so there are
            // no sets with the same position.
            mCount++;
        }
    }
}

void dSndBgmHarpDataBase_c::setField_0x08(s32 value) {
    if (value < 0) {
        value = 0;
    }
    field_0x08 = value;
}

dSndBgmDataHarpVarSetBase_c *dSndBgmHarpDataBase_c::getUsableVarSet(s32 idx) {
    if (idx < 0) {
        return nullptr;
    }

    if (idx >= mCount) {
        return nullptr;
    }

    return &mpVarSets[idx];
}

dSndBgmDataHarpVarSetBase_c *dSndBgmHarpDataBase_c::getVarSet(s32 idx) {
    if (idx < 0) {
        return nullptr;
    }

    if (idx >= mMax) {
        return nullptr;
    }

    return &mpVarSets[idx];
}

dSndBgmHarpData_c::dSndBgmHarpData_c() {
    mpVarSets = new dSndBgmDataHarpVarSet_c[NUM_SETS];
    mMax = NUM_SETS;
    resetVars();
}

dSndBgmHarpData_c::~dSndBgmHarpData_c() {
    delete[] mpVarSets;
}

void dSndBgmHarpData_c::writeSeqVars(s32 idx) {
    if (idx < 0) {
        return;
    }
    if (idx >= mCount) {
        return;
    }

    for (int i = 0; i < mpVarSets[idx].getCount(); i++) {
        nw4r::snd::SeqSoundHandle::WriteGlobalVariable(10 + i, mpVarSets[idx].getVar(i)->field_0x01);
    }
}
