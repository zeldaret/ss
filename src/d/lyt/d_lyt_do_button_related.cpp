#include "common.h"
#include "d/lyt/d2d.h"
#include "d/lyt/d_lyt_do_button.h"
#include "d/lyt/d_lyt_unknowns.h"
#include "d/lyt/meter/d_lyt_meter.h"
#include "toBeSorted/misc_actor.h"

class LytDoButtonRelatedClass {
public:
    LytDoButtonRelatedClass();
    virtual ~LytDoButtonRelatedClass();

private:
    /* 0x04 */ u8 field_0x04;
    /* 0x05 */ u8 field_0x05;
    /* 0x06 */ u8 field_0x06;
};

static LytDoButtonRelatedClass sDoButtonRelatedClass;

LytDoButtonRelatedClass::LytDoButtonRelatedClass() {
    field_0x04 = 0;
    field_0x05 = 0;
    field_0x06 = LytDoButtonRelated::ACT_IE_NONE;
}
LytDoButtonRelatedClass::~LytDoButtonRelatedClass() {}

LytDoButtonRelated *LytDoButtonRelated::sInstance;

LytDoButtonRelated::LytDoButtonRelated() {
    sInstance = this;
}

bool LytDoButtonRelated::build(d2d::ResAccIf_c *resAcc) {
    for (int i = 0; i < 12; i++) {
        field_0x04[i] = ACT_IE_NONE;
        field_0x34[i] = 0;
    }

    return true;
}

bool LytDoButtonRelated::remove() {
    return true;
}

