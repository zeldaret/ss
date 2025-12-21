#include "d/lyt/d_lyt_force_line.h"

#include "common.h"
#include "d/lyt/meter/d_lyt_meter.h"
#include "f/f_base.h"
#include "m/m_vec.h"
#include "nw4r/lyt/lyt_pane.h"
#include "nw4r/math/math_types.h"
#include "toBeSorted/d_d3d.h"
#include "toBeSorted/d_emitter.h"

static s32 getNumTadtonesInGroup(s32 arg) {
    static const s32 nums[] = {1, 4, 2, 8, 1, 8, 1, 4, 8, 1, 4, 8, 1, 4, 2, 4, 16};
    return nums[arg];
}

class dLytForceLine_HIO_c {
public:
    dLytForceLine_HIO_c();
    virtual ~dLytForceLine_HIO_c();

    /* 0x04 */ f32 field_0x04;
    /* 0x08 */ s32 mMoveDuration;
    /* 0x0C */ u8 field_0x0C;
};

static dLytForceLine_HIO_c sHio;

dLytForceLine_HIO_c::dLytForceLine_HIO_c() : field_0x04(5.0f), mMoveDuration(10), field_0x0C(0) {}
dLytForceLine_HIO_c::~dLytForceLine_HIO_c() {}

STATE_DEFINE(dLytForceLineParts_c, Wait);
STATE_DEFINE(dLytForceLineParts_c, Move);

int dLytForceLineParts_c::create(dLytForceLine_c *main) {
    mEmitter.init(main);
    reset();

    return fBase_c::SUCCEEDED;
}

int dLytForceLineParts_c::doDelete() {
    // no-op
    return fBase_c::SUCCEEDED;
}

int dLytForceLineParts_c::execute() {
    mStateMgr.executeState();
    return fBase_c::SUCCEEDED;
}

int dLytForceLineParts_c::draw() {
    // no-op
    return fBase_c::SUCCEEDED;
}

void dLytForceLineParts_c::start(mVec3_c tadtoneWorldPos, mVec2_c targetPos, s32 variant) {
    d3d::worldToScreen(mStartPos, tadtoneWorldPos);
    mStartPos.z = 0.0f;
    mEffectPos = mStartPos;
    mEffectPos.z = 0.0f;
    mTargetPos = targetPos;

    mVariant = variant;
    mMoveRequest = true;
    mIsDone = false;
}

void dLytForceLineParts_c::reset() {
    mStartPos.setZero();
    mEffectPos.setZero();
    mMoveLinearCoeff.setZero();
    mTargetPos.setZero();
    mMoveTimer = 0;
    mVariant = 0;
    mMoveRequest = false;
    mIsDone = false;
    mStateMgr.changeState(StateID_Wait);
}

void dLytForceLineParts_c::initializeState_Wait() {}

void dLytForceLineParts_c::executeState_Wait() {
    if (mMoveRequest) {
        mStateMgr.changeState(StateID_Move);
    }
}

void dLytForceLineParts_c::finalizeState_Wait() {}

void dLytForceLineParts_c::initializeState_Move() {
    mMoveTimer = 0;

    // Set up the coefficients for the quadratic parabola below
    mMoveLinearCoeff.x =
        ((mTargetPos.x - mStartPos.x) + (sHio.field_0x04 * (f32)sHio.mMoveDuration * (f32)sHio.mMoveDuration)) /
        (f32)sHio.mMoveDuration;
    mMoveLinearCoeff.y = (mTargetPos.y - mStartPos.y) / (f32)sHio.mMoveDuration;
}

