#include "d/lyt/d_lyt_drop_line.h"

#include "common.h"
#include "d/a/d_a_item.h"
#include "d/lyt/meter/d_lyt_meter.h"
#include "d/snd/d_snd_small_effect_mgr.h"
#include "d/snd/d_snd_wzsound.h"
#include "f/f_base.h"
#include "m/m_color.h"
#include "m/m_vec.h"
#include "nw4r/math/math_types.h"
#include "toBeSorted/d_d3d.h"
#include "toBeSorted/d_emitter.h"

class dLytDropLine_HIO_c {
public:
    dLytDropLine_HIO_c();
    virtual ~dLytDropLine_HIO_c();

    /* 0x04 */ f32 field_0x04;
    /* 0x08 */ s32 mMoveDuration;
    /* 0x0C */ u8 field_0x0C;
};

static dLytDropLine_HIO_c sHio;

dLytDropLine_HIO_c::dLytDropLine_HIO_c() : field_0x04(5.0f), mMoveDuration(10), field_0x0C(0) {}
dLytDropLine_HIO_c::~dLytDropLine_HIO_c() {}

STATE_DEFINE(dLytDropLineParts_c, Wait);
STATE_DEFINE(dLytDropLineParts_c, Move);

int dLytDropLineParts_c::create(dLytDropLine_c *main) {
    mEmitter.init(main);
    mpTargetPane = nullptr;
    reset();
    mStateMgr.changeState(StateID_Wait);
    return fBase_c::SUCCEEDED;
}

int dLytDropLineParts_c::doDelete() {
    // no-op
    return fBase_c::SUCCEEDED;
}

int dLytDropLineParts_c::execute() {
    mStateMgr.executeState();
    return fBase_c::SUCCEEDED;
}

int dLytDropLineParts_c::draw() {
    // no-op
    return fBase_c::SUCCEEDED;
}

void dLytDropLineParts_c::startPickup(mVec3_c tearWorldPos, s32 trial) {
    d3d::worldToScreen(mStartPos, tearWorldPos);
    mStartPos.z = 0.0f;
    mEffectPos = mStartPos;
    mEffectPos.z = 0.0f;

    mTrial = trial;
    mMoveRequest = true;
    mIsDone = false;
    mLeftRight = false;
    mIsChangeAnim = false;
}

void dLytDropLineParts_c::startChange(mVec3_c startPos, s32 trial, bool leftRight) {
    mStartPos = startPos;
    mEffectPos = mStartPos;

    mTrial = trial;
    mMoveRequest = true;
    mIsDone = false;
    mLeftRight = leftRight;
    mIsChangeAnim = true;
}

void dLytDropLineParts_c::reset() {
    mStartPos.setZero();
    mEffectPos.setZero();
    mMoveLinearCoeff.setZero();
    mMoveTimer = 0;
    mTrial = 4;
    mMoveRequest = false;
    mIsDone = false;
    mLeftRight = false;
    mIsChangeAnim = false;
}

void dLytDropLineParts_c::initializeState_Wait() {}

void dLytDropLineParts_c::executeState_Wait() {
    if (mMoveRequest) {
        mStateMgr.changeState(StateID_Move);
    }
}

void dLytDropLineParts_c::finalizeState_Wait() {}

void dLytDropLineParts_c::initializeState_Move() {
    mMoveTimer = 0;
    if (mpTargetPane != nullptr) {
        nw4r::math::MTX34 mtx = mpTargetPane->GetGlobalMtx();

        // Set up the coefficients for the quadratic parabola below
        mVec3_c v1 = mVec3_c(mtx._03, mtx._13, 0.0f);

        mMoveLinearCoeff.x =
            ((v1.x - mStartPos.x) + (sHio.field_0x04 * (f32)sHio.mMoveDuration * (f32)sHio.mMoveDuration)) /
            (f32)sHio.mMoveDuration;
        mMoveLinearCoeff.y = (v1.y - mStartPos.y) / (f32)sHio.mMoveDuration;

        if (!mIsChangeAnim) {
            dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_SIREN_SHIZUKU_GET_MOVE);
        }
    }
}

