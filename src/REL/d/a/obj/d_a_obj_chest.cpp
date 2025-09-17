#include "d/a/obj/d_a_obj_chest.h"

#include "c/c_lib.h"
#include "common.h"
#include "d/a/d_a_item.h"
#include "d/a/d_a_itembase.h"
#include "d/a/d_a_player.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_s.h"
#include "d/d_message.h"
#include "d/d_sc_game.h"
#include "d/flag/sceneflag_manager.h"
#include "d/flag/storyflag_manager.h"
#include "f/f_base.h"
#include "m/m3d/m_fanm.h"
#include "m/m3d/m_mdl.h"
#include "m/m_angle.h"
#include "m/m_vec.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "nw4r/g3d/res/g3d_resmdl.h"
#include "s/s_Math.h"
#include "toBeSorted/attention.h"
#include "toBeSorted/dowsing_target.h"
#include "toBeSorted/event.h"
#include "toBeSorted/event_manager.h"

const char *dAcOChest_c::CHEST_OPEN = "ChestOpen";
const char *dAcOChest_c::CHEST_OPEN_AFTER = "ChestOpenAfter";
const char *dAcOChest_c::CHEST_FAIL = "ChestFail";

const f32 dAcOChest_c::OPEN_WARDROBE_INTERACTION_FIELD_0x24 = 10.0f;

InteractionTargetDef dAcOChest_c::OPEN_WARDROBE_INTERACTION = {0,     3,     1,       OPEN,   0,     140.0f,
                                                               40.0f, 40.0f, -100.0f, 100.0f, 50.0f, 1.0f};

SPECIAL_ACTOR_PROFILE(OBJ_CHEST, dAcOChest_c, fProfile::OBJ_CHEST, 0x13E, 0, 3);

char *dAcOChest_c::INSIDE_MODEL_NAMES[4] = {
    "TansuInsideA",
    "TansuInsideB",
    "TansuInsideC",
    "TansuInsideD",
};

STATE_DEFINE(dAcOChest_c, Wait);
STATE_DEFINE(dAcOChest_c, OpenEvent);
STATE_DEFINE(dAcOChest_c, FailEvent);
STATE_DEFINE(dAcOChest_c, OrderOpenEventAfter);

void dAcOChest_c::changeStateOpenCallback(dAcOChest_c *chest) {
    if (chest == nullptr) {
        return;
    }
    chest->changeStateOpen();
}

void dAcOChest_c::changeStateWaitCallback(dAcOChest_c *chest) {
    if (chest == nullptr) {
        return;
    }
    chest->changeStateWait();
}

void dAcOChest_c::changeStateFailCallback(dAcOChest_c *chest) {
    if (chest == nullptr) {
        return;
    }
    chest->changeStateFail();
}

void dAcOChest_c::changeStateWait2Callback(dAcOChest_c *chest) {
    if (chest == nullptr) {
        return;
    }
    chest->changeStateWait2();
}

bool dAcOChest_c::createHeap() {
    char *name = getName();
    char *subtype = getSubtypeName();
    char *modelName = getModelName();
    char *openOrClose = getOpenOrClose(0);

    void *mdlData = getOarcResFile(name);
    void *anmData = getOarcResFile(modelName);
    TRY_CREATE(mAnmMdl.create3(*this, mdlData, anmData, subtype, openOrClose, 0x120));

    if (!getFromParams(0x1C, 0xFF)) {
        mAnmMdl.getModel().setPriorityDraw(0x1C, 0xFFFFFFFF);
    }
    if ((s32)getFromParams(0x10, 0xFF) != 0xFF) {
        void *insideMdlData = getOarcResFile("TansuInside");
        mResFile = nw4r::g3d::ResFile(insideMdlData);
        nw4r::g3d::ResMdl mdl = mResFile.GetResMdl(INSIDE_MODEL_NAMES[getFromParams(0x10, 0xFF)]);
        if (!mInsideMdl.create(mdl, &mAllocator, 0x120, 1, nullptr)) {
            return false;
        }
    }

    char *dzbPlcName = getDzbPlcName();
    void *dzb = getOarcDZB(name, dzbPlcName);
    void *plc = getOarcPLC(name, dzbPlcName);
    fn_326_C90();
    bool ok = !mBgW.Set((cBgD_t *)dzb, (PLC *)plc, cBgW::MOVE_BG_e, &mWorldMtx, &mScale);
    if (!ok) {
        return false;
    }
    return mBgW.InitMapStuff(&mAllocator);
}