void dLytForceLineParts_c::executeState_Move() {
    bool isDone = false;

    if (dLytMeter_c::getfn_800C9F70()) {
        mStateMgr.changeState(StateID_Wait);
        mMoveRequest = false;
        mIsDone = true;
        return;
    }

    if (mMoveTimer < sHio.mMoveDuration) {
        // Quadratic parabola
        mEffectPos.x =
            mStartPos.x + mMoveLinearCoeff.x * (f32)mMoveTimer - sHio.field_0x04 * (f32)mMoveTimer * (f32)mMoveTimer;

        mEffectPos.y = mStartPos.y + mMoveLinearCoeff.y * (f32)mMoveTimer;
        mEffectPos.z = 0.0f;
        mMoveTimer++;
    } else {
        isDone = true;
        mEffectPos.x = mTargetPos.x;
        mEffectPos.y = mTargetPos.y;
        mEffectPos.z = 0.0f;
    }

    switch (mVariant) {
        case 3:
            mEmitter.holdUIEffect(PARTICLE_RESOURCE_ID_MAPPING_870_, mEffectPos, nullptr, nullptr, nullptr, nullptr);
            break;
        case 2:
            mEmitter.holdUIEffect(PARTICLE_RESOURCE_ID_MAPPING_871_, mEffectPos, nullptr, nullptr, nullptr, nullptr);
            break;
        case 0:
            mEmitter.holdUIEffect(PARTICLE_RESOURCE_ID_MAPPING_873_, mEffectPos, nullptr, nullptr, nullptr, nullptr);
            break;
        case 1:
            mEmitter.holdUIEffect(PARTICLE_RESOURCE_ID_MAPPING_872_, mEffectPos, nullptr, nullptr, nullptr, nullptr);
            break;
        case 5:
            mEmitter.holdUIEffect(PARTICLE_RESOURCE_ID_MAPPING_875_, mEffectPos, nullptr, nullptr, nullptr, nullptr);
            break;
        case 4:
            mEmitter.holdUIEffect(PARTICLE_RESOURCE_ID_MAPPING_874_, mEffectPos, nullptr, nullptr, nullptr, nullptr);
            break;
    }

    if (isDone) {
        mStateMgr.changeState(StateID_Wait);
        mMoveRequest = false;
        mIsDone = true;
    }
}

void dLytForceLineParts_c::finalizeState_Move() {}

SPECIAL_BASE_PROFILE(LYT_FORCE_LINE, dLytForceLine_c, fProfile::LYT_FORCE_LINE, 0x2B1, 0x6F9);

dLytForceLine_c *dLytForceLine_c::sInstance;

int dLytForceLine_c::create() {
    if (sInstance != nullptr) {
        sInstance->createInternal();
    }
    return SUCCEEDED;
}

int dLytForceLine_c::doDelete() {
    if (sInstance != nullptr) {
        sInstance->doDeleteInternal();
    }
    return SUCCEEDED;
}

int dLytForceLine_c::execute() {
    if (sInstance != nullptr) {
        sInstance->executeInternal();
    }
    return SUCCEEDED;
}

int dLytForceLine_c::draw() {
    if (sInstance != nullptr) {
        sInstance->drawInternal();
    }
    return SUCCEEDED;
}

void dLytForceLine_c::start(mVec3_c tadtoneWorldPos, s32 tadtoneGroupIdx, s32 tadtoneIdx, s32 variant) {
    if (sInstance != nullptr) {
        sInstance->startInternal(tadtoneWorldPos, tadtoneGroupIdx, tadtoneIdx, variant);
    }
}

bool dLytForceLine_c::isDone(s32 idx) {
    if (sInstance != nullptr) {
        return sInstance->isDoneInternal(idx);
    }
    return false;
}

void dLytForceLine_c::initPos(d2d::LytBase_c *lyt, d2d::AnmGroup_c *anm) {
    if (sInstance != nullptr) {
        sInstance->initPosInternal(lyt, anm);
    }
}

int dLytForceLine_c::createInternal() {
    for (int i = 0; i < NUM_PARTS; i++) {
        mParts[i].create(this);
    }
    field_0xD50 = 0;
    return true;
}

int dLytForceLine_c::doDeleteInternal() {
    for (int i = 0; i < NUM_PARTS; i++) {
        mParts[i].doDelete();
    }
    return SUCCEEDED;
}

int dLytForceLine_c::executeInternal() {
    for (int i = 0; i < NUM_PARTS; i++) {
        mParts[i].execute();
    }
    return SUCCEEDED;
}

int dLytForceLine_c::drawInternal() {
    for (int i = 0; i < NUM_PARTS; i++) {
        mParts[i].draw();
    }
    return SUCCEEDED;
}

