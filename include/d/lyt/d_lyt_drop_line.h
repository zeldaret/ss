#ifndef D_LYT_DROP_LINE_H
#define D_LYT_DROP_LINE_H

#include "common.h"
#include "d/a/d_a_item.h"
#include "d/d_base.h"
#include "m/m_vec.h"
#include "nw4r/lyt/lyt_pane.h"
#include "s/s_State.hpp"
#include "toBeSorted/d_emitter.h"

class dLytDropLine_c;

class dLytDropLineParts_c {
public:
    dLytDropLineParts_c() : mStateMgr(*this) {}

    int create(dLytDropLine_c *main);
    int doDelete();
    int execute();
    int draw();

    /** Starts a line from the tear's world position to the tear slot, when picking up a tear. */
    void startPickup(mVec3_c tearWorldPos, s32 trial);

    /** Starts a line from the tear slot to the flower. */
    void startChange(mVec3_c startPos, s32 trial, bool leftRight);

    bool isMoveRequest() const {
        return mMoveRequest;
    }

    bool isDone() const {
        return mIsDone;
    }

    void setTargetPane(nw4r::lyt::Pane *pane) {
        mIsDone = false;
        mIsChangeAnim = false;
        mpTargetPane = pane;
    }

    void finish() {
        mIsDone = false;
        mIsChangeAnim = false;
    }

    nw4r::lyt::Pane *getTargetPane() const {
        return mpTargetPane;
    }

private:
    STATE_FUNC_DECLARE(dLytDropLineParts_c, Wait);
    STATE_FUNC_DECLARE(dLytDropLineParts_c, Move);

    void reset();

    /* 0x00 */ UI_STATE_MGR_DECLARE(dLytDropLineParts_c);
    /* 0x3C */ nw4r::lyt::Pane *mpTargetPane;
    /* 0x40 */ dEmitter_c mEmitter;
    /* 0x74 */ mVec3_c mStartPos;
    /* 0x80 */ mVec3_c mEffectPos;
    /* 0x8C */ mVec3_c mMoveLinearCoeff;
    /* 0x98 */ s32 mTrial;
    /* 0x9C */ s32 mMoveTimer;
    /* 0xA0 */ bool mMoveRequest;
    /* 0xA1 */ bool mIsDone;
    /* 0xA2 */ bool mLeftRight;
    /* 0xA3 */ bool mIsChangeAnim;
};

/** 2D UI - Silent realm tear trail. */
class dLytDropLine_c : public dBase_c {
public:
    dLytDropLine_c() {
        sInstance = this;
    }
    virtual ~dLytDropLine_c() {
        sInstance = nullptr;
    }

    static dLytDropLine_c *GetInstance() {
        return sInstance;
    }

    virtual int create() override;
    virtual int doDelete() override;
    virtual int execute() override;
    virtual int draw() override;

    static bool finishPart();
    static void setPane(nw4r::lyt::Pane *pane);
    static nw4r::lyt::Pane *getActivePane();
    static void startPickup(mVec3_c tearWorldPos, dAcItem_c::Trial_e trial);
    static void startChange(nw4r::lyt::Pane *tearSlotPane, nw4r::lyt::Pane *flowerPane, dAcItem_c::Trial_e trial, bool leftRight);

private:
    static dLytDropLine_c *sInstance;

    static const s32 NUM_PARTS = 15;

    int createInternal();
    int doDeleteInternal();
    int executeInternal();
    int drawInternal();

    bool finishPartInternal();
    void setPaneInternal(nw4r::lyt::Pane *pane);
    nw4r::lyt::Pane *getActivePaneInternal();

    void startPickupInternal(mVec3_c tearWorldPos, dAcItem_c::Trial_e trial);
    void startChangeInternal(nw4r::lyt::Pane *tearSlotPane, nw4r::lyt::Pane *flowerPane, dAcItem_c::Trial_e trial, bool leftRight);

    /* 0x68 */ dLytDropLineParts_c mParts[NUM_PARTS];
};

#endif
