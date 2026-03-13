#ifndef D_LYT_NOTE_H
#define D_LYT_NOTE_H

#include "d/lyt/d2d.h"
#include "s/s_State.hpp"

// size 0x27C
class dLytNote_c {
public:
    dLytNote_c() : mStateMgr(*this) {}
    virtual ~dLytNote_c() {}

    bool build(d2d::ResAccIf_c *);
    bool remove();
    void execute(u8 alpha);
    void draw();

    void setTransform(nw4r::lyt::Pane &p);

    f32 getField0x260() const;
    void setFrame(f32);

    bool isStateWait() const {
        return mStateMgr.isState(StateID_Wait);
    }

    void forceOut() {
        mShouldMove = false;
        mForceOut = true;
    }

    void lose() {
        mShouldMove = false;
        mIsCollected = false;
    }

    void collect() {
        mShouldMove = false;
        mIsCollected = true;
    }

    void startLose() {
        mIsAboutToLose = true;
    }

    void stopLose() {
        mIsAboutToLose = false;
    }

    void setColor(s32 color) {
        mColor = color;
    }

    void setMove() {
        mIsAboutToLose = false;
        mShouldMove = true;
    }

    bool isMove() const {
        return mIsMove;
    }

private:
    STATE_FUNC_DECLARE(dLytNote_c, Wait);
    STATE_FUNC_DECLARE(dLytNote_c, In);
    STATE_FUNC_DECLARE(dLytNote_c, Move);
    STATE_FUNC_DECLARE(dLytNote_c, Out);

    /* 0x004 */ STATE_MGR_DECLARE(dLytNote_c);
    /* 0x040 */ d2d::LytBase_c mLyt;
    /* 0x0D0 */ d2d::AnmGroup_c mAnmGroups[6];
    /* 0x250 */ nw4r::lyt::Pane *mpPane;
    /* 0x254 */ mVec3_c mTranslate;
    /* 0x260 */ f32 field_0x260;
    /* 0x264 */ s32 mOutAnim;
    /* 0x268 */ s32 mInAnim;
    /* 0x26C */ s32 mColor;
    /* 0x270 */ bool mShouldMove;
    /* 0x271 */ bool mIsCollected;
    /* 0x272 */ bool mIsAboutToLose;
    /* 0x273 */ bool mIsMove;
    /* 0x274 */ bool mForceOut;
};

#endif
