#include "d/lyt/meter/d_lyt_meter_drink.h"

#include "common.h"
#include "d/lyt/d2d.h"
#include "m/m_vec.h"
#include "nw4r/lyt/lyt_pane.h"
#include "toBeSorted/file_manager.h"

STATE_DEFINE(dLytMeterDrinkParts_c, Invisible);
STATE_DEFINE(dLytMeterDrinkParts_c, In);
STATE_DEFINE(dLytMeterDrinkParts_c, Visible);
STATE_DEFINE(dLytMeterDrinkParts_c, Out);

STATE_DEFINE(dLytMeterDrink_c, Invisible);
STATE_DEFINE(dLytMeterDrink_c, In);
STATE_DEFINE(dLytMeterDrink_c, Wait);
STATE_DEFINE(dLytMeterDrink_c, Move);
STATE_DEFINE(dLytMeterDrink_c, Out);

void dLytMeterDrinkParts_c::initializeState_Invisible() {
    if (field_0x51) {
        if (mAnmGroups[2]->isEnabled()) {
            mAnmGroups[2]->setAnimEnable(false);
        }
        if (!mAnmGroups[0]->isEnabled()) {
            mAnmGroups[0]->setAnimEnable(true);
        }
        mAnmGroups[0]->setFrame(0.0f);
        field_0x48 = 6;
        mDrinkType = TYPE_NONE;
        field_0x51 = 0;
    }
}
void dLytMeterDrinkParts_c::executeState_Invisible() {
    if (mAnmGroups[0]->isEnabled()) {
        mAnmGroups[0]->setAnimEnable(false);
    }
    if (field_0x50) {
        mStateMgr.changeState(StateID_In);
    }
}
void dLytMeterDrinkParts_c::finalizeState_Invisible() {}

void dLytMeterDrinkParts_c::initializeState_In() {
    if (field_0x51) {
        if (mAnmGroups[2]->isEnabled()) {
            mAnmGroups[2]->setAnimEnable(false);
        }
        mAnmGroups[0]->setToEnd();
        field_0x51 = 0;
    } else {
        mAnmGroups[0]->setFrame(0.0f);
    }
    mAnmGroups[0]->setAnimEnable(true);
}
void dLytMeterDrinkParts_c::executeState_In() {
    if (mAnmGroups[0]->isEndReached()) {
        mAnmGroups[0]->setAnimEnable(false);
        mStateMgr.changeState(StateID_Visible);
    }

    if (mAnmGroups[0]->isEnabled()) {
        mAnmGroups[0]->play();
    }
}
void dLytMeterDrinkParts_c::finalizeState_In() {}

void dLytMeterDrinkParts_c::initializeState_Visible() {
    if (field_0x51) {
        if (mAnmGroups[2]->isEnabled()) {
            mAnmGroups[2]->setAnimEnable(false);
        }
        mAnmGroups[0]->setAnimEnable(true);
        mAnmGroups[0]->setToEnd();
        field_0x51 = 0;
    }
}

void dLytMeterDrinkParts_c::executeState_Visible() {
    if (mAnmGroups[0]->isEnabled()) {
        mAnmGroups[0]->setAnimEnable(false);
    }

    if (field_0x48 != getDrinkFrame(mDrinkType)) {
        mStateMgr.changeState(StateID_Out);
    }
}
void dLytMeterDrinkParts_c::finalizeState_Visible() {}

void dLytMeterDrinkParts_c::initializeState_Out() {
    if (field_0x51) {
        if (mAnmGroups[0]->isEnabled()) {
            mAnmGroups[0]->setAnimEnable(false);
        }
        mAnmGroups[2]->setToEnd();
        field_0x51 = 0;
    } else {
        mAnmGroups[2]->setFrame(0.0f);
    }

    mAnmGroups[2]->setAnimEnable(true);
}
void dLytMeterDrinkParts_c::executeState_Out() {
    if (mAnmGroups[2]->isEndReached()) {
        mAnmGroups[2]->setAnimEnable(false);
        field_0x48 = 6;
        mDrinkType = TYPE_NONE;
        field_0x50 = 0;
        mStateMgr.changeState(StateID_Invisible);
    }
    if (mAnmGroups[2]->isEnabled()) {
        mAnmGroups[2]->play();
    }
}
void dLytMeterDrinkParts_c::finalizeState_Out() {}

