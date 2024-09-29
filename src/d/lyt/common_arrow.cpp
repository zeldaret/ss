#include <d/lyt/common_arrow.h>
#include <toBeSorted/arc_managers/layout_arc_manager.h>

STATE_DEFINE(dLytCommonArrow_c, None);
STATE_DEFINE(dLytCommonArrow_c, In);
STATE_DEFINE(dLytCommonArrow_c, Wait);
STATE_DEFINE(dLytCommonArrow_c, Out);

f32 arrow_fs[0x20];

static const d2d::LytBrlanMapping brlanMap[] = {
        {"commonArrow_00_in.brlan", "G_inOut_00"},
        {"commonArrow_00_loop.brlan", "G_loop_00"},
        {"commonArrow_00_type.brlan", "G_type_00"},
        {"commonArrow_00_onOff.brlan", "G_arrowL_00"},
        {"commonArrow_00_onOff.brlan", "G_arrowR_00"},
        {"commonArrow_00_decide.brlan", "G_arrowL_00"},
        {"commonArrow_00_decide.brlan", "G_arrowR_00"},
        {"commonArrow_00_input.brlan", "G_input_00"},
        {"commonArrow_00_out.brlan", "G_inOut_00"},
};

dLytCommonArrow_c::dLytCommonArrow_c() : mStateMgr(*this, sStateID::null) {}

extern "C" d2d::dLytStructD *lbl_80575260;

bool dLytCommonArrow_c::init() {
    void *data = LayoutArcManager::sInstance->getLoadedData("CommonArrow");
    mResAcc.attach(data, "");
    mLytBase.build("commonArrow_00.brlyt", &mResAcc);
    mLytBase.mPriority = 0x86;

    for (int i = 0; i < 9; i++) {
        field_0x440[i].init(brlanMap[i].mFile, &mResAcc, mLytBase.getLayout(), brlanMap[i].mName);
    }
    field_0x680.fn_80065E70(mLytBase.getLayout()->GetRootPane(), 1, 0, 0);
    lbl_80575260->append(&field_0x680);
    field_0x6A8 = mLytBase.findBounding("B_arrowL_00");
    field_0x6AC = mLytBase.findBounding("B_arrowR_00");
    mStateMgr.changeState(StateID_None);
    setState(0);
    return true;
}

bool dLytCommonArrow_c::fn_80168490() {
    lbl_80575260->detach(&field_0x680);
    mLytBase.unbindAnims();
    for (int i = 0; i < 9; i++) {
        field_0x440[i].fn_800AC860();
    }
    return true;
}

bool dLytCommonArrow_c::fn_80168500() {
    mStateMgr.executeState();
    if (!mStateMgr.getStateID()->isEqual(StateID_None)) {
        field_0x440[1].mpFrameCtrl->play();
        field_0x440[1].syncAnmFrame();
    }
    mLytBase.fn_Multi_c_0x14();
    field_0x680.field_0x22 = 0;
    field_0x680.fn_80065F70();
    field_0x6BC = field_0x6B8;
    return true;
}

bool dLytCommonArrow_c::addToDrawList() {
    if (field_0x6CB == 1) {
        mLytBase.addToDrawList();
    }
    return true;
}

void dLytCommonArrow_c::setState(s32 state) {
    field_0x6B0 = state;
    if (state == 0) {
        mLytBase.mPriority = 0x86;
    } else {
        mLytBase.mPriority = 0x80;
    }
}

void dLytCommonArrow_c::fn_80168640(f32 *arg) {
    // TODO
    mLytBase.getLayout()->GetRootPane()->SetTranslate(nw4r::math::VEC3(arg[0], arg[1], 0.0f));
}

bool dLytCommonArrow_c::fn_80168680() {
    if (!mStateMgr.getStateID()->isEqual(StateID_None)) {
        return false;
    }
    field_0x6C8 = 1;
    return true;
}

bool dLytCommonArrow_c::fn_801686F0() {
    if (!mStateMgr.getStateID()->isEqual(StateID_Wait)) {
        return false;
    }
    field_0x6C9 = 1;
    return true;
}

