#include "d/a/obj/d_a_obj_tower_hand_D101.h"

#include "common.h"
#include "d/a/d_a_item.h"
#include "d/a/d_a_player.h"
#include "d/col/bg/d_bg_s.h"
#include "d/col/bg/d_bg_w.h"
#include "d/flag/sceneflag_manager.h"
#include "f/f_base.h"
#include "m/m3d/m_fanm.h"
#include "m/m_angle.h"
#include "m/m_mtx.h"
#include "m/m_vec.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "rvl/MTX/mtx.h"
#include "rvl/MTX/vec.h"
#include "s/s_Math.h"
#include "toBeSorted/actor_event.h"
#include "toBeSorted/event_manager.h"
#include "toBeSorted/room_manager.h"

SPECIAL_ACTOR_PROFILE(OBJ_TOWER_HAND_D101, dAcOTowerHandD101_c, fProfile::OBJ_TOWER_HAND_D101, 0x180, 0, 6);

const mVec3_c sScales[2] = {mVec3_c(1.0f, 1.0f, 1.0f), mVec3_c(-1.0f, 1.0f, 1.0f)};
const mVec3_c sBoundingLower = mVec3_c(-1000.0f, 0.0f, -600.0f);
const mVec3_c sBoundingUpper = mVec3_c(1000.0f, 1000.0f, 1000.0f);

const mVec3_c v1 = mVec3_c::Ex * 50.0f;
const mVec3_c v2 = mVec3_c::Ey * 50.0f;
const mVec3_c v3 = mVec3_c::Ez * 50.0f;

STATE_DEFINE(dAcOTowerHandD101_c, RemainOpen);
STATE_DEFINE(dAcOTowerHandD101_c, Close);
STATE_DEFINE(dAcOTowerHandD101_c, RemainClosed);
STATE_DEFINE(dAcOTowerHandD101_c, Open);
STATE_DEFINE(dAcOTowerHandD101_c, Hold);

static const char *const sOarcName = "TowerHandD101";
static const char *const sMdlName = "TowerHandD101";
static const char *const sAnmName = "TowerHandD101";

static const char *const sCollisionParts[2][3] = {
    {  "TowerHandD101A",   "TowerHandD101B",   "TowerHandD101C"},
    {"TowerHandD101A_R", "TowerHandD101B_R", "TowerHandD101C_R"},
};

static const char *const sLocator1 = "locator1";
static const char *const sLocator2 = "locator2";

// TODO there are tons of unused floats in the rodata section

bool dAcOTowerHandD101_c::getScale(int direction, mVec3_c &outScale) {
    if (!isValidDirectionParam(direction)) {
        return false;
    }

    outScale.x = sScales[direction].x;
    outScale.y = sScales[direction].y;
    outScale.z = sScales[direction].z;
    return true;
}

const mVec3_c &dAcOTowerHandD101_c::getBoundingLower() {
    return sBoundingLower;
}

const mVec3_c &dAcOTowerHandD101_c::getBoundingUpper() {
    return sBoundingUpper;
}

bool dAcOTowerHandD101_c::isValidDirectionParam(int param) {
    return 0 <= param && param <= 1;
}

void dAcOTowerHandD101_c::transformMtx(
    const mVec3_c &localTranslation, const mAng3_c &rotation, const mVec3_c &offset, mVec3_c &result
) {
    mMtx_c tmp1, tmp2;
    tmp1.transS(localTranslation);
    tmp1.ZXYrotM(rotation.x, rotation.y, rotation.z);
    tmp2.transS(offset);
    tmp1 += tmp2;
    tmp1.getTranslation(result);
}

f32 dAcOTowerHandD101_c::f32Sq(f32 value) {
    return value * value;
}

bool dAcOTowerHandD101_c::isValidBgWIndex(int index) {
    return 0 <= index && index <= 2;
}