void dLytMeterDrinkParts_c::init() {
    field_0x50 = 0;
    field_0x51 = 0;
    field_0x48 = 6;
    mDrinkType = TYPE_NONE;
    mAnmGroups[1]->setAnimEnable(true);
    mStateMgr.changeState(StateID_Invisible);
}

void dLytMeterDrinkParts_c::execute() {
    mStateMgr.executeState();
    if (field_0x48 != 6) {
        mAnmGroups[1]->setFrame(field_0x48);
    }
}

dLytMeterDrinkParts_c::DrinkFrame_e dLytMeterDrinkParts_c::getDrinkFrame(dLytMeterDrinkParts_c::DrinkType_e type
) const {
    if (type == TYPE_STAMINA) {
        if (FileManager::GetInstance()->getStaminaPotionPlusTimer()) {
            return FRAME_STAMINA_PLUS;
        }
        if (FileManager::GetInstance()->getStaminaPotionTimer()) {
            return FRAME_STAMINA;
        }
    }
    if (type == TYPE_AIR) {
        if (FileManager::GetInstance()->getAirPotionPlusTimer()) {
            return FRAME_AIR_PLUS;
        }
        if (FileManager::GetInstance()->getAirPotionTimer()) {
            return FRAME_AIR;
        }
    }
    if (type == TYPE_GUARDIAN) {
        if (FileManager::GetInstance()->getGuardianPotionPlusTimer()) {
            return FRAME_GUARDIAN_PLUS;
        }
        if (FileManager::GetInstance()->getGuardianPotionTimer()) {
            return FRAME_GUARDIAN;
        }
    }
    return FRAME_NONE;
}

bool dLytMeterDrinkParts_c::isDrinkAboutToExpire() const {
    if (field_0x48 == FRAME_STAMINA_PLUS) {
        return FileManager::GetInstance()->getStaminaPotionPlusTimer() < 600;
    } else if (field_0x48 == FRAME_STAMINA) {
        return FileManager::GetInstance()->getStaminaPotionTimer() < 600;
    } else if (field_0x48 == FRAME_AIR_PLUS) {
        return FileManager::GetInstance()->getAirPotionPlusTimer() < 600;
    } else if (field_0x48 == FRAME_AIR) {
        return FileManager::GetInstance()->getAirPotionTimer() < 600;
    } else if (field_0x48 == FRAME_GUARDIAN_PLUS) {
        return FileManager::GetInstance()->getGuardianPotionPlusTimer() < 600;
    } else if (field_0x48 == FRAME_GUARDIAN) {
        return FileManager::GetInstance()->getGuardianPotionTimer() < 600;
    }
    return false;
}

void dLytMeterDrinkParts_c::copy(dLytMeterDrinkParts_c *other, dLytMeterDrinkParts_c *third) {
    field_0x50 = other->field_0x50;
    field_0x51 = 1;
    field_0x48 = other->field_0x48;
    mDrinkType = other->mDrinkType;
    mStateMgr.changeState(*other->mStateMgr.getStateID());
    if (third == nullptr) {
        other->field_0x50 = 0;
        other->field_0x51 = 1;
        other->field_0x48 = FRAME_NONE;
        other->mDrinkType = TYPE_NONE;
        other->mStateMgr.changeState(StateID_Invisible);
    } else {
        other->field_0x50 = third->field_0x50;
        other->field_0x51 = 1;
        other->field_0x48 = third->field_0x48;
        other->mDrinkType = third->mDrinkType;
        other->mStateMgr.changeState(*third->mStateMgr.getStateID());

        third->field_0x50 = 0;
        third->field_0x51 = 1;
        third->field_0x48 = FRAME_NONE;
        third->mDrinkType = TYPE_NONE;
        third->mStateMgr.changeState(StateID_Invisible);
    }
}

