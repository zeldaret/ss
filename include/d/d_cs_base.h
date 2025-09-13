#ifndef D_C_BASE_H
#define D_C_BASE_H

#include "common.h"
#include "d/d_cs.h"
#include "d/d_cursor_hit_check.h"
#include "d/lyt/d2d.h"
#include "d/lyt/d_lyt_cursor_stick.h"
#include "m/m2d.h"
#include "m/m_angle.h"
#include "m/m_vec.h"
#include "nw4r/lyt/lyt_pane.h"

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

    void setVisible(bool val) {
        mVisible = val;
    }
    void setDrawDirectly(bool val) {
        mDrawDirectly = val;
    }
    bool getDrawDirectly() const {
        return mDrawDirectly;
    }
    void setCalibrationPointCenterEnabled(bool val) {
        mCalibrationPointCenterEnabled = val;
    }

    dCursorHitCheck_c *getHitCheck() {
        return mCursorIf.getHit();
    }

    dCursorInterface_c *getCursorIf() {
        return &mCursorIf;
    }

    static dCsBase_c *GetInstance() {
        return sInstance;
    }

    void setCursorStickVisible(bool visible) {
        mCursorStick.setShouldBeOn(visible);
    }

    void setCursorStickTargetPane(const nw4r::lyt::Pane *target) {
        mCursorStick.setTargetPane(target);
    }

private:
    static dCsBase_c *sInstance;
    void setCurrentLyt(int lyt);

    /* 0x068 */ m2d::ResAccIf_c mResAcc;
    /* 0x11C */ d2d::LytBase_c mLyts[2];
    /* 0x23C */ d2d::LytBase_c *mpCurrLyt;
    /* 0x240 */ dCursorInterface_c mCursorIf;
    /* 0x254 */ dLytCursorStick_c mCursorStick;
    /* 0x6F0 */ mVec2_c mCsPosition;
    /* 0x6F8 */ mVec2_c mCsVelocity;
    /* 0x700 */ mAng field_0x700;
    /* 0x702 */ u8 mAlpha;
    /* 0x703 */ bool mVisible;
    /* 0x704 */ bool mDrawDirectly;
    /* 0x705 */ bool mCalibrationPointCenterEnabled;
};

#endif
