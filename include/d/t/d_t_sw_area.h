#ifndef D_T_SW_AREA_H
#define D_T_SW_AREA_H

#include "d/t/d_tg.h"
#include "m/m_mtx.h"

class dTgSwArea_c : public dTg_c {
public:
    dTgSwArea_c() {}
    virtual ~dTgSwArea_c() {}

    virtual int create() override;
    virtual int actorExecute() override;

private:
    u8 getSetSceneflag() {
        return mParams;
    }

    u8 getUnsetSceneflag() {
        return mParams >> 0x8;
    }

    u8 getIsPersistent() {
        return mParams >> 0x10 & 1;
    }

    mAng getSetStoryflag() {
        return mRotation.x & 0x7FF;
    }

    mAng getUnsetStoryflag() {
        return mRotation.z & 0x7FF;
    }

    mMtx_c area;
    u8 setSceneflag;
    u8 unsetSceneflag;
    bool isTemporary;
    f32 scale;
    u16 setStoryflag;
    u16 unsetStoryflag;
};

#endif