static const dLytDobutton_c::Act_Do_e sActIEtoDoConversionTable[LytDoButtonRelated::ACT_IE_NONE] = {
    dLytDobutton_c::ACT_DO_CLIMB_UP,     // ACT_IE_INFO_CLIMB_UP
    dLytDobutton_c::ACT_DO_JUMP,         // ACT_IE_INFO_JUMP
    dLytDobutton_c::ACT_DO_DROP_DOWN,    // ACT_IE_INFO_DROP_DOWN
    dLytDobutton_c::ACT_DO_LET_GO,       // ACT_IE_INFO_LET_GO
    dLytDobutton_c::ACT_DO_OPEN,         // ACT_IE_INFO_OPEN
    dLytDobutton_c::ACT_DO_PUT_DOWN,     // ACT_IE_INFO_DROP
    dLytDobutton_c::ACT_DO_THROW,        // ACT_IE_INFO_THROW
    dLytDobutton_c::ACT_DO_BURROW,       // ACT_IE_INFO_BURROW
    dLytDobutton_c::ACT_DO_ROLL,         // ACT_IE_INFO_ROLL
    dLytDobutton_c::ACT_DO_9,            // ACT_IE_INFO_9
    dLytDobutton_c::ACT_DO_MOVE,         // ACT_IE_INFO_MOVE
    dLytDobutton_c::ACT_DO_GRAB,         // ACT_IE_INFO_GRAB
    dLytDobutton_c::ACT_DO_SHOOT,        // ACT_IE_INFO_SHOOT
    dLytDobutton_c::ACT_DO_13,           // ACT_IE_INFO_13
    dLytDobutton_c::ACT_DO_14,           // ACT_IE_INFO_14
    dLytDobutton_c::ACT_DO_15,           // ACT_IE_INFO_15
    dLytDobutton_c::ACT_DO_TALK,         // ACT_IE_INFO_TALK
    dLytDobutton_c::ACT_DO_EXAMINE,      // ACT_IE_INFO_EXAMINE
    dLytDobutton_c::ACT_DO_DRAW,         // ACT_IE_INFO_DRAW
    dLytDobutton_c::ACT_DO_19,           // ACT_IE_INFO_19
    dLytDobutton_c::ACT_DO_BLOW,         // ACT_IE_INFO_BLOW
    dLytDobutton_c::ACT_DO_21,           // ACT_IE_INFO_21
    dLytDobutton_c::ACT_DO_22,           // ACT_IE_INFO_22
    dLytDobutton_c::ACT_DO_SWIM,         // ACT_IE_INFO_SWIM
    dLytDobutton_c::ACT_DO_LEAP,         // ACT_IE_INFO_LEAP
    dLytDobutton_c::ACT_DO_DASH,         // ACT_IE_INFO_DASH
    dLytDobutton_c::ACT_DO_GET_OUT,      // ACT_IE_INFO_GET_OUT
    dLytDobutton_c::ACT_DO_27,           // ACT_IE_INFO_27
    dLytDobutton_c::ACT_DO_PICK_UP,      // ACT_IE_INFO_PICK_UP
    dLytDobutton_c::ACT_DO_SIT,          // ACT_IE_INFO_SIT
    dLytDobutton_c::ACT_DO_STAND,        // ACT_IE_INFO_STAND
    dLytDobutton_c::ACT_DO_DIG,          // ACT_IE_INFO_DIG
    dLytDobutton_c::ACT_DO_32,           // ACT_IE_INFO_32
    dLytDobutton_c::ACT_DO_33,           // ACT_IE_INFO_33
    dLytDobutton_c::ACT_DO_BREAK_FREE,   // ACT_IE_INFO_BREAK_FREE
    dLytDobutton_c::ACT_DO_35,           // ACT_IE_INFO_35
    dLytDobutton_c::ACT_DO_36,           // ACT_IE_INFO_36
    dLytDobutton_c::ACT_DO_PLANT,        // ACT_IE_INFO_PLANT
    dLytDobutton_c::ACT_DO_CATCH,        // ACT_IE_INFO_CATCH
    dLytDobutton_c::ACT_DO_SPEED_UP,     // ACT_IE_INFO_SPEED_UP
    dLytDobutton_c::ACT_DO_SAILCLOTH,    // ACT_IE_INFO_SAILCLOTH
    dLytDobutton_c::ACT_DO_PUT_AWAY,     // ACT_IE_INFO_PUT_AWAY
    dLytDobutton_c::ACT_DO_EMERGE,       // ACT_IE_INFO_EMERGE
    dLytDobutton_c::ACT_DO_INSERT,       // ACT_IE_INFO_INSERT
    dLytDobutton_c::ACT_DO_44,           // ACT_IE_INFO_44
    dLytDobutton_c::ACT_DO_TURN,         // ACT_IE_INFO_TURN
    dLytDobutton_c::ACT_DO_PUSH_IN,      // ACT_IE_INFO_ALIGN
    dLytDobutton_c::ACT_DO_47,           // ACT_IE_INFO_47
    dLytDobutton_c::ACT_DO_DRAW_BOW,     // ACT_IE_INFO_DRAW_BOW
    dLytDobutton_c::ACT_DO_49,           // ACT_IE_INFO_49
    dLytDobutton_c::ACT_DO_50,           // ACT_IE_INFO_50
    dLytDobutton_c::ACT_DO_51,           // ACT_IE_INFO_51
    dLytDobutton_c::ACT_DO_SWING,        // ACT_IE_INFO_SWING
    dLytDobutton_c::ACT_DO_BALANCE,      // ACT_IE_INFO_BALANCE
    dLytDobutton_c::ACT_DO_JOSTLE,       // ACT_IE_INFO_JOSTLE
    dLytDobutton_c::ACT_DO_55,           // ACT_IE_INFO_55
    dLytDobutton_c::ACT_DO_SLEEP,        // ACT_IE_INFO_SLEEP
    dLytDobutton_c::ACT_DO_DROP,         // ACT_IE_INFO_DROP_2
    dLytDobutton_c::ACT_DO_LAUNCH,       // ACT_IE_INFO_LAUNCH
    dLytDobutton_c::ACT_DO_READ,         // ACT_IE_INFO_READ
    dLytDobutton_c::ACT_DO_WHIP,         // ACT_IE_INFO_WHIP
    dLytDobutton_c::ACT_DO_PULL,         // ACT_IE_INFO_PULL
    dLytDobutton_c::ACT_DO_DRINK,        // ACT_IE_INFO_DRINK
    dLytDobutton_c::ACT_DO_63,           // ACT_IE_INFO_63
    dLytDobutton_c::ACT_DO_GET_IN,       // ACT_IE_INFO_GET_IN
    dLytDobutton_c::ACT_DO_FIRE,         // ACT_IE_INFO_FIRE
    dLytDobutton_c::ACT_DO_ATTACK,       // ACT_IE_INFO_ATTACK
    dLytDobutton_c::ACT_DO_FATAL_BLOW,   // ACT_IE_INFO_FATAL_BLOW
    dLytDobutton_c::ACT_DO_JUMP_DOWN,    // ACT_IE_INFO_JUMP_DOWN
    dLytDobutton_c::ACT_DO_STRUM,        // ACT_IE_INFO_STRUM
    dLytDobutton_c::ACT_DO_70,           // ACT_IE_INFO_70
    dLytDobutton_c::ACT_DO_RELEASE,      // ACT_IE_INFO_RELEASE
    dLytDobutton_c::ACT_DO_GRAB_2,       // ACT_IE_INFO_GRAB_2
    dLytDobutton_c::ACT_DO_READY_STANCE, // ACT_IE_INFO_READY_STANCE
    dLytDobutton_c::ACT_DO_REMOVE,       // ACT_IE_INFO_REMOVE
    dLytDobutton_c::ACT_DO_READY_SWORD,  // ACT_IE_INFO_READY_SWORD
    dLytDobutton_c::ACT_DO_THRUST_SWORD, // ACT_IE_INFO_THRUST_SWORD
    dLytDobutton_c::ACT_DO_USE,          // ACT_IE_INFO_USE
    dLytDobutton_c::ACT_DO_SCOOP,        // ACT_IE_INFO_SCOOP
    dLytDobutton_c::ACT_DO_79,           // ACT_IE_INFO_79
    dLytDobutton_c::ACT_DO_80,           // ACT_IE_INFO_80
    dLytDobutton_c::ACT_DO_LOOK,         // ACT_IE_INFO_LOOK
    dLytDobutton_c::ACT_DO_DOWSE,        // ACT_IE_INFO_DOWSE
    dLytDobutton_c::ACT_DO_PILOT,        // ACT_IE_INFO_PILOT
    dLytDobutton_c::ACT_DO_LOOK_DOWN,    // ACT_IE_INFO_LOOK_DOWN
    dLytDobutton_c::ACT_DO_DIVE,         // ACT_IE_INFO_DIVE
    dLytDobutton_c::ACT_DO_SELECT,       // ACT_IE_INFO_SELECT
    dLytDobutton_c::ACT_DO_GRAB_3,       // ACT_IE_INFO_GRAB_3
    dLytDobutton_c::ACT_DO_MOVE_2,       // ACT_IE_INFO_MOVE_1
    dLytDobutton_c::ACT_DO_ACCELERATE,   // ACT_IE_INFO_ACCELERATE
    dLytDobutton_c::ACT_DO_CHARGE,       // ACT_IE_INFO_CHARGE
    dLytDobutton_c::ACT_DO_LEAN,         // ACT_IE_INFO_LEAN
    dLytDobutton_c::ACT_DO_PRESS,        // ACT_IE_INFO_PRESS
};