bool dLytCommonArrow_c::fn_80168760() {
    if (field_0x6C0 == 2 && field_0x6B8 != 2) {
        field_0x6C0 = field_0x6B8;
        return true;
    }
    return false;
}

void dLytCommonArrow_c::fn_80168790(s32 idx, f32 frame) {
    d2d::AnmGroup_c *s = &field_0x440[idx];
    s->setFrame(frame);
}

void dLytCommonArrow_c::fn_80168800(s32 idx) {
    field_0x440[idx].fn_800AC7D0();
}

void dLytCommonArrow_c::fn_80168810(d2d::AnmGroup_c *ctrl) {
    if (ctrl->mpFrameCtrl->getFrame()) {
        f32 newFrame = ctrl->mpFrameCtrl->getFrame() - 1.0f;
        if (newFrame <= 0.0f) {
            newFrame = 0.0f;
        }
        ctrl->mpFrameCtrl->setFrame(newFrame);
        ctrl->syncAnmFrame();
    }
}

void dLytCommonArrow_c::fn_80168880() {}

void dLytCommonArrow_c::initializeState_None() {
    mLytBase.unbindAnims();
    field_0x6C8 = 0;
    field_0x6C9 = 0;
    field_0x6CA = 0;
    field_0x6CB = 0;
    field_0x6B4 = 2;
    field_0x6B8 = 2;
    field_0x6BC = 2;
    field_0x6C0 = 2;
    field_0x6C4 = 0;
    fn_80168790(0, 0.0f);
    field_0x6CC = 1;
}
void dLytCommonArrow_c::executeState_None() {
    if (field_0x6C8 == 1) {
        mStateMgr.changeState(StateID_In);
    }
}
void dLytCommonArrow_c::finalizeState_None() {
    field_0x6C8 = 0;
    fn_80168790(1, 0.0f);
}

void dLytCommonArrow_c::initializeState_In() {
    field_0x6C4 = 0;
    field_0x6CB = 1;
    f32 tmp = 0.0f;
    if (field_0x6B0 == 1) {
        tmp = 1.0f;
    }
    fn_80168790(2, tmp);
    fn_80168790(3, 0.0f);
    fn_80168790(4, 0.0f);
    fn_80168790(7, 0.0f);
}
void dLytCommonArrow_c::executeState_In() {
    switch (field_0x6C4) {
    case 0:
        d2d::AnmGroup_c *s = &field_0x440[0];
        if (s->mpFrameCtrl->isEndReached() == 1) {
            field_0x6C4 += 1;
            field_0x6CA = 1;
        } else {
            s->mpFrameCtrl->play();
            s->syncAnmFrame();
        }
        break;
    case 1:
        field_0x6CA = 0;
        mStateMgr.changeState(StateID_Wait);
        break;
    }
}
void dLytCommonArrow_c::finalizeState_In() {
    fn_80168800(0);
}

void dLytCommonArrow_c::initializeState_Wait() {
    field_0x6C4 = 0;
}
void dLytCommonArrow_c::executeState_Wait() {
    if (field_0x6C9 == 1) {
        mStateMgr.changeState(StateID_Out);
        return;
    }

    fn_80168880();
    if (field_0x6B8 == 0) {
        field_0x440[3].play();
        fn_80168810(&field_0x440[4]);
    } else if (field_0x6B8 == 1) {
        field_0x440[4].play();
        fn_80168810(&field_0x440[3]);
    } else {
        fn_80168810(&field_0x440[3]);
        fn_80168810(&field_0x440[4]);
    }
}
void dLytCommonArrow_c::finalizeState_Wait() {}

void dLytCommonArrow_c::initializeState_Out() {
    field_0x6C4 = 0;
    fn_80168790(8, 0.0f);
    field_0x6C9 = 0;
}
void dLytCommonArrow_c::executeState_Out() {
    switch (field_0x6C4) {
    case 0:
        d2d::AnmGroup_c *s = &field_0x440[8];
        if (s->mpFrameCtrl->isEndReached() == 1) {
            field_0x6C4 = 1;
            field_0x6CA = 1;
        }
        s->play();
        break;
    case 1:
        mStateMgr.changeState(StateID_None);
        break;
    }
}
void dLytCommonArrow_c::finalizeState_Out() {}
