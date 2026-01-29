#ifndef D_LYT_METER_KEY_H
#define D_LYT_METER_KEY_H

#include "common.h"
#include "d/lyt/d2d.h"
#include "m/m_vec.h"
#include "nw4r/lyt/lyt_pane.h"
#include "s/s_State.hpp"

class dLytMeterKakeraKey_c {
public:
    dLytMeterKakeraKey_c() : mStateMgr(*this) {}
    ~dLytMeterKakeraKey_c() {}

    bool build(d2d::ResAccIf_c *);
    bool remove();
    bool execute();
    bool draw();

    void setPosition(s32 position);

    void setShouldBeVisible(bool v) {
        mShouldBeVisible = v;
    }

private:
    STATE_FUNC_DECLARE(dLytMeterKakeraKey_c, Wait);
    STATE_FUNC_DECLARE(dLytMeterKakeraKey_c, On);
    STATE_FUNC_DECLARE(dLytMeterKakeraKey_c, Increase);
    STATE_FUNC_DECLARE(dLytMeterKakeraKey_c, Active);
    STATE_FUNC_DECLARE(dLytMeterKakeraKey_c, Off);

    STATE_MGR_DEFINE_UTIL_CHANGESTATE(dLytMeterKakeraKey_c);

    /* 0x000 */ UI_STATE_MGR_DECLARE(dLytMeterKakeraKey_c);
    /* 0x03C */ d2d::dLytSub mLyt;
    /* 0x0D0 */ d2d::AnmGroup_c mAnm[8];
    /* 0x2D0 */ nw4r::lyt::Pane *mpPane;
    /* 0x2D4 */ mVec3_c mPanePositions[2];
    /* 0x2EC */ mVec3_c mPosition;
    /* 0x2F8 */ u32 mSavedKeyPieceCount;
    /* 0x2FC */ s32 mNextKeyPieceCount;
    /* 0x300 */ s32 mPosititionIndex;
    /* 0x304 */ s32 mOldPosititionIndex;
    /* 0x308 */ s32 mMovementFrame;
    /* 0x30C */ u8 mShouldBeVisible;
};

class dLytMeterBossKey_c {
public:
    dLytMeterBossKey_c() : mStateMgr(*this) {}
    ~dLytMeterBossKey_c() {}

    virtual d2d::LytBase_c *getLyt() {
        return &mLyt;
    }
    virtual const char *getName() const {
        return mLyt.getName();
    }

    bool build(d2d::ResAccIf_c *);
    bool remove();
    bool execute();
    bool draw();

    void setPosition(s32 position);

    void setShouldBeVisible(bool v) {
        mShouldBeVisible = v;
    }

private:
    STATE_FUNC_DECLARE(dLytMeterBossKey_c, Wait);
    STATE_FUNC_DECLARE(dLytMeterBossKey_c, On);
    STATE_FUNC_DECLARE(dLytMeterBossKey_c, Active);
    STATE_FUNC_DECLARE(dLytMeterBossKey_c, Off);

    STATE_MGR_DEFINE_UTIL_CHANGESTATE(dLytMeterBossKey_c);

    /* 0x004 */ UI_STATE_MGR_DECLARE(dLytMeterBossKey_c);
    /* 0x040 */ d2d::dLytSub mLyt;
    /* 0x0D4 */ d2d::AnmGroup_c mAnm[5];
    /* 0x214 */ nw4r::lyt::Pane *mpPane;
    /* 0x218 */ mVec3_c mPanePositions[4];
    /* 0x248 */ mVec3_c mPosition;
    /* 0x254 */ s32 mPosititionIndex;
    /* 0x258 */ s32 mOldPosititionIndex;
    /* 0x25C */ s32 mMovementFrame;
    /* 0x260 */ u8 mShouldBeVisible;
};

class dLytMeterSmallKey_c {
public:
    dLytMeterSmallKey_c() : mStateMgr(*this) {}
    ~dLytMeterSmallKey_c() {}

    virtual d2d::LytBase_c *getLyt() {
        return &mLyt;
    }
    virtual const char *getName() const {
        return mLyt.getName();
    }

    bool build(d2d::ResAccIf_c *);
    bool remove();
    bool execute();
    bool draw();

    void setPosition(s32 position);

    void setShouldBeVisible(bool v) {
        mShouldBeVisible = v;
    }

private:
    STATE_FUNC_DECLARE(dLytMeterSmallKey_c, Wait);
    STATE_FUNC_DECLARE(dLytMeterSmallKey_c, On);
    STATE_FUNC_DECLARE(dLytMeterSmallKey_c, Active);
    STATE_FUNC_DECLARE(dLytMeterSmallKey_c, Off);

    STATE_MGR_DEFINE_UTIL_CHANGESTATE(dLytMeterSmallKey_c);

    /* 0x004 */ UI_STATE_MGR_DECLARE(dLytMeterSmallKey_c);
    /* 0x040 */ d2d::dLytSub mLyt;
    /* 0x0D4 */ d2d::AnmGroup_c mAnm[5];
    /* 0x214 */ nw4r::lyt::Pane *mpPane;
    /* 0x218 */ mVec3_c mPanePositions[4];
    /* 0x248 */ mVec3_c mPosition;
    /* 0x254 */ s32 mSavedSmallKeyCount;
    /* 0x258 */ s32 mPosititionIndex;
    /* 0x25C */ s32 mOldPosititionIndex;
    /* 0x260 */ s32 mMovementFrame;
    /* 0x264 */ u8 mShouldBeVisible;
};

#endif
