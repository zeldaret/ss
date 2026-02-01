#ifndef D_LYT_DEMO_DOWSING_H
#define D_LYT_DEMO_DOWSING_H

#include "common.h"
#include "d/d_base.h"
#include "d/lyt/d2d.h"
#include "s/s_State.hpp"

class dLytDemoDowsing_c : public dBase_c {
public:
    dLytDemoDowsing_c() : mStateMgr(*this) {
        sInstance = this;
    }
    virtual ~dLytDemoDowsing_c() {
        sInstance = nullptr;
    }

    static dLytDemoDowsing_c *GetInstance() {
        return sInstance;
    }

    void start() {
        mShouldStart = true;
    }

private:
    STATE_FUNC_DECLARE(dLytDemoDowsing_c, Wait);
    STATE_FUNC_DECLARE(dLytDemoDowsing_c, Start);

    static dLytDemoDowsing_c *sInstance;

    /* 0x068 */ UI_STATE_MGR_DECLARE(dLytDemoDowsing_c);
    /* 0x0A4 */ d2d::ResAccIf_c mResAcc;
    /* 0x414 */ d2d::LytBase_c mLyt;
    /* 0x4A4 */ d2d::AnmGroup_c mAnm[2];
    /* 0x524 */ bool mShouldStart;
};

#endif
