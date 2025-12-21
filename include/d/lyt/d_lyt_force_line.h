#ifndef D_LYT_FORCE_LINE_H
#define D_LYT_FORCE_LINE_H

#include "common.h"
#include "d/d_base.h"
#include "d/lyt/d2d.h"
#include "m/m_vec.h"
#include "s/s_State.hpp"
#include "toBeSorted/d_emitter.h"

class dLytForceLine_c;

class dLytForceLineParts_c {
public:
    dLytForceLineParts_c() : mStateMgr(*this) {}

    int create(dLytForceLine_c *main);
    int doDelete();
    int execute();
    int draw();

    void start(mVec3_c tadtoneWorldPos, mVec2_c targetPos, s32 variant);
    void reset();

    bool isDone() const {
        return mIsDone;
    }

private:
    STATE_FUNC_DECLARE(dLytForceLineParts_c, Wait);
    STATE_FUNC_DECLARE(dLytForceLineParts_c, Move);

    /* 0x00 */ UI_STATE_MGR_DECLARE(dLytForceLineParts_c);
    /* 0x3C */ dEmitter_c mEmitter;
    /* 0x70 */ mVec3_c mStartPos;
    /* 0x7C */ mVec3_c mEffectPos;
    /* 0x88 */ mVec3_c mMoveLinearCoeff;
    /* 0x94 */ mVec2_c mTargetPos;
    /* 0x9C */ s32 mVariant;
    /* 0xA0 */ s32 mMoveTimer;
    /* 0xA4 */ bool mMoveRequest;
    /* 0xA5 */ bool mIsDone;
};

/** 2D UI - Tadtone pickup trail */
class dLytForceLine_c : public dBase_c {
public:
    dLytForceLine_c() {
        sInstance = this;
    }
    virtual ~dLytForceLine_c() {
        sInstance = nullptr;
    }

    static dLytForceLine_c *GetInstance() {
        return sInstance;
    }

    virtual int create() override;
    virtual int doDelete() override;
    virtual int execute() override;
    virtual int draw() override;

    static void start(mVec3_c tadtoneWorldPos, s32 tadtoneGroupIdx, s32 tadtoneIdx, s32 variant);
    static bool isDone(s32 idx);
    static void initPos(d2d::LytBase_c *lyt, d2d::AnmGroup_c *anm);

private:
    static dLytForceLine_c *sInstance;

    static const s32 NUM_PARTS = 16;
    static const s32 NUM_TADTONES = 77;

    int createInternal();
    int doDeleteInternal();
    int executeInternal();
    int drawInternal();

    void startInternal(mVec3_c tadtoneWorldPos, s32 tadtoneGroupIdx, s32 tadtoneIdx, s32 variant);
    bool isDoneInternal(s32 idx);
    void initPosInternal(d2d::LytBase_c *lyt, d2d::AnmGroup_c *anm);

    /* 0x068 */ dLytForceLineParts_c mParts[NUM_PARTS];
    /* 0xAE8 */ mVec2_c field_0xAE8[NUM_TADTONES];
    /* 0xD50 */ UNKWORD field_0xD50;
};

#endif
