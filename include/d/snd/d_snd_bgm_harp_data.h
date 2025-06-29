#ifndef D_SND_BGM_HARP_DATA_H
#define D_SND_BGM_HARP_DATA_H

#include "common.h"

/**
 * This file deals with the pitch of the Goddess' Harp when Link
 * is freely strumming. In this case the pitch of the strings
 * is adjusted to match the key of background music through sequence variables.
 * When Link strums the harp, 12 strings can be heard, but it's actually only four
 * notes of the same chord spanning three octaves.
 *
 * I haven't yet investigated all the data, but a typical chord Link might play
 * is Cmaj^7, consisting of C-E-G-B. Link actually plays:
 * G3-C4-E4-G4-B4-C5-E5-G5-B5-C6-E6-G6
 * B3 is missing, possibly to prevent dissonance at the ends of the scale.
 * (Sealed Grounds, before Demise fight, no actual BGM)
 */

/**
 * Contains a single Seq sound var controlling the pitch of a subset of harp strings
 * Size: 0x02
 */
struct dSndBgmDataHarpVar_c {
    dSndBgmDataHarpVar_c() : field_0x00(0), field_0x01(0) {}

    void reset() {
        field_0x00 = 0;
        field_0x01 = 0;
    }

    void onFlag() {
        field_0x00 |= 1;
    }

    /* 0x00 */ u8 field_0x00; // flags
    /* 0x01 */ s8 field_0x01; // var
};

/**
 * Contains Seq sound vars for all harp strings, represents a "key" in the bgm music
 * Size: 0x0C
 */
class dSndBgmDataHarpVarSetBase_c {
public:
    dSndBgmDataHarpVarSetBase_c(s32 count);
    ~dSndBgmDataHarpVarSetBase_c();

    void resetVars();
    void addVar(u8 value);

    s32 getPosition() const {
        return mPosition;
    }

    void setPosition(s32 position) {
        mPosition = position;
    }

    s32 getCount() const {
        return mCount;
    }

    s32 getMax() const {
        return mMax;
    }

    dSndBgmDataHarpVar_c *getVar(s32 idx) {
        if (idx >= getCount()) {
            return nullptr;
        }
        return &mpVars[idx];
    }

    dSndBgmDataHarpVar_c *getUnusedVar() {
        if (mCount >= mMax) {
            return nullptr;
        }
        return &mpVars[mCount];
    }

private:
    /* 0x00 */ dSndBgmDataHarpVar_c *mpVars;
    /* 0x04 */ s32 mPosition;
    /* 0x08 */ s16 mMax;
    /* 0x0A */ s16 mCount;
};

class dSndBgmDataHarpVarSet_c : public dSndBgmDataHarpVarSetBase_c {
public:
    dSndBgmDataHarpVarSet_c() : dSndBgmDataHarpVarSetBase_c(4) {}

private:
};

/**
 * Contains parsed seq data for harp strings for a single bgm sound
 */
class dSndBgmHarpDataBase_c {
public:
    dSndBgmHarpDataBase_c();
    void resetVars();
    s32 getIdxForPosition(s32 position);

    dSndBgmDataHarpVarSetBase_c *getUsableVarSet(s32 idx);
    void addVar(s32 position, s32 value, s32 unk);
    void setField_0x08(s32 value);

protected:
    dSndBgmDataHarpVarSetBase_c *getVarSet(s32 idx);
    /* 0x00 */ dSndBgmDataHarpVarSetBase_c *mpVarSets;
    /* 0x04 */ s16 mMax;
    /* 0x06 */ s16 mCount;
    /* 0x08 */ s32 field_0x08;
};

class dSndBgmHarpData_c : public dSndBgmHarpDataBase_c {
public:
    dSndBgmHarpData_c();
    ~dSndBgmHarpData_c();

    /**
     * Writes the Seq sound variables that control the pitch
     * of the individual harp strings.
     */
    void writeSeqVars(s32 idx);

private:
    static const u32 NUM_SETS = 300;
};

#endif
