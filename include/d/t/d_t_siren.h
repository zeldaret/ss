#ifndef D_T_SIREN_H
#define D_T_SIREN_H

#include "common.h"
#include "d/t/d_tg.h"
#include "toBeSorted/timekeeper.h"

class dTgSiren_c : public dTg_c {
public:
    dTgSiren_c() {}
    virtual ~dTgSiren_c() {}

    static bool isOutOfTime() {
        if (sInstance == nullptr) {
            return false;
        }
        return sInstance->outOfTime();
    }

    static s32 getTime() {
        if (sInstance == nullptr) {
            return -1;
        }
        return sInstance->mTimekeeper.getTime();
    }

    static bool hasCollectedAllTears() {
        if (sInstance == nullptr) {
            return false;
        }
        return sInstance->_hasCollectedAllTears();
    }

private:
    bool outOfTime() const;
    bool _hasCollectedAllTears();
    static dTgSiren_c *sInstance;

    /* 0x0FC */ u8 _0x0FC[0x2C0 - 0x0FC];
    /* 0x2C0 */ Timekeeper mTimekeeper;
};

#endif