static const char *sPanes[] = {
    "N_drink_00",
    "N_drink_01",
    "N_drink_02",
};

static const d2d::LytBrlanMapping brlanMap[] = {
    { "drink_00_demoIn.brlan", "G_demoInOut_00"},
    {     "drink_00_in.brlan",     "G_inOut_00"},
    {     "drink_00_in.brlan",     "G_inOut_01"},
    {     "drink_00_in.brlan",     "G_inOut_02"},
    {  "drink_00_drink.brlan",     "G_drink_00"},
    {  "drink_00_drink.brlan",     "G_drink_01"},
    {  "drink_00_drink.brlan",     "G_drink_02"},
    {   "drink_00_loop.brlan",      "G_loop_00"},
    {   "drink_00_loop.brlan",      "G_loop_01"},
    {   "drink_00_loop.brlan",      "G_loop_02"},
    {"drink_00_caution.brlan",   "G_caution_00"},
    {"drink_00_caution.brlan",   "G_caution_01"},
    {"drink_00_caution.brlan",   "G_caution_02"},
    {   "drink_00_move.brlan",      "G_move_00"},
    {   "drink_00_move.brlan",      "G_move_01"},
    {   "drink_00_move.brlan",      "G_move_02"},
    {    "drink_00_out.brlan",     "G_inOut_00"},
    {    "drink_00_out.brlan",     "G_inOut_01"},
    {    "drink_00_out.brlan",     "G_inOut_02"},
    {"drink_00_demoOut.brlan", "G_demoInOut_00"},
};

#define LYT_METER_DRINK_OFFSET_ANIM_LOOP 7
#define LYT_METER_DRINK_OFFSET_ANIM_CAUTION 10
#define LYT_METER_DRINK_OFFSET_ANIM_MOVE 13

#define LYT_METER_DRINK_ANIM_DEMO_OUT 19

void dLytMeterDrink_c::initializeState_Invisible() {}
void dLytMeterDrink_c::executeState_Invisible() {
    if (field_0x6F4) {
        mStateMgr.changeState(StateID_In);
    }
}
void dLytMeterDrink_c::finalizeState_Invisible() {}

void dLytMeterDrink_c::initializeState_In() {
    mAnmGroups[0].setAnimEnable(true);
    mAnmGroups[0].setFrame(0.0f);
}
void dLytMeterDrink_c::executeState_In() {
    if (mAnmGroups[0].isEndReached()) {
        mAnmGroups[0].setAnimEnable(false);
        mStateMgr.changeState(StateID_Wait);
    }
    if (mAnmGroups[0].isEnabled()) {
        mAnmGroups[0].play();
    }
}
void dLytMeterDrink_c::finalizeState_In() {}

void dLytMeterDrink_c::initializeState_Wait() {
    field_0x6F0 = -1;
}
void dLytMeterDrink_c::executeState_Wait() {
    int i5 = -1;
    int i7 = -1;
    for (int i = 0; i < 3; i++) {
        int a = getPartForDrinkType((dLytMeterDrinkParts_c::DrinkType_e)i);
        if (fn_800E5C40(i) != 0 && a == 3) {
            i5 = i;
        }
        if (field_0x6F5[i] == 1 && mParts[i].field_0x50 == 0) {
            i7 = i;
        }
    }

    if (i5 >= 0) {
        for (int i = 0; i < 3; i++) {
            if (mParts[i].field_0x50 == 0 && i7 != i) {
                mParts[i].field_0x50 = 1;
                mParts[i].field_0x51 = 0;
                mParts[i].field_0x48 = mParts[i].getDrinkFrame((dLytMeterDrinkParts_c::DrinkType_e)i5);
                mParts[i].mDrinkType = (dLytMeterDrinkParts_c::DrinkType_e)i5;
                break;
            }
        }
    }

    switch (i7) {
        case 0:
            if (mParts[1].field_0x50 != 0) {
                field_0x6F0 = i7;
                mStateMgr.changeState(StateID_Move);
            }
            break;
        case 1:
            if (mParts[2].field_0x50 != 0) {
                field_0x6F0 = i7;
                mStateMgr.changeState(StateID_Move);
            }
            break;
    }
    if (field_0x6F4 == 0) {
        mStateMgr.changeState(StateID_Out);
    }
}
void dLytMeterDrink_c::finalizeState_Wait() {}