void dLytForceLine_c::startInternal(mVec3_c tadtoneWorldPos, s32 tadtoneGroupIdx, s32 tadtoneIdx, s32 variant) {
    if (field_0xD50 != tadtoneGroupIdx || tadtoneIdx == 0) {
        field_0xD50 = tadtoneGroupIdx;
        for (int i = 0; i < NUM_PARTS; i++) {
            mParts[i].reset();
        }
    }

    s32 sum = 0;
    for (int i = 0; i < tadtoneGroupIdx; i++) {
        sum += getNumTadtonesInGroup(i);
    }
    sum += tadtoneIdx;

    mParts[tadtoneIdx].start(tadtoneWorldPos, field_0xAE8[sum], variant);
}

bool dLytForceLine_c::isDoneInternal(s32 idx) {
    return mParts[idx].isDone();
}

static const char *sPaneNames[] = {
    "N_scoreA_00", "N_scoreB_00", "N_scoreB_01", "N_scoreB_02", "N_scoreB_03", "N_scoreC_00", "N_scoreC_01",
    "N_scoreD_00", "N_scoreD_01", "N_scoreD_02", "N_scoreD_03", "N_scoreD_04", "N_scoreD_05", "N_scoreD_06",
    "N_scoreD_07", "N_scoreE_00", "N_scoreF_00", "N_scoreF_01", "N_scoreF_02", "N_scoreF_03", "N_scoreF_04",
    "N_scoreF_05", "N_scoreF_06", "N_scoreF_07", "N_scoreG_00", "N_scoreH_00", "N_scoreH_01", "N_scoreH_02",
    "N_scoreH_03", "N_scoreI_00", "N_scoreI_01", "N_scoreI_02", "N_scoreI_03", "N_scoreI_04", "N_scoreI_05",
    "N_scoreI_06", "N_scoreI_07", "N_scoreJ_00", "N_scoreK_00", "N_scoreK_01", "N_scoreK_02", "N_scoreK_03",
    "N_scoreL_00", "N_scoreL_01", "N_scoreL_02", "N_scoreL_03", "N_scoreL_04", "N_scoreL_05", "N_scoreL_06",
    "N_scoreL_07", "N_scoreM_00", "N_scoreN_00", "N_scoreN_01", "N_scoreN_02", "N_scoreN_03", "N_scoreO_00",
    "N_scoreO_01", "N_scoreP_00", "N_scoreP_01", "N_scoreP_02", "N_scoreP_03", "N_scoreQ_00", "N_scoreQ_01",
    "N_scoreQ_02", "N_scoreQ_03", "N_scoreQ_04", "N_scoreQ_05", "N_scoreQ_06", "N_scoreQ_07", "N_scoreQ_08",
    "N_scoreQ_09", "N_scoreQ_10", "N_scoreQ_11", "N_scoreQ_12", "N_scoreQ_13", "N_scoreQ_14", "N_scoreQ_15",
};

void dLytForceLine_c::initPosInternal(d2d::LytBase_c *lyt, d2d::AnmGroup_c *anm) {
    nw4r::lyt::Pane *panes[NUM_TADTONES];

    for (int i = 0; i < NUM_TADTONES; i++) {
        panes[i] = lyt->findPane(sPaneNames[i]);
    }

    bool enabled = anm->isEnabled_();
    anm->setAnimEnable(true);
    anm->setFrame(0.0f);
    lyt->calc();

    for (int i = 0; i < 24; i++) {
        nw4r::math::MTX34 mtx = panes[i]->GetGlobalMtx();
        field_0xAE8[i].set(mtx._03, mtx._13);
    }

    anm->setFrame(10.0f);
    lyt->calc();

    for (int i = 24; i < 50; i++) {
        nw4r::math::MTX34 mtx = panes[i]->GetGlobalMtx();
        field_0xAE8[i].set(mtx._03, mtx._13);
    }

    anm->setFrame(20.0f);
    lyt->calc();

    for (int i = 50; i < 77; i++) {
        nw4r::math::MTX34 mtx = panes[i]->GetGlobalMtx();
        field_0xAE8[i].set(mtx._03, mtx._13);
    }

    anm->setAnimEnable(enabled);
}
