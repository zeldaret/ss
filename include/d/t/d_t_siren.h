#ifndef D_T_SIREN_H
#define D_T_SIREN_H

#include "common.h"
#include "d/t/d_tg.h"

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

private:
    bool outOfTime() const;

    static dTgSiren_c *sInstance;
};

#endif
