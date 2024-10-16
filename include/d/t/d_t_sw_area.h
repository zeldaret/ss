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
        return params;
    }

    u8 getUnsetSceneflag() {
        return params >> 0x8;
    }

    u8 getSetOnLeave() {
        return params >> 0x10 & 1;
    }

    mAng getSetStoryflag() {
        return rotation.x & 0x7FF;
    }

    mAng getUnsetStoryflag() {
        return rotation.z & 0x7FF;
    }

    mMtx_c area;
    u8 setSceneflag;
    u8 unsetSceneflag;
    bool unsetOnLeave;
    f32 scale;
    u16 setStoryflag;
    u16 unsetStoryflag;

    static u32 sDefaultRotX;
    static u32 sDefaultRotZ;
};

#endif