int dAcOChest_c::create() {
    if (!initAllocatorWork1Heap(0x4000, "dAcOChest_c::m_allocator", 0x20)) {
        return FAILED;
    }
    mBgW.Lock();
    dBgS::GetInstance()->Regist(&mBgW, this);
    dBgS::GetInstance()->RegistBg(&mBgW, this);

    forwardAccel = 0.0f;
    forwardMaxSpeed = 0.0f;

    mStateMgr.changeState(StateID_Wait);

    boundingBox.Set(mVec3_c(-200.0f, -0.0f, -100.0f), mVec3_c(200.0f, 500.0f, 100.0f));

    mAnmMdl.setAnm(getOpenOrClose(0), m3d::PLAY_MODE_4);
    mAnmMdl.setRate(0.0f);

    poscopy2.y = mPosition.y + 150.0f;
    poscopy3 = poscopy2;
    if ((s32)getFromParams(0x10, 0xFF) != 0xFF) {
        mInsideMdl.setLocalMtx(mWorldMtx);
    }
    if (dScGame_c::isCurrentStage("F001r") && mRoomID == 1 && 900.0f < mPosition.x && mPosition.x < 1000.0f &&
        -50.0f < mPosition.y && mPosition.y < 50.0f && -2730.0f < mPosition.z && mPosition.z < -2630.0f) {
        mIsLinksCloset = true;
    }

    mVec3_c dowsingPos(0.0f, 150.0f, 0.0f);
    ITEM_ID itemId = (ITEM_ID)getFromParams(0x8, 0xFF);
    bool isTreasure = dAcItem_c::isTreasure(itemId);
    if (isTreasure) {
        mDowsingTarget.initialize(DowsingTarget::SLOT_TREASURE, 0, &dowsingPos, 0.0f);
    } else if (dAcItem_c::isRupee(itemId)) {
        mDowsingTarget.initialize(DowsingTarget::SLOT_RUPEE, 0, &dowsingPos, 0.0f);
    }
    return SUCCEEDED;
}

int dAcOChest_c::doDelete() {
    return SUCCEEDED;
}
int dAcOChest_c::actorExecute() {
    ITEM_ID itemId = (ITEM_ID)getFromParams(0x8, 0xFF);
    if (!hasBeenOpened() && (dAcItem_c::isTreasure(itemId) || dAcItem_c::isRupee(itemId))) {
        mDowsingTarget.doRegister();
    } else {
        mDowsingTarget.doUnregister();
    }
    mStateMgr.executeState();
    fn_326_C90();
    return SUCCEEDED;
}

int dAcOChest_c::draw() {
    drawModelType1(&mAnmMdl.getModel());
    if (hasInsideModel()) {
        drawModelType1(&mInsideMdl);
    }
    return SUCCEEDED;
}

void dAcOChest_c::initializeState_Wait() {}

void dAcOChest_c::executeState_Wait() {
    OPEN_WARDROBE_INTERACTION.field_0x24 = OPEN_WARDROBE_INTERACTION_FIELD_0x24;
    AttentionManager::GetInstance()->addTarget(*this, OPEN_WARDROBE_INTERACTION, 0, nullptr);
}

void dAcOChest_c::finalizeState_Wait() {}

void dAcOChest_c::initializeState_OpenEvent() {
    stateOpenEventEnter2();
}

void dAcOChest_c::executeState_OpenEvent() {
    stateOpenUpdate2();
}

void dAcOChest_c::finalizeState_OpenEvent() {}

void dAcOChest_c::initializeState_OrderOpenEventAfter() {}

