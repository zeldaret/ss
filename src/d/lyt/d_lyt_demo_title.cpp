#include "d/lyt/d_lyt_demo_title.h"

#include "d/d_fader.h"
#include "d/d_stage.h"
#include "d/d_stage_mgr.h"
#include "d/d_sys.h"
#include "d/lyt/d_lyt_deposit.h"
#include "toBeSorted/arc_managers/layout_arc_manager.h"

extern "C" u32 lbl_805B7120;

#define TITLE_DEMO_MAIN_ANIM_IN 0
#define TITLE_DEMO_MAIN_ANIM_OUT 1

STATE_DEFINE(dLytDemoTitleMain_c, ModeNone);
STATE_DEFINE(dLytDemoTitleMain_c, ModeIn);
STATE_DEFINE(dLytDemoTitleMain_c, ModeMove);
STATE_DEFINE(dLytDemoTitleMain_c, ModeOut);
STATE_DEFINE(dLytDemoTitleMain_c, ModeEnd);

STATE_DEFINE(dLytDemoTitle_c, None);
STATE_DEFINE(dLytDemoTitle_c, In);
STATE_DEFINE(dLytDemoTitle_c, Move);
STATE_DEFINE(dLytDemoTitle_c, Out);
STATE_DEFINE(dLytDemoTitle_c, End);

void dLytDemoTitleMain_c::initializeState_ModeNone() {
    return;
}

void dLytDemoTitleMain_c::executeState_ModeNone() {
    mIsAnimating = true;
    return;
}

void dLytDemoTitleMain_c::finalizeState_ModeNone() {
    return;
}

void dLytDemoTitleMain_c::initializeState_ModeIn() {
    return;
}

void dLytDemoTitleMain_c::executeState_ModeIn() {
    if (mAnmGroups[TITLE_DEMO_MAIN_ANIM_IN].isEndReached()) {
        mAnmGroups[TITLE_DEMO_MAIN_ANIM_IN].setAnimEnable(false);
        mIsAnimating = true;
    }
    return;
}

void dLytDemoTitleMain_c::finalizeState_ModeIn() {
    return;
}

void dLytDemoTitleMain_c::initializeState_ModeMove() {
    return;
}

void dLytDemoTitleMain_c::executeState_ModeMove() {
    mIsAnimating = true;
    return;
}

void dLytDemoTitleMain_c::finalizeState_ModeMove() {
    return;
}

void dLytDemoTitleMain_c::initializeState_ModeOut() {
    return;
}

void dLytDemoTitleMain_c::executeState_ModeOut() {
    if (mAnmGroups[TITLE_DEMO_MAIN_ANIM_OUT].isEndReached()) {
        mAnmGroups[TITLE_DEMO_MAIN_ANIM_OUT].setAnimEnable(false);
        mIsAnimating = true;
    }
    return;
}

void dLytDemoTitleMain_c::finalizeState_ModeOut() {
    return;
}

void dLytDemoTitleMain_c::initializeState_ModeEnd() {
    return;
}

void dLytDemoTitleMain_c::executeState_ModeEnd() {
    mIsAnimating = true;
    return;
}

void dLytDemoTitleMain_c::finalizeState_ModeEnd() {
    return;
}

dLytDemoTitleMain_c::dLytDemoTitleMain_c() : mStateMgr(*this) {}

static const d2d::LytBrlanMapping brlanMap[] = {
    { "demoTitle_00_in.brlan", "G_inOut_00"},
    {"demoTitle_00_out.brlan", "G_inOut_00"},
};

bool dLytDemoTitleMain_c::fn_802B0760(d2d::ResAccIf_c *resAcc) {
    mLytBase.setResAcc(resAcc);
    mLytBase.build("demoTitle_00.brlyt", nullptr);
    mLytBase.setPriority(0x86);

    for (int i = 0; i < 2; i++) {
        mAnmGroups[i].init(brlanMap[i].mFile, resAcc, mLytBase.getLayout(), brlanMap[i].mName);
        mAnmGroups[i].bind(false);
        mAnmGroups[i].setAnimEnable(false);
    }

    mAnmGroups[TITLE_DEMO_MAIN_ANIM_IN].setAnimEnable(true);
    mAnmGroups[TITLE_DEMO_MAIN_ANIM_IN].setFrame(0.0f);

    mLytBase.calc();

    mAnmGroups[TITLE_DEMO_MAIN_ANIM_IN].setAnimEnable(false);

    fn_802B09F0();

    return true;
}