void dLytDropLineParts_c::executeState_Move() {
    bool isDone = false;
    if (mpTargetPane != nullptr) {
        nw4r::math::MTX34 mtx = mpTargetPane->GetGlobalMtx();
        mVec3_c panePos(mtx._03, mtx._13, 0.0f);

        if (!mIsChangeAnim) {
            if (dLytMeter_c::getfn_800C9FE0()) {
                mEffectPos = panePos;
                mStateMgr.changeState(StateID_Wait);
                mMoveRequest = false;
                mIsDone = true;
                return;
            }
        }

        if (mMoveTimer < sHio.mMoveDuration) {
            // Quadratic parabola
            if (mLeftRight) {
                mEffectPos.x = mStartPos.x - (mMoveLinearCoeff.x * (f32)mMoveTimer -
                                              sHio.field_0x04 * (f32)mMoveTimer * (f32)mMoveTimer);
            } else {
                mEffectPos.x = mStartPos.x + mMoveLinearCoeff.x * (f32)mMoveTimer -
                               sHio.field_0x04 * (f32)mMoveTimer * (f32)mMoveTimer;
            }

            mEffectPos.y = mStartPos.y + mMoveLinearCoeff.y * (f32)mMoveTimer;
            mEffectPos.z = 0.0f;
            mMoveTimer++;
        } else {
            isDone = true;
            mEffectPos = panePos;
        }

        static const mColor sColors1[] = {
            mColor(0xA0, 0xFF, 0xB4, 0xFF), // #A0FFB4FF
            mColor(0xFF, 0x80, 0xB4, 0xFF), // #FF80B4FF
            mColor(0xFF, 0xFF, 0x80, 0xFF), // #FFFF80FF
            mColor(0x80, 0xC8, 0xFF, 0xFF), // #80C8FFFF
        };

        static const mColor sColors2[] = {
            mColor(0x00, 0xFF, 0x30, 0xFF), // #00FF30FF
            mColor(0xFF, 0x40, 0x80, 0xFF), // #FF4080FF
            mColor(0xFF, 0xFF, 0x00, 0xFF), // #FFFF00FF
            mColor(0x60, 0x96, 0xFF, 0xFF), // #6096FFFF
        };

        mColor c1 = 0xFFFFFFFF;
        mColor c2 = 0xFFFFFFFF;
        switch (mTrial) {
            case dAcItem_c::TRIAL_SKYLOFT:
                c1 = sColors1[0];
                c2 = sColors2[0];
                break;
            case dAcItem_c::TRIAL_FARON:
                c1 = sColors1[1];
                c2 = sColors2[1];
                break;
            case dAcItem_c::TRIAL_ELDIN:
                c1 = sColors1[2];
                c2 = sColors2[2];
                break;
            case dAcItem_c::TRIAL_LANAYRU:
                c1 = sColors1[3];
                c2 = sColors2[3];
                break;
        }

        mEmitter.holdUIEffect(PARTICLE_RESOURCE_ID_MAPPING_138_, mEffectPos, nullptr, nullptr, &c1, &c2);
    } else {
        isDone = true;
    }

    if (isDone) {
        mStateMgr.changeState(StateID_Wait);
        mMoveRequest = false;
        mIsDone = true;
        mIsChangeAnim = false;
    }
}

void dLytDropLineParts_c::finalizeState_Move() {}

SPECIAL_BASE_PROFILE(LYT_DROP_LINE, dLytDropLine_c, fProfile::LYT_DROP_LINE, 0x2B0, 0x6FD);

dLytDropLine_c *dLytDropLine_c::sInstance;

int dLytDropLine_c::create() {
    sInstance->createInternal();
    return SUCCEEDED;
}

int dLytDropLine_c::doDelete() {
    sInstance->doDeleteInternal();
    return SUCCEEDED;
}