void dAcOChest_c::executeState_OrderOpenEventAfter() {
    if (mGaveItem && EventManager::sInstance->isInEvent0Or7()) {
        char *name = getName();
        void *data = getOarcZev(name);
        Event event(CHEST_OPEN_AFTER, data, 400, 0x100001, nullptr, (void *)changeStateWaitCallback);
        mEvent.scheduleEvent(event, 0);
    } else if (EventManager::sInstance->isInEvent(this, CHEST_OPEN_AFTER)) {
        ITEM_ID itemId = (ITEM_ID)getFromParams(0x8, 0xFF);
        if (dAcItem_c::isRupee(itemId)) {
            dMessage_c::getInstance()->setField_0x2FC(0xFFFFFFFF);
        }
        mField_0x65D = true;
        stateOpenUpdate2();
        mStateMgr.changeState(StateID_OpenEvent);
    }
}
void dAcOChest_c::finalizeState_OrderOpenEventAfter() {}

void dAcOChest_c::initializeState_FailEvent() {
    stateOpenEventEnter2();
}

void dAcOChest_c::executeState_FailEvent() {
    stateOpenUpdate2();
}

void dAcOChest_c::finalizeState_FailEvent() {}

void dAcOChest_c::doInteraction(s32 _unused) {
    char *name = getName();
    void *data = getOarcZev(name);

    if (hasBeenOpened()) {
        Event event(CHEST_FAIL, data, 100, 0x100001, (void *)changeStateFailCallback, (void *)changeStateWait2Callback);
        mEvent.scheduleEvent(event, 0);
    } else {
        Event event(CHEST_OPEN, data, 100, 0x100001, (void *)changeStateOpenCallback, nullptr);
        mEvent.scheduleEvent(event, 0);
    }
}

void dAcOChest_c::fn_326_C90() {
    mWorldMtx.transS(mPosition);
    mWorldMtx.ZXYrotM(mRotation);
    mAnmMdl.getModel().setLocalMtx(mWorldMtx);
    mAnmMdl.getModel().calc(false);
}

void dAcOChest_c::changeStateOpen() {
    if (mField_0x65D) {
        return;
    }
    u32 flag = getFromParams(0, 0xFF);
    if (flag < 0xFF) {
        SceneflagManager::sInstance->setFlag(mRoomID, flag);
    }
    fn_326_1470();
    mStateMgr.changeState(StateID_OpenEvent);
}

void dAcOChest_c::changeStateWait() {
    if (!mField_0x65D) {
        return;
    }
    fn_326_1440();
    fn_326_14A0();
    mStateMgr.changeState(StateID_Wait);
}

void dAcOChest_c::changeStateFail() {
    fn_326_1470();
    mStateMgr.changeState(StateID_FailEvent);
}

void dAcOChest_c::changeStateWait2() {
    fn_326_1440();
    fn_326_14A0();
    mStateMgr.changeState(StateID_Wait);
}

char *dAcOChest_c::getName() {
    return "Tansu";
}

char *dAcOChest_c::getSubtypeName() {
    switch (getSubtype()) {
        case 0:  return "TansuA";
        case 1:  return "TansuB";
        case 2:  return "TansuC";
        case 3:  return "TansuD";
        default: return "TansuA";
    }
}

char *dAcOChest_c::getModelName() {
    return "Tansu";
}

char *dAcOChest_c::getOpenOrClose(u8 openOrClose) {
    switch (openOrClose) {
        case 0:  return "TansuOpen";
        case 1:  return "TansuClose";
        default: return nullptr;
    }
}

char *dAcOChest_c::getDzbPlcName() {
    switch (getSubtype()) {
        case 0:  return "TansuA";
        case 1:  return "TansuB";
        case 2:  return "TansuC";
        case 3:  return "TansuD";
        default: return "TansuA";
    }
}

s32 dAcOChest_c::getSubtype() {
    return getFromParams(0x1C, 0xFF);
}