bool dAcOTowerHandD101_c::createHeap() {
    const char *oarcName = sOarcName;
    void *resP = getOarcResFile(oarcName);
    if (resP == nullptr) {
        return false;
    }
    if (!mMdl.create(resP, sMdlName, sAnmName, &heap_allocator, 0x130)) {
        return false;
    }
    int direction = getDirection();
    if (!isValidDirectionParam(direction)) {
        return false;
    }
    nw4r::g3d::ResFile res(resP);
    if (!res.IsValid()) {
        return false;
    }
    RoomManager::bindStageResToFile(&res);
    if (direction == 1) {
        mMdl.getModel().setCullModeAll(GX_CULL_FRONT, false);
    }
    updateMatrix();
    mMdl.getModel().setLocalMtx(mWorldMtx);
    mVec3_c newScale;
    if (!getScale(direction, newScale)) {
        return false;
    }
    mMdl.getModel().setScale(newScale);
    mMdl.getModel().calc(false);
    mMtx_c loc1Mtx;
    getLoc1Mtx(loc1Mtx);
    mLoc1Mtx.copyFrom(loc1Mtx);
    mMtx_c loc2Mtx;
    getLoc2Mtx(loc2Mtx);
    mLoc2Mtx.copyFrom(loc2Mtx);

    const char *const *parts = sCollisionParts[direction];
    for (int i = 0; i <= 2; i++) {
        void *dzb = getOarcDZB(oarcName, parts[i]);
        if (dzb == nullptr) {
            return false;
        }
        void *plc = getOarcPLC(oarcName, parts[i]);
        if (plc == nullptr) {
            return false;
        }
        dBgW *bgW = nullptr;
        getBgW(i, bgW);
        mMtx_c *bgWMtx;
        getBgWMtx(i, bgWMtx);
        bool ok = !(BOOL)bgW->Set((cBgD_t *)dzb, (PLC *)plc, cBgW::MOVE_BG_e, bgWMtx, &mScale);
        if (!ok) {
            return false;
        }
    }

    return true;
}

int dAcOTowerHandD101_c::actorCreate() {
    CREATE_ALLOCATOR(dAcOTowerHandD101_c);

    mMdl.getModel().setLocalMtx(mWorldMtx);
    mMdl.setAnm(sAnmName, m3d::PLAY_MODE_4, 0.0f);
    mMdl.getModel().setPriorityDraw(0x1C, 0);
    for (int i = 0; i <= 2; i++) {
        dBgW *bgW = nullptr;
        getBgW(i, bgW);
        bgW->SetCrrFunc(dBgS_MoveBGProc_Typical);
        dBgS::GetInstance()->Regist(bgW, this);
    }

    boundingBox.Set(getBoundingLower(), getBoundingUpper());

    return SUCCEEDED;
}

int dAcOTowerHandD101_c::actorPostCreate() {
    mVec3_c pos;
    getItemPos(pos);
    bool handClosed = true;
    dAcObjBase_c *ac = dAcObjBase_c::getNextObject(&dAcItem_c::sItemList, nullptr);
    f32 distLimit = 90000.0f;
    while (handClosed && ac != nullptr) {
        if (PSVECSquareDistance(pos, ac->position) < distLimit) {
            handClosed = false;
            mHeldItem.link(static_cast<dAcItem_c *>(ac));
        } else {
            ac = dAcObjBase_c::getNextObject(&dAcItem_c::sItemList, ac);
        }
    }

    if (handClosed) {
        mFlags |= 1;
    }

    if (handClosed) {
        mStateMgr.changeState(StateID_RemainClosed);
    } else {
        mStateMgr.changeState(StateID_RemainOpen);
    }

    return SUCCEEDED;
}

inline bool dAcOTowerHandD101_c::getItem(dAcItem_c *&outItem) {
    outItem = !mHeldItem.isLinked() ? nullptr : mHeldItem.get();
    bool ret = outItem == nullptr;
    return ret;
}