void LytDoButtonRelated::setCrossTop(Act_IE_e act, bool bHas) {
    LytDoButtonRelated *p = sInstance;
    if (p != nullptr) {
        if (act == ACT_IE_147) {
            p->field_0x04[DO_BUTTON_CROSS_T] = ACT_IE_NONE;
            p->field_0x34[DO_BUTTON_CROSS_T] = bHas;
            if (!checkIsInSkykeepPuzzle()) {
                dLytMeter_c::setCrossIconTop(dLytMeterCrossBtn_c::CROSS_ICON_TOP_1);
            } else {
                dLytMeter_c::setCrossIconTop(dLytMeterCrossBtn_c::CROSS_ICON_NONE);
            }
        } else if (act == ACT_IE_149) {
            p->field_0x04[DO_BUTTON_CROSS_T] = ACT_IE_NONE;
            p->field_0x34[DO_BUTTON_CROSS_T] = bHas;
            if (!checkIsInSkykeepPuzzle()) {
                dLytMeter_c::setCrossIconTop(dLytMeterCrossBtn_c::CROSS_ICON_TOP_3);
            } else {
                dLytMeter_c::setCrossIconTop(dLytMeterCrossBtn_c::CROSS_ICON_NONE);
            }
        } else if (act == ACT_IE_151) {
            p->field_0x04[DO_BUTTON_CROSS_T] = ACT_IE_NONE;
            p->field_0x34[DO_BUTTON_CROSS_T] = bHas;
            if (!checkIsInSkykeepPuzzle()) {
                dLytMeter_c::setCrossIconTop(dLytMeterCrossBtn_c::CROSS_ICON_TOP_4);
            } else {
                dLytMeter_c::setCrossIconTop(dLytMeterCrossBtn_c::CROSS_ICON_NONE);
            }
        } else {
            p->field_0x04[DO_BUTTON_CROSS_T] = act;
            p->field_0x34[DO_BUTTON_CROSS_T] = bHas;
            dLytMeter_c::setCrossIconTop(dLytMeterCrossBtn_c::CROSS_ICON_NONE);
        }
    }
}

