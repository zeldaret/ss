#ifndef D_C_BASE_H
#define D_C_BASE_H

#include "common.h"
#include "d/d_cs.h"
#include "d/d_cursor_hit_check.h"
#include "d/lyt/d2d.h"
#include "d/lyt/d_lyt_cursor_stick.h"
#include "m/m2d.h"
#include "m/m_angle.h"

class dCsBase_c : public dCs_c {
public:
    dCsBase_c();
    virtual ~dCsBase_c();
    virtual int create() override;
    virtual int doDelete() override;
    virtual int draw() override;
    virtual int execute() override;

    int update();
    bool drawDirectly();

    void setField703(bool val) {
        field_0x703 = val;
    }

    void setField704(bool val) {
        field_0x704 = val;
    }
    void setField705(bool val) {
        field_0x705 = val;
    }

    dCursorHitCheck_c *getHitCheck() {
        return mCursorIf.getHit();
    }

    static dCsBase_c *GetInstance() {
        return sInstance;
    }

private:
    static dCsBase_c *sInstance;
    void setCurrentLyt(int lyt);

    /* 0x068 */ m2d::ResAccIf_c mResAcc;
    /* 0x11C */ d2d::LytBase_c mLyts[2];
    /* 0x23C */ d2d::LytBase_c *mpCurrLyt;
    /* 0x240 */ dCursorInterface_c mCursorIf;
    /* 0x254 */ dLytCursorStick_c mCursorStick;
    /* 0x6F0 */ f32 field_0x6F0;
    /* 0x6F4 */ f32 field_0x6F4;
    /* 0x6F8 */ f32 field_0x6F8;
    /* 0x6FC */ f32 field_0x6FC;
    /* 0x700 */ mAng field_0x700;
    /* 0x702 */ u8 field_0x702;
    /* 0x703 */ u8 field_0x703;
    /* 0x704 */ u8 field_0x704;
    /* 0x705 */ u8 field_0x705;
};

#endif