int dAcOTowerHandD101_c::actorExecute() {
    dAcPy_c *link;
    dAcItem_c *item;
    link = dAcPy_c::LINK;
    UNKWORD w = link->IfCurrentActionToActor(this, 0x3D);
    bool b = getItem(item);
    if (!b && item->isStateWait()) {
        f32 dist = PSVECSquareDistance(item->position, link->position);
        if (w == 0 && dist < 15625.0f) {
            item->getItemFromBWheelItem();
        } else {
            item->setItemPosition(item->position);
        }
    }

    bool doSomething;
    if (b) {
        doSomething = true;
    } else {
        doSomething = !item->isStateWait();
    }
    if (b || doSomething) {
        link->vt_0x084(this, 0x3D);
    }
    mStateMgr.executeState();

    mVec3_c newScale;
    int direction = getDirection();
    getScale(direction, newScale);
    mMdl.getModel().calc(false);

    mMtx_c tmpMtx2;
    mMtx_c tmpMtx3;
    mMtx_c tmpMtx1;
    mMtx_c tmpMtx4;

    getLoc1Mtx(tmpMtx2);
    PSMTXScale(tmpMtx1, newScale.x, newScale.y, newScale.z);
    PSMTXConcat(tmpMtx2, tmpMtx1, tmpMtx2);
    mLoc1Mtx.copyFrom(tmpMtx2);

    getLoc2Mtx(tmpMtx3);
    PSMTXScale(tmpMtx4, newScale.x, newScale.y, newScale.z);
    PSMTXConcat(tmpMtx3, tmpMtx4, tmpMtx3);
    mLoc2Mtx.copyFrom(tmpMtx3);

    if (mEffects.checkField0x00()) {
        if (direction == 1) {
            tmpMtx3.YrotM(-0x8000);
        }
        mEffects.setMtx(tmpMtx3);
    }
    for (int i = 0; i <= 2; i++) {
        dBgW *bgW = nullptr;
        getBgW(i, bgW);
        bgW->Move();
    }

    return SUCCEEDED;
}

int dAcOTowerHandD101_c::actorExecuteInEvent() {
    u32 cmd = getEventStuff().getCurrentEventCommand();
    switch (cmd) {
        case 'wait': getEventStuff().advanceNext(); break;
        case 'clos': {
            if (getEventStuff().isAdvance()) {
                if (mStateMgr.isState(StateID_Close) || mStateMgr.isState(StateID_RemainClosed)) {
                    mStateMgr.changeState(StateID_Open);
                }
            }

            mStateMgr.executeState();

            if (mStateMgr.isState(StateID_RemainClosed)) {
                getEventStuff().advanceNext();
            }

            mVec3_c newScale;
            int direction = getDirection();
            getScale(direction, newScale);
            mMdl.getModel().calc(false);

            mMtx_c tmpMtx2;
            mMtx_c tmpMtx3;
            mMtx_c tmpMtx1;
            mMtx_c tmpMtx4;

            getLoc1Mtx(tmpMtx2);
            PSMTXScale(tmpMtx1, newScale.x, newScale.y, newScale.z);
            PSMTXConcat(tmpMtx2, tmpMtx1, tmpMtx2);
            mLoc1Mtx.copyFrom(tmpMtx2);

            getLoc2Mtx(tmpMtx3);
            PSMTXScale(tmpMtx4, newScale.x, newScale.y, newScale.z);
            PSMTXConcat(tmpMtx3, tmpMtx4, tmpMtx3);
            mLoc2Mtx.copyFrom(tmpMtx3);

            if (mEffects.checkField0x00()) {
                if (direction == 1) {
                    tmpMtx3.YrotM(-0x8000);
                }
                mEffects.setMtx(tmpMtx3);
            }
            for (int i = 0; i <= 2; i++) {
                dBgW *bgW = nullptr;
                getBgW(i, bgW);
                bgW->Move();
            }
        } break;

        case '????': actorExecute(); break;
        default:
            actorExecute();
            // Cool and here we're NOT using getEventStuff. Why even bother?
            mEvent.advanceNext();
    }
    return SUCCEEDED;
}

int dAcOTowerHandD101_c::draw() {
    drawModelType1(&mMdl.getModel());
    return SUCCEEDED;
}

