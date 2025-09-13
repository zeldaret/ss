#ifndef D_SND_HARP_SONG_DATA_H
#define D_SND_HARP_SONG_DATA_H

#include "common.h"

class dSndHarpSongEntryBase_c {
public:
    dSndHarpSongEntryBase_c(s16 count, s16 stride);
    ~dSndHarpSongEntryBase_c();

    void insert(u32 idx, s16 value);
    s32 get(u32 idx) const;

    void reset();

private:
    /* 0x00 */ s16 *mpData;
    /* 0x04 */ u16 mCount;
    /* 0x06 */ u16 mStride;
};

class dSndHarpSongEntry1_c : public dSndHarpSongEntryBase_c {
    static const s32 NUM_ENTRIES_TYPE_1 = 24;

public:
    dSndHarpSongEntry1_c();
    void loadStaticData(s32 i1, u32 i2, s32 i3);
    s16 getStaticDatum(u32 idx) const;

private:
    /* 0x08 */ s16 mStaticValues[24];
};

class dSndHarpSongEntry2_c : public dSndHarpSongEntryBase_c {
    static const s32 NUM_ENTRIES_TYPE_2 = 12;

public:
    dSndHarpSongEntry2_c() : dSndHarpSongEntryBase_c(NUM_ENTRIES_TYPE_2, NUM_ENTRIES_TYPE_2 * 2) {}

private:
};

class dSndHarpSongData_c {
public:
    dSndHarpSongData_c();

    void init(u32 idx);
    void loadStaticValues();

    void insert2(u32 key, s16 value, s32);
    void insert1(u32 key, s16 value);

    static const s32 sNumHarpSongs;

    void setSoundId(u32 id) {
        mSoundId = id;
    }

private:
    /* 0x00 */ u32 mSoundId;
    /* 0x04 */ u32 mIndex;
    /* 0x08 */ UNKWORD field_0x08;
    /* 0x0C */ f32 field_0x0C;
    /* 0x10 */ dSndHarpSongEntry2_c mEntries2[4][2][2];
    /* 0x90 */ UNKWORD field_0x90[4][2];
    /* 0xB0 */ dSndHarpSongEntry1_c mEntries1[4][2];
};

#endif