bool dAcOChest_c::hasBeenOpened() {
    if ((s32)getFromParams(8, 0xFF) == 0xFF) {
        return true;
    }
    u32 flag = getFromParams(0, 0xFF);
    if (flag >= 0xFF) {
        return true;
    }
    return (flag < 0xFF) && SceneflagManager::sInstance->checkBoolFlag(mRoomID, flag);
}

void dAcOChest_c::stateOpenUpdate2() {
    dAcPy_c *player = dAcPy_c::LINK;
    switch (mEvent.getCurrentEventCommand()) {
        case 'wait': {
            mEvent.advanceNext();
            break;
        }
        case 'move': {
            if (mEvent.isAdvance()) {
                int i;
                mEvent.getSingleIntData(&i, 'type', 0);
                if (i == 1) {
                    char *openOrClose = getOpenOrClose(0);
                    mAnmMdl.setAnm(openOrClose, m3d::PLAY_MODE_4);
                    mAnmMdl.setFrame(0.0f);
                    mAnmMdl.setRate(1.0f);
                    mAnmMdl.getAnm().setPlayState(m3d::PLAY_MODE_1);
                } else if (i == 2) {
                    char *openOrClose = getOpenOrClose(1);
                    mAnmMdl.setAnm(openOrClose, m3d::PLAY_MODE_4);
                    mAnmMdl.setFrame(0.0f);
                    mAnmMdl.setRate(1.0f);
                    mAnmMdl.getAnm().setPlayState(m3d::PLAY_MODE_1);
                }
            } else {
                mAnmMdl.play();
                if (mAnmMdl.getAnm().isStop()) {
                    mEvent.advanceNext();
                }
            }
            break;
        }
        case 'comp': {
            mVec3_c targetPosition;
            mVec3_c playerPosition;
            mVec3_c t(0.0f, 0.0f, 122.35f);
            s32 targetAngle = 0x7FFF;
            targetPosition = t;
            targetAngle += mRotation.y;
            targetPosition.rotY(mRotation.y);
            targetPosition += mPosition;

            if (player != nullptr) {
                playerPosition = player->mPosition;
                cLib::addCalcPos(&playerPosition, targetPosition, 0.25f, 200.0f, 0.0f);
                s16 YRot = mRotation.y;
                sLib::addCalcAngle(&YRot, targetAngle, 4, 0x7FFF, 0);

                if ((targetPosition.squareDistanceToXZ(playerPosition) < 25.0f) &&
                    mAng::abs(YRot - mAng(targetAngle)) < 0xb6) {
                    playerPosition = targetPosition;
                    YRot = targetAngle;
                    mEvent.advanceNext();
                }
                player->setPosYRot(playerPosition, YRot, 0, 0, 0);
            } else {
                mEvent.advanceNext();
            }
            break;
        }
        case 'item': {
            ITEM_ID itemId = (ITEM_ID)getFromParams(0x8, 0xFF);
            dAcItem_c::giveItem(itemId, 0, 0);
            mGaveItem = true;
            mStateMgr.changeState(StateID_OrderOpenEventAfter);
            mEvent.advanceNext();
            break;
        }
        case '????': {
            break;
        }
        default: {
            mEvent.advanceNext();
        }
    }
}

void dAcOChest_c::stateOpenEventEnter2() {
    if ((s32)getFromParams(0x10, 0xFF) == 0xFF) {
        return;
    }
    mHasInsideModel = true;
}

void dAcOChest_c::fn_326_1440() {
    if ((s32)getFromParams(0x10, 0xFF) == 0xFF) {
        return;
    }
    mHasInsideModel = false;
}

bool dAcOChest_c::hasInsideModel() {
    return mHasInsideModel;
}

void dAcOChest_c::fn_326_1470() {
    if (!mIsLinksCloset) {
        return;
    }
    StoryflagManager::sInstance->setFlag(STORYFLAG_ALTERNATE_WARDROBE_TEXT);
}

void dAcOChest_c::fn_326_14A0() {
    if (!mIsLinksCloset) {
        return;
    }
    StoryflagManager::sInstance->unsetFlag(STORYFLAG_ALTERNATE_WARDROBE_TEXT);
}

const u32 dAcOChest_c::unused = 0;