bool dAcOTowerHandD101_c::getBgW(int index, dBgW *&outBgW) {
    if (isValidBgWIndex(index)) {
        outBgW = &mBgWs[index];
        return true;
    }
    return false;
}

ActorEventRelated &dAcOTowerHandD101_c::getEventStuff() {
    return mEvent;
}

f32 dAcOTowerHandD101_c::getItemOffsetX() {
    return 0.0f;
}

f32 dAcOTowerHandD101_c::getItemOffsetY() {
    return 100.0f;
}

f32 dAcOTowerHandD101_c::getItemOffsetZ() {
    return 0.0f;
}

f32 dAcOTowerHandD101_c::getDist1() {
    return 300.0f;
}

f32 dAcOTowerHandD101_c::getDist2() {
    return 200.0f;
}

f32 dAcOTowerHandD101_c::getDist3() {
    return 300.0f;
}

f32 dAcOTowerHandD101_c::getLinkOffsetX() {
    return 0.0f;
}

f32 dAcOTowerHandD101_c::getLinkOffsetY() {
    return 180.0f;
}

f32 dAcOTowerHandD101_c::getLinkOffsetZ() {
    return -180.0f;
}

f32 dAcOTowerHandD101_c::getFrame1() {
    return 15.0f;
}

f32 dAcOTowerHandD101_c::getFrame2() {
    return 30.0f;
}

f32 dAcOTowerHandD101_c::getCloseRate() {
    return 3.65f;
}

f32 dAcOTowerHandD101_c::getOpenRate() {
    return 1.0f;
}

void dAcOTowerHandD101_c::doSomethingHold(f32 arg) {
    field_0xAF4 = arg;
}

int dAcOTowerHandD101_c::getDirection() const {
    return params & 0xFF;
}

int dAcOTowerHandD101_c::getHoldFlag() const {
    return (params >> 8) & 0xFF;
}

int dAcOTowerHandD101_c::getSceneFlag() const {
    return (params >> 16) & 0xFF;
}

u32 dAcOTowerHandD101_c::getEventId() const {
    return (params >> 24) & 0xFF;
}

bool dAcOTowerHandD101_c::getBgWMtx(int index, mMtx_c *&outMtx) {
    switch (index) {
        case 0:  outMtx = &mWorldMtx; return true;
        case 1:  outMtx = &mLoc1Mtx; return true;
        case 2:  outMtx = &mLoc2Mtx; return true;
        default: return false;
    }
}

bool dAcOTowerHandD101_c::getNodeMtx(const char *nodeName, mMtx_c &outMtx) const {
    int nodeId = mMdl.getModel().getNodeID(nodeName);
    if (nodeId == -1) {
        return false;
    }
    mMtx_c mtx;
    if (mMdl.getModel().getNodeWorldMtx(nodeId, mtx)) {
        outMtx.copyFrom(mtx);
        return true;
    }
    return false;
}

bool dAcOTowerHandD101_c::getLoc1Mtx(mMtx_c &outMtx) const {
    return getNodeMtx(sLocator1, outMtx);
}

bool dAcOTowerHandD101_c::getLoc2Mtx(mMtx_c &outMtx) const {
    return getNodeMtx(sLocator2, outMtx);
}

mVec3_c dAcOTowerHandD101_c::getItemOffset() {
    return mVec3_c(getItemOffsetX(), getItemOffsetY(), getItemOffsetZ());
}

void dAcOTowerHandD101_c::getItemPos(mVec3_c &outVec) const {
    calcItemPosition(getItemOffset(), outVec);
}

f32 dAcOTowerHandD101_c::getDist1Sq() {
    return f32Sq(getDist1());
}

f32 dAcOTowerHandD101_c::getDist1PlusDist2() {
    return getDist1() + getDist2();
}

f32 dAcOTowerHandD101_c::getDist1PlusDist2Sq() {
    return f32Sq(getDist1PlusDist2());
}

f32 dAcOTowerHandD101_c::getDist3Sq() {
    return f32Sq(getDist3());
}