void dLytMeterDrink_c::initializeState_Move() {
    for (int i = field_0x6F0; i < 3; i++) {
        mAnmGroups[LYT_METER_DRINK_OFFSET_ANIM_MOVE + i].setAnimEnable(true);
        mAnmGroups[LYT_METER_DRINK_OFFSET_ANIM_MOVE + i].setFrame(0.0f);
    }
}
void dLytMeterDrink_c::executeState_Move() {
    bool needsCopy = false;
    for (int i = field_0x6F0; i < 3; i++) {
        if (mAnmGroups[LYT_METER_DRINK_OFFSET_ANIM_MOVE + i].isEndReached()) {
            mAnmGroups[LYT_METER_DRINK_OFFSET_ANIM_MOVE + i].setFrame(0.0f);
            needsCopy = true;
        }
    }

    if (needsCopy) {
        if (field_0x6F0 == 0) {
            if (mParts[1].field_0x50) {
                mParts[0].copy(&mParts[1], &mParts[2]);
            }
        } else if (field_0x6F0 == 1 && mParts[2].field_0x50) {
            mParts[1].copy(&mParts[2], nullptr);
        }

        mLyt.calc();

        for (int i = 0; i < 3; i++) {
            if (mAnmGroups[LYT_METER_DRINK_OFFSET_ANIM_MOVE + i].isEnabled()) {
                mAnmGroups[LYT_METER_DRINK_OFFSET_ANIM_MOVE + i].setAnimEnable(false);
            }
        }
        mStateMgr.changeState(StateID_Wait);
    }

    for (int i = 0; i < 3; i++) {
        if (mAnmGroups[LYT_METER_DRINK_OFFSET_ANIM_MOVE + i].isEnabled()) {
            mAnmGroups[LYT_METER_DRINK_OFFSET_ANIM_MOVE + i].play();
        }
    }
}
void dLytMeterDrink_c::finalizeState_Move() {}

void dLytMeterDrink_c::initializeState_Out() {
    mAnmGroups[LYT_METER_DRINK_ANIM_DEMO_OUT].setAnimEnable(true);
    mAnmGroups[LYT_METER_DRINK_ANIM_DEMO_OUT].setFrame(0.0f);
}
void dLytMeterDrink_c::executeState_Out() {
    if (mAnmGroups[LYT_METER_DRINK_ANIM_DEMO_OUT].isEndReached()) {
        mAnmGroups[LYT_METER_DRINK_ANIM_DEMO_OUT].setAnimEnable(false);
        mStateMgr.changeState(StateID_Invisible);
    }
    if (mAnmGroups[LYT_METER_DRINK_ANIM_DEMO_OUT].isEnabled()) {
        mAnmGroups[LYT_METER_DRINK_ANIM_DEMO_OUT].play();
    }
}
void dLytMeterDrink_c::finalizeState_Out() {}

