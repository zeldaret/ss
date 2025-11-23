#include "d/lyt/d_lyt_draw_mark.h"

#include "d/d_sc_game.h"

STATE_DEFINE(dLytDrawMark_c, Wait);
STATE_DEFINE(dLytDrawMark_c, In);
STATE_DEFINE(dLytDrawMark_c, RingIn);
STATE_DEFINE(dLytDrawMark_c, MoveWait);
STATE_DEFINE(dLytDrawMark_c, MoveDraw);
STATE_DEFINE(dLytDrawMark_c, MoveFix);
STATE_DEFINE(dLytDrawMark_c, MoveEnd);
STATE_DEFINE(dLytDrawMark_c, Complete);

#define DRAW_MARK_ANIM_IN 0
#define DRAW_MARK_ANIM_RING_IN 1
#define DRAW_MARK_ANIM_COMPLETE 15

#define DRAW_MARK_NUM_ANIMS 16

bool dLytDrawMark_c::build(UNKWORD) {
    mVariant = dScGame_c::currentSpawnInfo.layer == 3 ? 1 : dScGame_c::currentSpawnInfo.layer == 4 ? 2 : 0;

    if (mVariant == 1) {
        buildVariant02();
    } else if (mVariant == 2) {
        buildVariant03();
    } else {
        buildVariant01();
    }

    field_0x500 = field_0x4CC;

    field_0x942 = 0;
    field_0x943 = 0;
    field_0x940 = 0;
    field_0x941 = 0;

    field_0x8F4 = -1;
    field_0x8F8 = -1;
    field_0x8FC = 0;

    field_0x944 = 0;

    field_0x900 = 0;
    field_0x904 = 1;
    field_0x908 = 0;
    field_0x8EC = 0;

    field_0x874.x = field_0x874.y = 0.0f;
    field_0x87C.x = field_0x87C.y = 0.0f;

    for (int i = 0; i < 12; i++) {
        field_0x910[i] = 0.0f;
    }

    field_0x955 = 0;
    field_0x956 = 0;
    field_0x957 = 0;

    for (int i = 0; i < 6; i++) {
        field_0x88C[i].x = field_0x88C[i].y = 0.0f;
        field_0x8BC[i].x = field_0x8BC[i].y = 0.0f;
    }

    mAnm[DRAW_MARK_ANIM_IN].setFrame(0.0f);
    mAnm[DRAW_MARK_ANIM_IN].setAnimEnable(true);
    mAnm[DRAW_MARK_ANIM_RING_IN].setFrame(0.0f);
    mAnm[DRAW_MARK_ANIM_RING_IN].setAnimEnable(true);

    mLyt.calc();

    mAnm[DRAW_MARK_ANIM_IN].setAnimEnable(false);
    mAnm[DRAW_MARK_ANIM_RING_IN].setAnimEnable(false);

    mStateMgr.changeState(StateID_In);

    return true;
}

void dLytDrawMark_c::initializeState_Wait() {}
void dLytDrawMark_c::executeState_Wait() {}
void dLytDrawMark_c::finalizeState_Wait() {}

void dLytDrawMark_c::initializeState_In() {}
void dLytDrawMark_c::executeState_In() {}
void dLytDrawMark_c::finalizeState_In() {}

void dLytDrawMark_c::initializeState_RingIn() {}
void dLytDrawMark_c::executeState_RingIn() {}
void dLytDrawMark_c::finalizeState_RingIn() {}

void dLytDrawMark_c::initializeState_MoveWait() {}
void dLytDrawMark_c::executeState_MoveWait() {}
void dLytDrawMark_c::finalizeState_MoveWait() {}

void dLytDrawMark_c::initializeState_MoveDraw() {}
void dLytDrawMark_c::executeState_MoveDraw() {}
void dLytDrawMark_c::finalizeState_MoveDraw() {}

void dLytDrawMark_c::initializeState_MoveFix() {}
void dLytDrawMark_c::executeState_MoveFix() {}
void dLytDrawMark_c::finalizeState_MoveFix() {}

void dLytDrawMark_c::initializeState_MoveEnd() {}
void dLytDrawMark_c::executeState_MoveEnd() {}
void dLytDrawMark_c::finalizeState_MoveEnd() {}

void dLytDrawMark_c::initializeState_Complete() {}
void dLytDrawMark_c::executeState_Complete() {}
void dLytDrawMark_c::finalizeState_Complete() {}
