#ifndef D_T_TELOP_H
#define D_T_TELOP_H

#include "d/flag/sceneflag_manager.h"
#include "d/t/d_tg.h"

class dTgTelop_c : public dTg_c {
public:
    dTgTelop_c() {}
    virtual ~dTgTelop_c() {}

    virtual int create() override;
    virtual int doDelete() override;
    virtual int draw() override;
    virtual int actorExecute() override;

    u16 getArea() const {
        return params >> 8 & 0xFF;
    }
    u8 getSceneflag() const {
        return params >> 16 & 0xFF;
    }
    u8 getTempflag() const {
        return params >> 24 & 0xFF;
    }

private:
    /* 0xFC */ u16 mArea;
    /* 0xFD */ u8 mSceneflag;
    /* 0xFF */ u8 mTempflag;
};

#endif