void dAcOTowerHandD101_c::getLinkOffsetNormal(mVec3_c &out) {
    out.set(getLinkOffsetX(), getLinkOffsetY(), getLinkOffsetZ());
}

void dAcOTowerHandD101_c::getLinkOffsetMirror(mVec3_c &out) {
    out.set(-getLinkOffsetX(), getLinkOffsetY(), getLinkOffsetZ());
}

void dAcOTowerHandD101_c::getLinkOffset(mVec3_c &out) const {
    switch (getDirection()) {
        case 0: getLinkOffsetNormal(out); break;
        case 1: getLinkOffsetMirror(out); break;
    }
}

void dAcOTowerHandD101_c::setSceneFlag(int flag) const {
    u16 theFlag = flag;
    if (theFlag >= 255) {
        return;
    }
    SceneflagManager::sInstance->setFlag(roomid, theFlag);
}

void dAcOTowerHandD101_c::unsetSceneFlag(int flag) const {
    u16 theFlag = flag;
    if (theFlag >= 255) {
        return;
    }
    SceneflagManager::sInstance->unsetFlag(roomid, theFlag);
}

bool dAcOTowerHandD101_c::checkSceneFlag1(int flag, bool &result) const {
    u16 theFlag = flag;
    // I guess 255 is not a valid scene flag
    bool isValidSceneFlag = theFlag < 255;
    if (isValidSceneFlag) {
        result = SceneflagManager::sInstance->checkBoolFlag(roomid, theFlag);
    }
    return isValidSceneFlag;
}

void dAcOTowerHandD101_c::setHoldFlag() {
    setSceneFlag(getHoldFlag());
}

void dAcOTowerHandD101_c::unsetHoldFlag() {
    unsetSceneFlag(getHoldFlag());
}

bool dAcOTowerHandD101_c::checkFlag(bool &result) const {
    return checkSceneFlag1(getSceneFlag(), result);
}

// ???
inline u32 getFlags() {
    static volatile u32 FLAGS_1 = 0x00000001;
    static u32 FLAGS_2 = 0x00100001;
    // The remarkable bit here is that this does not use an `andc` instruction,
    // but rather `nor`s FLAGS_1, contrary to the other usages so far
    u32 f1 = ~FLAGS_1;
    u32 f2 = FLAGS_2;
    return f2 & f1;
}

void dAcOTowerHandD101_c::doEvent() {
    Event e = Event(getEventId(), roomid, getFlags(), (void *)&eventCallback, nullptr);
    getEventStuff().scheduleEvent(e, 0);
}

void dAcOTowerHandD101_c::eventCallback(void *arg) {
    dAcOTowerHandD101_c *self = static_cast<dAcOTowerHandD101_c *>(arg);
    self->mFlags |= 1;
    dAcPy_c *link = dAcPy_c::LINK;
    if (link->IfCurrentActionToActor(self, 0x3D)) {
        link->vt_0x084(self, 0x3D);
    }
}

void dAcOTowerHandD101_c::calcItemPosition(const mVec3_c &offset, mVec3_c &outPosition) const {
    transformMtx(position, rotation, offset, outPosition);
}

void dAcOTowerHandD101_c::initializeState_RemainOpen() {
    mMdl.setFrame(mMdl.getAnm().getStartFrame());
}

inline bool dAcOTowerHandD101_c::getItem(dAcItem_c *&outItem, bool &outIsNotWait) {
    dAcItem_c *item = mHeldItem.get();
    if (item == nullptr) {
        outItem = nullptr;
    } else {
        outItem = item;
    }
    bool ret = item == nullptr;
    if (ret) {
        outIsNotWait = true;
    } else {
        outIsNotWait = !outItem->isStateWait();
    }
    return ret;
}