int dLytDropLine_c::execute() {
    if (sInstance != nullptr) {
        sInstance->executeInternal();
    }
    return SUCCEEDED;
}

int dLytDropLine_c::draw() {
    if (sInstance != nullptr) {
        sInstance->drawInternal();
    }
    return SUCCEEDED;
}

void dLytDropLine_c::startPickup(mVec3_c tearWorldPos, dAcItem_c::Trial_e trial) {
    if (sInstance != nullptr) {
        sInstance->startPickupInternal(tearWorldPos, trial);
    }
}

void dLytDropLine_c::startChange(
    nw4r::lyt::Pane *tearSlotPane, nw4r::lyt::Pane *flowerPane, dAcItem_c::Trial_e trial, bool leftRight
) {
    if (sInstance != nullptr) {
        sInstance->startChangeInternal(tearSlotPane, flowerPane, trial, leftRight);
    }
}

bool dLytDropLine_c::finishPart() {
    if (sInstance != nullptr) {
        return sInstance->finishPartInternal();
    }
    return false;
}

void dLytDropLine_c::setPane(nw4r::lyt::Pane *pane) {
    if (sInstance != nullptr) {
        sInstance->setPaneInternal(pane);
    }
}

nw4r::lyt::Pane *dLytDropLine_c::getActivePane() {
    if (sInstance != nullptr) {
        return sInstance->getActivePaneInternal();
    }
    return nullptr;
}

int dLytDropLine_c::createInternal() {
    for (int i = 0; i < NUM_PARTS; i++) {
        mParts[i].create(this);
    }
    return true;
}

int dLytDropLine_c::doDeleteInternal() {
    for (int i = 0; i < NUM_PARTS; i++) {
        mParts[i].doDelete();
    }
    return SUCCEEDED;
}

int dLytDropLine_c::executeInternal() {
    for (int i = 0; i < NUM_PARTS; i++) {
        mParts[i].execute();
    }
    return SUCCEEDED;
}

int dLytDropLine_c::drawInternal() {
    for (int i = 0; i < NUM_PARTS; i++) {
        mParts[i].draw();
    }
    return SUCCEEDED;
}

void dLytDropLine_c::startPickupInternal(mVec3_c tearWorldPos, dAcItem_c::Trial_e trial) {
    for (int i = 0; i < NUM_PARTS; i++) {
        if (!mParts[i].isMoveRequest()) {
            mParts[i].startPickup(tearWorldPos, trial);
            return;
        }
    }
}

void dLytDropLine_c::startChangeInternal(
    nw4r::lyt::Pane *tearSlotPane, nw4r::lyt::Pane *flowerPane, dAcItem_c::Trial_e trial, bool leftRight
) {
    for (int i = 0; i < NUM_PARTS; i++) {
        if (!mParts[i].isMoveRequest()) {
            mParts[i].setTargetPane(flowerPane);
            if (tearSlotPane != nullptr) {
                nw4r::math::MTX34 mtx = tearSlotPane->GetGlobalMtx();
                mVec3_c startPos(mtx._03, mtx._13, 0.0f);
                mParts[i].startChange(startPos, trial, leftRight);
            } else {
                mVec3_c startPos(0.0f, -200.0f, 0.0f);
                mParts[i].startChange(startPos, trial, leftRight);
            }

            return;
        }
    }
}

bool dLytDropLine_c::finishPartInternal() {
    for (int i = 0; i < NUM_PARTS; i++) {
        if (mParts[i].isDone()) {
            mParts[i].finish();
            return true;
        }
    }
    return false;
}

void dLytDropLine_c::setPaneInternal(nw4r::lyt::Pane *pane) {
    for (int i = 0; i < NUM_PARTS; i++) {
        mParts[i].setTargetPane(pane);
    }
}

nw4r::lyt::Pane *dLytDropLine_c::getActivePaneInternal() {
    for (int i = 0; i < NUM_PARTS; i++) {
        if (mParts[i].getTargetPane() != nullptr) {
            return mParts[i].getTargetPane();
        }
    }
    return nullptr;
}