bool dLytMeterDrink_c::build(d2d::ResAccIf_c *resAcc) {
    mLyt.setResAcc(resAcc);
    mLyt.build("drink_00.brlyt", nullptr);
    for (int i = 0; i < 3; i++) {
        mpPartPanes[i] = mLyt.findPane(sPanes[i]);
    }

    for (int i = 0; i < 20; i++) {
        mAnmGroups[i].init(brlanMap[i].mFile, resAcc, mLyt.getLayout(), brlanMap[i].mName);
        mAnmGroups[i].bind(false);
        mAnmGroups[i].setAnimEnable(false);
    }

    for (int i = 0; i < 3; i++) {
        mParts[i].mAnmGroups[0] = &mAnmGroups[1 + i];
        mParts[i].mAnmGroups[1] = &mAnmGroups[4 + i];
        mParts[i].mAnmGroups[2] = &mAnmGroups[16 + i];
        mParts[i].init();
        field_0x6F5[i] = mParts[i].field_0x50;
    }

    mpPane = mLyt.getLayout()->GetRootPane();

    field_0x6F0 = -1;
    field_0x6F4 = 0;
    field_0x6E0.z = 0.0f;
    field_0x6E0.y = 0.0f;
    field_0x6E0.x = 0.0f;
    mpPane->SetTranslate(field_0x6E0);

    mAnmGroups[0].setFrame(0.0f);
    mAnmGroups[0].setAnimEnable(true);
    mLyt.calc();
    mAnmGroups[0].setAnimEnable(false);

    mStateMgr.changeState(StateID_Invisible);

    return true;
}

bool dLytMeterDrink_c::remove() {
    for (int i = 0; i < 20; i++) {
        mAnmGroups[i].unbind();
        mAnmGroups[i].remove();
    }

    return true;
}

bool dLytMeterDrink_c::execute() {
    mStateMgr.executeState();

    for (int i = 0; i < 3; i++) {
        field_0x6F5[i] = mParts[i].field_0x50;
        mParts[i].execute();
    }

    executeInternal();
    mpPane->SetTranslate(field_0x6E0);
    mLyt.calc();
    return true;
}

bool dLytMeterDrink_c::draw() {
    mLyt.addToDrawList();
    return true;
}

bool dLytMeterDrink_c::fn_800E5C40(int i) {
    if ((i == 0 || i == 3) && (FileManager::GetInstance()->getStaminaPotionTimer() != 0 ||
                               FileManager::GetInstance()->getStaminaPotionPlusTimer() != 0)) {
        return true;
    }

    if ((i == 1 || i == 3) &&
        (FileManager::GetInstance()->getAirPotionTimer() != 0 || FileManager::GetInstance()->getAirPotionPlusTimer() != 0)) {
        return true;
    }

    if ((i == 2 || i == 3) && (FileManager::GetInstance()->getGuardianPotionTimer() != 0 ||
                               FileManager::GetInstance()->getGuardianPotionPlusTimer() != 0)) {
        return true;
    }

    return false;
}

int dLytMeterDrink_c::getPartForDrinkType(dLytMeterDrinkParts_c::DrinkType_e ty) const {
    for (int i = 0; i < 3; i++) {
        if (ty == mParts[i].mDrinkType) {
            return i;
        }
    }
    return 3;
}

