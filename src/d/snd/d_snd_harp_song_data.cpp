#include "d/snd/d_snd_harp_song_data.h"

#include "common.h"
#include "d/snd/d_snd_fi_vocal_mgr.h"
#include "d/snd/d_snd_wzsound.h"

const s32 dSndHarpSongData_c::sNumHarpSongs = LABEL_HARP_TONE_RAW_END - LABEL_HARP_TONE_RAW_START - 1;
static const f32 sUnkSongVals[] = {
    40.0f,
    40.0f,
    40.0f,
    40.0f,
    40.0f,
    40.0f,
};

dSndHarpSongEntryBase_c::dSndHarpSongEntryBase_c(s16 count, s16 stride) : mCount(count), mStride(stride) {
    mpData = new s16[count];
    reset();
}

dSndHarpSongEntryBase_c::~dSndHarpSongEntryBase_c() {
    delete[] mpData;
}

void dSndHarpSongEntryBase_c::reset() {
    for (int i = 0; i < mCount; i++) {
        mpData[i] = -1;
    }
}

void dSndHarpSongEntryBase_c::insert(u32 idx, s16 value) {
    // TODO - magic number
    if (idx > 0x120) {
        return;
    }

    u32 insert = 0;
    while (mStride <= idx) {
        insert++;
        idx -= mStride;
    }
    mpData[insert] = value;
}

s32 dSndHarpSongEntryBase_c::get(u32 idx) const {
    if (idx >= mCount) {
        return -1;
    }
    return mpData[idx];
}

dSndHarpSongEntry1_c::dSndHarpSongEntry1_c() : dSndHarpSongEntryBase_c(NUM_ENTRIES_TYPE_1, NUM_ENTRIES_TYPE_1 / 2) {
    reset();
    for (s32 i = 0; i < NUM_ENTRIES_TYPE_1; i++) {
        mStaticValues[i] = 0;
    }
}

void dSndHarpSongEntry1_c::loadStaticData(s32 i1, u32 i2, s32 i3) {
    for (s32 i = 0; i < NUM_ENTRIES_TYPE_1; i++) {
        if (get(i) > 0) {
            mStaticValues[i] = dSndFiVocalMgr_c::GetInstance()->getStaticSongValue(i1, i2, i3, i);
        }
    }
}

s16 dSndHarpSongEntry1_c::getStaticDatum(u32 idx) const {
    if (idx >= NUM_ENTRIES_TYPE_1) {
        return -1;
    }
    return mStaticValues[idx];
}

dSndHarpSongData_c::dSndHarpSongData_c() : field_0x0C(30.0f) {
    init(0);
}

void dSndHarpSongData_c::init(u32 idx) {
    if (idx >= sNumHarpSongs) {
        return;
    }
    mIndex = idx;
    mSoundId = -1;
    field_0x08 = 0;

    for (s32 i1 = 0; i1 < 4; i1++) {
        for (u32 i2 = 0; i2 < 2; i2++) {
            for (s32 i3 = 0; i3 < 2; i3++) {
                mEntries2[i1][i2][i3].reset();
            }
            field_0x90[i1][i2] = 0;
        }
    }

    field_0x0C = sUnkSongVals[mIndex];
    loadStaticValues();
}

void dSndHarpSongData_c::loadStaticValues() {
    for (s32 i1 = 0; i1 < 4; i1++) {
        for (u32 i2 = 0; i2 < 2; i2++) {
            mEntries1[i1][i2].loadStaticData(mIndex, i1, i2);
        }
    }
}

void dSndHarpSongData_c::insert2(u32 key, s16 value, s32) {
    // TODO
}

void dSndHarpSongData_c::insert1(u32 key, s16 value) {
    // TODO
}