void dAcOTowerHandD101_c::executeState_RemainOpen() {
    dAcPy_c *link = dAcPy_c::LINK;
    mVec3_c pos;
    getItemPos(pos);
    f32 linkDistToItem = PSVECSquareDistance(pos, link->position);
    if (EventManager::isInEvent() && getEventStuff().getCurrentEventCommand() == 'wait') {
        return;
    }
    if (EventManager::isInEvent() && getEventStuff().getCurrentEventCommand() == 'clos') {
        mStateMgr.changeState(StateID_Close);
        return;
    }
    bool isNotWait;
    dAcItem_c *item;
    if (getItem(item, isNotWait)) {
        if (160000.0f < linkDistToItem) {
            doEvent();
        }
    } else if (!isNotWait) {
        if (linkDistToItem < getDist1Sq()) {
            mStateMgr.changeState(StateID_Close);
        }
    }
}
void dAcOTowerHandD101_c::finalizeState_RemainOpen() {}

extern "C" u16 PARTICLE_RESOURCE_ID_MAPPING_573;
void dAcOTowerHandD101_c::initializeState_Close() {
    mMdl.getAnm().setPlayState(m3d::PLAY_MODE_1);
    mMdl.setRate(getCloseRate());
    mEffects.fn_80029920(PARTICLE_RESOURCE_ID_MAPPING_573, &position, nullptr, nullptr, nullptr, nullptr);
    playSound(0xC0C);
}
void dAcOTowerHandD101_c::executeState_Close() {
    dAcPy_c *link = dAcPy_c::LINK;
    mVec3_c pos;
    getItemPos(pos);
    f32 linkDistToItem = PSVECSquareDistance(pos, link->position);
    if (EventManager::isInEvent() && getEventStuff().getCurrentEventCommand() == 'wait') {
        mStateMgr.changeState(StateID_Open);
        return;
    }
    if (EventManager::isInEvent() && getEventStuff().getCurrentEventCommand() == 'clos') {
        mMdl.play();
        if (mMdl.getAnm().isStop()) {
            mStateMgr.changeState(StateID_RemainClosed);
        }
        return;
    }

    bool isNotWait;
    dAcItem_c *item;
    if (getItem(item, isNotWait)) {
        if (160000.0f < linkDistToItem) {
            doEvent();
        }
        mStateMgr.changeState(StateID_Open);
    } else if (isNotWait) {
        mStateMgr.changeState(StateID_Open);
    } else if (link->IfCurrentActionToActor(this, 0x3D)) {
        mStateMgr.changeState(StateID_Hold);
    } else {
        if (mMdl.getAnm().isStop()) {
            mStateMgr.changeState(StateID_RemainClosed);
        } else {
            if (getFrame1() <= mMdl.getAnm().getFrame() && getFrame2() <= mMdl.getAnm().getFrame() &&
                linkDistToItem < getDist1PlusDist2Sq()) {
                link->set0x439F(this, 5);
            }

            mMdl.play();
        }
    }
}
void dAcOTowerHandD101_c::finalizeState_Close() {}

void dAcOTowerHandD101_c::initializeState_Open() {
    bool isFlagSet;
    checkFlag(isFlagSet);
    if (!isFlagSet) {
        mMdl.getAnm().setPlayState(m3d::PLAY_MODE_3);
        mMdl.setRate(getOpenRate());
    }
    playSound(0xC0D);
}
void dAcOTowerHandD101_c::executeState_Open() {
    dAcPy_c *link = dAcPy_c::LINK;
    mVec3_c pos;
    getItemPos(pos);
    f32 linkDistToItem = PSVECSquareDistance(pos, link->position);
    if (EventManager::isInEvent() && getEventStuff().getCurrentEventCommand() == 'wait') {
        if (mMdl.getAnm().isStop()) {
            mStateMgr.changeState(StateID_RemainOpen);
        } else {
            mMdl.play();
        }
        return;
    }
    if (EventManager::isInEvent() && getEventStuff().getCurrentEventCommand() == 'clos') {
        if (mMdl.getAnm().isStop()) {
            mStateMgr.changeState(StateID_Close);
        } else {
            mMdl.play();
        }
        return;
    }

    bool isNotWait;
    dAcItem_c *item;
    if (getItem(item, isNotWait)) {
        if (160000.0f < linkDistToItem) {
            doEvent();
        }
        if (mMdl.getAnm().isStop()) {
            mStateMgr.changeState(StateID_Close);
        } else {
            mMdl.play();
        }
    } else if (isNotWait) {
        if (mMdl.getAnm().isStop()) {
            mStateMgr.changeState(StateID_Close);
        } else {
            mMdl.play();
        }
    } else {
        if (mMdl.getAnm().isStop()) {
            mStateMgr.changeState(StateID_Close);
        } else {
            mMdl.play();
        }
    }
}
void dAcOTowerHandD101_c::finalizeState_Open() {}