void dLytMeterDrink_c::executeInternal() {
    bool bHasNotSoonExpiringDrink = false;
    bool bHasSoonExpiringDrink = false;
    bool drinksLast[3];

    f32 frameLoop = 0.0f;
    f32 frameCaution = 0.0f;

    for (int i = 0; i < 3; i++) {
        if (!mParts[i].isDrinkAboutToExpire()) {
            drinksLast[i] = true;
            bHasNotSoonExpiringDrink = true;
            if (mAnmGroups[LYT_METER_DRINK_OFFSET_ANIM_LOOP + i].isEnabled()) {
                frameLoop = mAnmGroups[LYT_METER_DRINK_OFFSET_ANIM_LOOP + i].getNextFrame();
            }
        } else {
            drinksLast[i] = false;
            bHasSoonExpiringDrink = true;
            if (mAnmGroups[LYT_METER_DRINK_OFFSET_ANIM_CAUTION + i].isEnabled()) {
                frameCaution = mAnmGroups[LYT_METER_DRINK_OFFSET_ANIM_CAUTION + i].getNextFrame();
            }
        }
    }

    if (bHasNotSoonExpiringDrink) {
        for (int i = 0; i < 3; i++) {
            if (drinksLast[i] == true) {
                if (mAnmGroups[LYT_METER_DRINK_OFFSET_ANIM_LOOP + i].isEnabled()) {
                    mAnmGroups[LYT_METER_DRINK_OFFSET_ANIM_LOOP + i].play();
                } else {
                    mAnmGroups[LYT_METER_DRINK_OFFSET_ANIM_LOOP + i].setAnimEnable(true);
                    mAnmGroups[LYT_METER_DRINK_OFFSET_ANIM_LOOP + i].setFrame(frameLoop);
                }

                if (mAnmGroups[LYT_METER_DRINK_OFFSET_ANIM_CAUTION + i].isEnabled()) {
                    bool oldVisible = mpPartPanes[i]->IsVisible();
                    mpPartPanes[i]->SetVisible(true);
                    mAnmGroups[LYT_METER_DRINK_OFFSET_ANIM_CAUTION + i].setFrame(0.0f);
                    mLyt.calc();
                    mAnmGroups[LYT_METER_DRINK_OFFSET_ANIM_CAUTION + i].setAnimEnable(false);
                    mpPartPanes[i]->SetVisible(oldVisible);
                }
            }
        }
    } else {
        for (int i = 0; i < 3; i++) {
            if (mAnmGroups[LYT_METER_DRINK_OFFSET_ANIM_LOOP + i].isEnabled()) {
                bool oldVisible = mpPartPanes[i]->IsVisible();
                mpPartPanes[i]->SetVisible(true);
                mAnmGroups[LYT_METER_DRINK_OFFSET_ANIM_LOOP + i].setFrame(0.0f);
                mLyt.calc();
                mAnmGroups[LYT_METER_DRINK_OFFSET_ANIM_LOOP + i].setAnimEnable(false);
                mpPartPanes[i]->SetVisible(oldVisible);
            }
        }
    }

    if (bHasSoonExpiringDrink) {
        for (int i = 0; i < 3; i++) {
            if (drinksLast[i] == false) {
                if (mAnmGroups[LYT_METER_DRINK_OFFSET_ANIM_CAUTION + i].isEnabled()) {
                    mAnmGroups[LYT_METER_DRINK_OFFSET_ANIM_CAUTION + i].play();
                } else {
                    mAnmGroups[LYT_METER_DRINK_OFFSET_ANIM_CAUTION + i].setAnimEnable(true);
                    mAnmGroups[LYT_METER_DRINK_OFFSET_ANIM_CAUTION + i].setFrame(frameCaution);
                }

                if (mAnmGroups[LYT_METER_DRINK_OFFSET_ANIM_LOOP + i].isEnabled()) {
                    bool oldVisible = mpPartPanes[i]->IsVisible();
                    mpPartPanes[i]->SetVisible(true);
                    mAnmGroups[LYT_METER_DRINK_OFFSET_ANIM_LOOP + i].setFrame(0.0f);
                    mLyt.calc();
                    mAnmGroups[LYT_METER_DRINK_OFFSET_ANIM_LOOP + i].setAnimEnable(false);
                    mpPartPanes[i]->SetVisible(oldVisible);
                }
            }
        }
    } else {
        for (int i = 0; i < 3; i++) {
            if (mAnmGroups[LYT_METER_DRINK_OFFSET_ANIM_CAUTION + i].isEnabled()) {
                bool oldVisible = mpPartPanes[i]->IsVisible();
                mpPartPanes[i]->SetVisible(true);
                mAnmGroups[LYT_METER_DRINK_OFFSET_ANIM_CAUTION + i].setFrame(0.0f);
                mLyt.calc();
                mAnmGroups[LYT_METER_DRINK_OFFSET_ANIM_CAUTION + i].setAnimEnable(false);
                mpPartPanes[i]->SetVisible(oldVisible);
            }
        }
    }
}