void LytDoButtonRelated::setCrossDown(Act_IE_e act, bool bHas) {
    LytDoButtonRelated *p = sInstance;
    if (p != nullptr) {
        if (act == ACT_IE_148) {
            p->field_0x04[DO_BUTTON_CROSS_D] = ACT_IE_NONE;
            p->field_0x34[DO_BUTTON_CROSS_D] = bHas;
            if (!checkIsInSkykeepPuzzle()) {
                dLytMeter_c::setCrossIconDown(dLytMeterCrossBtn_c::CROSS_ICON_DOWN_0);
            } else {
                dLytMeter_c::setCrossIconDown(dLytMeterCrossBtn_c::CROSS_ICON_NONE);
            }
        } else if (act == ACT_IE_150) {
            p->field_0x04[DO_BUTTON_CROSS_D] = ACT_IE_NONE;
            p->field_0x34[DO_BUTTON_CROSS_D] = bHas;
            if (!checkIsInSkykeepPuzzle()) {
                dLytMeter_c::setCrossIconDown(dLytMeterCrossBtn_c::CROSS_ICON_DOWN_2);
            } else {
                dLytMeter_c::setCrossIconDown(dLytMeterCrossBtn_c::CROSS_ICON_NONE);
            }
        } else if (act == ACT_IE_ETC_CENTER) {
            p->field_0x04[DO_BUTTON_CROSS_D] = ACT_IE_NONE;
            p->field_0x34[DO_BUTTON_CROSS_D] = bHas;
            dLytMeter_c::setCrossIconDown(dLytMeterCrossBtn_c::CROSS_ICON_DOWN_5);
        } else {
            p->field_0x04[DO_BUTTON_CROSS_D] = act;
            p->field_0x34[DO_BUTTON_CROSS_D] = bHas;
            dLytMeter_c::setCrossIconDown(dLytMeterCrossBtn_c::CROSS_ICON_NONE);
        }
    }
}

LytDoButtonRelated::Act_IE_e LytDoButtonRelated::convertDoButton(dLytDobutton_c::Act_Do_e act) {
    for (s32 i = 0; i < ACT_IE_NONE; i++) {
        if (sActIEtoDoConversionTable[i] == act) {
            return static_cast<Act_IE_e>(i);
        }
    }
    return (Act_IE_e)-1;
}