void dAcOTowerHandD101_c::initializeState_RemainClosed() {
    mMdl.setFrame(mMdl.getAnm().getEndFrame());
    bool isFlagSet = false;
    checkFlag(isFlagSet);
    if (isFlagSet) {
        mFlags |= 1;
    }
    field_0xB08 = 0;
}
void dAcOTowerHandD101_c::executeState_RemainClosed() {
    dAcPy_c *link = dAcPy_c::LINK;
    mVec3_c pos;
    getItemPos(pos);
    f32 linkDistToItem = PSVECSquareDistance(pos, link->position);
    if (EventManager::isInEvent() && getEventStuff().getCurrentEventCommand() == 'wait') {
        mStateMgr.changeState(StateID_Open);
        return;
    }

    if (EventManager::isInEvent() && getEventStuff().getCurrentEventCommand() == 'clos') {
        return;
    }
    if ((mFlags & 1) == 0) {
        bool isNotWait;
        dAcItem_c *item;
        // Okay here the inline theory completely falls apart because
        // the 160000.0f is only being loaded once in the middle of that
        // this suspected inline body does
        if (getItem(item, isNotWait)) {
            if (160000.0f < linkDistToItem) {
                doEvent();
            }
            mStateMgr.changeState(StateID_Open);
        } else if (isNotWait) {
            mStateMgr.changeState(StateID_Open);
        } else {
            if (getDist1PlusDist2Sq() < 160000.0f) {
                mStateMgr.changeState(StateID_Open);
            }
        }
    }
}
void dAcOTowerHandD101_c::finalizeState_RemainClosed() {}

void dAcOTowerHandD101_c::initializeState_Hold() {
    mMdl.getAnm().setPlayState(m3d::PLAY_MODE_1);
    mMdl.setRate(getCloseRate());
    setHoldFlag();
    doSomethingHold(0.0f);
}
void dAcOTowerHandD101_c::executeState_Hold() {
    dAcPy_c *link = dAcPy_c::LINK;
    if (link->IfCurrentActionToActor(this, 0x3D)) {
        mMdl.play();
        f32 linkValue = link->vt_0x090();
        u32 i = 0;
        bool keepGoing = true;
        f32 counter = 0.8f;
        while (keepGoing == true && i < 4) {
            if (counter < linkValue) {
                keepGoing = false;
            } else {
                counter -= 0.2f;
                i++;
            }
        }
        f32 value = field_0xAF4;
        sLib::addCalcScaledDiff(&value, i * 0.25f, 0.5f, 1.0f);
        doSomethingHold(value);
        mMtx_c tmpMtx1;
        tmpMtx1.transS(position);
        // Different address calculation here
        tmpMtx1.ZXYrotM(rotation.x, rotation.y, rotation.z);
        mVec3_c linkVec;
        getLinkOffset(linkVec);
        mMtx_c tmpMtx2;
        tmpMtx2.transS(linkVec);
        tmpMtx1 += tmpMtx2;
        tmpMtx1.YrotM(-0x8000);
        mMtx_c tmpMtx3;
        f32 transScale = 100.0f;
        tmpMtx3.transS(0.0f, 0.0f, field_0xAF4 * transScale);
        tmpMtx1 += tmpMtx3;
        tmpMtx1.XrotM(0x4000);
        link->setTransform(tmpMtx1, 1, 0, 0);
    } else {
        mStateMgr.changeState(StateID_Open);
    }
}
void dAcOTowerHandD101_c::finalizeState_Hold() {
    unsetHoldFlag();
}