bool dLytDemoTitleMain_c::fn_802B0860() {
    mLytBase.getLayout()->GetRootPane()->SetInfluencedAlpha(true);
    mLytBase.getLayout()->GetRootPane()->SetAlpha(255 - dStage_c::GetInstance()->getFader().getColorFaderAlpha());

    mStateMgr.executeState();

    for (int i = 0; i < 2; i++) {
        if (mAnmGroups[i].isEnabled()) {
            mAnmGroups[i].play();
        }
    }

    mLytBase.calc();

    return true;
}

bool dLytDemoTitleMain_c::fn_802B0950() {
    mLytBase.addToDrawList();
    return true;
}

bool dLytDemoTitleMain_c::fn_802B0980() {
    dStageMgr_c::GetInstance()->unsetFlags0x88a0(0xfffffffe);

    mLytBase.unbindAnims();

    for (int i = 0; i < 2; i++) {
        mAnmGroups[i].remove();
    }
    return true;
}

void dLytDemoTitleMain_c::fn_802B09F0() {
    mStateMgr.changeState(StateID_ModeNone);
    mIsAnimating = false;
    return;
}

void dLytDemoTitleMain_c::fn_802B0A50() {
    dStageMgr_c::GetInstance()->setFlags0x88a0(0x1);

    dSys_c::setClearColor(mColor(0xFF));
    
    mAnmGroups[TITLE_DEMO_MAIN_ANIM_IN].setAnimEnable(true);
    mAnmGroups[TITLE_DEMO_MAIN_ANIM_IN].setFrame(0.0f);
    mLytBase.calc();
    mStateMgr.changeState(StateID_ModeIn);
    mIsAnimating = false;
}

void dLytDemoTitleMain_c::fn_802B0B00(){
    mStateMgr.changeState(StateID_ModeMove);
    mIsAnimating = false;
    return;  
}

void dLytDemoTitleMain_c::fn_802B0B50(){
    mAnmGroups[TITLE_DEMO_MAIN_ANIM_OUT].setAnimEnable(true);
    mAnmGroups[TITLE_DEMO_MAIN_ANIM_OUT].setFrame(0.0f);
    mLytBase.calc();
    mStateMgr.changeState(StateID_ModeOut);
    mIsAnimating = false;
    return; 
}

void dLytDemoTitleMain_c::fn_802B0BD0(){
    mStateMgr.changeState(StateID_ModeEnd);
    mIsAnimating = false;
    return;  
}

void dLytDemoTitleMain_c::fn_802B0C20(){
    return;
}

void dLytDemoTitle_c::fn_802B0C20(){
    if (mIsAnimating) {
        mMain.fn_802B0A50();
        mStateMgr.changeState(StateID_In);
        mIsAnimating=false;
    }   
    return; 
}

void dLytDemoTitle_c::fn_802B0CA0(){
    return;
}

void dLytDemoTitle_c::fn_802B0CB0(){
    return;
}

void dLytDemoTitle_c::fn_802B0CC0(){
    if (mMain.getmIsAnimating()) {
        mMain.fn_802B0B00();
        mStateMgr.changeState(StateID_Move);
    }   
    return; 
}

void dLytDemoTitle_c::fn_802B0D20(){
    return;
}

void dLytDemoTitle_c::fn_802B0D30(){
    return;
}

void dLytDemoTitle_c::fn_802B0D40(){
    if (mMain.getmIsAnimating() && field_0x58d) {
        mMain.fn_802B0B50();
        mStateMgr.changeState(StateID_Out);
        field_0x58d=false;
    }   
    return; 
}

void dLytDemoTitle_c::fn_802B0DB0(){
    return;
}

void dLytDemoTitle_c::fn_802B0DC0(){
    return;
}

void dLytDemoTitle_c::fn_802B0DD0(){

    if (mMain.getmIsAnimating()) {
        deleteRequest();
        mMain.fn_802B0BD0();
        mStateMgr.changeState(StateID_End);
    }
    
    return;
}

void dLytDemoTitle_c::fn_802B0E30(){
    return;
}

void dLytDemoTitle_c::fn_802B0E40(){
    return;
}

void dLytDemoTitle_c::fn_802B0E50(){
    return;
}

void dLytDemoTitle_c::fn_802B0E60(){
    return;
}

dLytDemoTitle_c::dLytDemoTitle_c() : mStateMgr(*this) {}

dLytDemoTitleMain_c::~dLytDemoTitleMain_c() {}

bool dLytDemoTitle_c::fn_802B1270(d2d::ResAccIf_c *resAcc){

    mMain.fn_802B0760(resAcc);
    mStateMgr.changeState(StateID_None);
    mIsAnimating=true;
    field_0x58d=false;
    return true;
}
