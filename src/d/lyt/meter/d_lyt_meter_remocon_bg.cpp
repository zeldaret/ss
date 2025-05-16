#include "d/lyt/meter/d_lyt_meter_remocon_bg.h"

#include "d/lyt/d_lyt_do_button.h"
#include "d/lyt/d_lyt_unknowns.h"
#include "d/lyt/meter/d_lyt_meter.h"
#include "toBeSorted/event_manager.h"

STATE_DEFINE(dLytMeterRemoconBg_c, Wait);
STATE_DEFINE(dLytMeterRemoconBg_c, On);
STATE_DEFINE(dLytMeterRemoconBg_c, Active);
STATE_DEFINE(dLytMeterRemoconBg_c, Off);

// Incredible use of the state manager here
void dLytMeterRemoconBg_c::initializeState_Wait() {}
void dLytMeterRemoconBg_c::executeState_Wait() {}
void dLytMeterRemoconBg_c::finalizeState_Wait() {}

void dLytMeterRemoconBg_c::initializeState_On() {}
void dLytMeterRemoconBg_c::executeState_On() {}
void dLytMeterRemoconBg_c::finalizeState_On() {}

void dLytMeterRemoconBg_c::initializeState_Active() {}
void dLytMeterRemoconBg_c::executeState_Active() {}
void dLytMeterRemoconBg_c::finalizeState_Active() {}

void dLytMeterRemoconBg_c::initializeState_Off() {}
void dLytMeterRemoconBg_c::executeState_Off() {}
void dLytMeterRemoconBg_c::finalizeState_Off() {}

bool dLytMeterRemoconBg_c::build(d2d::ResAccIf_c *resAcc) {
    mLyt.setResAcc(resAcc);
    mLyt.build("remoConBg_00.brlyt", nullptr);
    static const char *sPanes[] = {"N_remoConBg_00"};
    for (int i = 0; i < 1; i++) {
        mpPane[i] = mLyt.findPane(sPanes[i]);
    }
    field_0xE8 = 0x61;
    field_0xDC = 0x61;
    field_0xE0 = 0x61;
    field_0xEC = false;
    field_0xE4 = 1;
    mStateMgr.changeState(StateID_Wait);
    return true;
}

bool dLytMeterRemoconBg_c::remove() {
    return true;
}

bool dLytMeterRemoconBg_c::execute() {
    if (EventManager::isInEvent() || dLytMeter_c::getItemSelect0x75A2() != 0) {
        field_0xEC = 0;
        LytDoButtonRelated::set(LytDoButtonRelated::DO_BUTTON_REMOCON_BG, LytDoButtonRelated::ACT_IE_NONE);
    } else {
        field_0xEC = 1;
    }
    s32 d = dLytDobutton_c::getFn0x8010E5E0();
    if (d == 1 || d == 2 || d == 3 || d == 4 || d == 12 || d == 13 || d == 14 || d == 6) {
        LytDoButtonRelated::Act_IE_e a = LytDoButtonRelated::convertDoButton(dLytDobutton_c::getAction());
        if (a >= 0) {
            LytDoButtonRelated::set(LytDoButtonRelated::DO_BUTTON_REMOCON_BG, a);
        }
    }

    field_0xE0 = LytDoButtonRelated::get(LytDoButtonRelated::DO_BUTTON_REMOCON_BG);
    field_0xE4 = LytDoButtonRelated::getHas(LytDoButtonRelated::DO_BUTTON_REMOCON_BG);
    mStateMgr.executeState();
    LytDoButtonRelated::set(LytDoButtonRelated::DO_BUTTON_REMOCON_BG, LytDoButtonRelated::ACT_IE_NONE);
    return true;
}
