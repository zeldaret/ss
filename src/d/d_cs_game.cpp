#include "d/d_cs_game.h"
#include "d/d_cs_base.h"
#include "d/lyt/d_structd.h"
#include "f/f_base.h"
#include "toBeSorted/arc_managers/layout_arc_manager.h"

SPECIAL_BASE_PROFILE(C_GAME, dCsGame_c, fProfile::C_GAME, 0x2BF, 0x06F9);

STATE_DEFINE(dCsGame_c::lytItemCursor_c, Invisible);
STATE_DEFINE(dCsGame_c::lytItemCursor_c, Bow);
STATE_DEFINE(dCsGame_c::lytItemCursor_c, Dowsing);
STATE_DEFINE(dCsGame_c::lytItemCursor_c, Vacuum);
STATE_DEFINE(dCsGame_c::lytItemCursor_c, Pachinko);
STATE_DEFINE(dCsGame_c::lytItemCursor_c, HookShot);
STATE_DEFINE(dCsGame_c::lytItemCursor_c, PlayerCam);

STATE_DEFINE(dCsGame_c::lytItemCursor_c::lytDowsingCsr_c, NotFind);
STATE_DEFINE(dCsGame_c::lytItemCursor_c::lytDowsingCsr_c, ToFind);
STATE_DEFINE(dCsGame_c::lytItemCursor_c::lytDowsingCsr_c, Find);
STATE_DEFINE(dCsGame_c::lytItemCursor_c::lytDowsingCsr_c, FindMinimum);
STATE_DEFINE(dCsGame_c::lytItemCursor_c::lytDowsingCsr_c, ToNotFind);

STATE_DEFINE(dCsGame_c::lytItemCursor_c::lytBowCsr_c, Invisible);
STATE_DEFINE(dCsGame_c::lytItemCursor_c::lytBowCsr_c, Select);
STATE_DEFINE(dCsGame_c::lytItemCursor_c::lytBowCsr_c, Ready);
STATE_DEFINE(dCsGame_c::lytItemCursor_c::lytBowCsr_c, ToAimStart);
STATE_DEFINE(dCsGame_c::lytItemCursor_c::lytBowCsr_c, AimStart);
STATE_DEFINE(dCsGame_c::lytItemCursor_c::lytBowCsr_c, Draw);
STATE_DEFINE(dCsGame_c::lytItemCursor_c::lytBowCsr_c, Charge);

STATE_DEFINE(dCsGame_c::lytItemCursor_c::lytPachinkoCsr_c, Invisible);
STATE_DEFINE(dCsGame_c::lytItemCursor_c::lytPachinkoCsr_c, Select);
STATE_DEFINE(dCsGame_c::lytItemCursor_c::lytPachinkoCsr_c, ToDraw);
STATE_DEFINE(dCsGame_c::lytItemCursor_c::lytPachinkoCsr_c, Draw);
STATE_DEFINE(dCsGame_c::lytItemCursor_c::lytPachinkoCsr_c, Out);
STATE_DEFINE(dCsGame_c::lytItemCursor_c::lytPachinkoCsr_c, Charge);
STATE_DEFINE(dCsGame_c::lytItemCursor_c::lytPachinkoCsr_c, ChargeFull);

STATE_DEFINE(dCsGame_c::lytItemCursor_c::lytCrawShotCsr_c, Normal);
STATE_DEFINE(dCsGame_c::lytItemCursor_c::lytCrawShotCsr_c, ToLock);
STATE_DEFINE(dCsGame_c::lytItemCursor_c::lytCrawShotCsr_c, Lock);
STATE_DEFINE(dCsGame_c::lytItemCursor_c::lytCrawShotCsr_c, ToNormal);

STATE_DEFINE(dCsGame_c::lytItemCursor_c::lytVacuumCsr_c, Normal);
STATE_DEFINE(dCsGame_c::lytItemCursor_c::lytVacuumCsr_c, ToLock);
STATE_DEFINE(dCsGame_c::lytItemCursor_c::lytVacuumCsr_c, Lock);
STATE_DEFINE(dCsGame_c::lytItemCursor_c::lytVacuumCsr_c, ToNormal);

dCsGame_c::dCsGame_c() : mCursorType(0) {
    sInstance = this;
}

dCsGame_c::~dCsGame_c() {
    sInstance = nullptr;
}

int dCsGame_c::create() {
    void *csData = LayoutArcManager::sInstance->getLoadedData("cursor");
    mCursorResAcc.attach(csData, "arc");
    void *mainData = LayoutArcManager::sInstance->getLoadedData("Main2D");
    mMain2DResAcc.attach(mainData, "");

    mLyt1.setResAcc(&mCursorResAcc);
    mLyt1.dCsGameLytBase_0x10();
    mLyt2.setResAcc(&mCursorResAcc);
    mLyt2.dCsGameLytBase_0x10();
    mCursor.setResAcc(&mMain2DResAcc);
    mCursor.init();
    
    setSomething(0);
    mCursorType = 0;

    mCursor.setField0x9A0(0);
    mStructC.field_0x10 = 2;
    d2d::dLytStructDList::sInstance->appendToList1(&mStructC);
    dCsBase_c::sInstance->setField703(false);
    return SUCCEEDED;
}

int dCsGame_c::doDelete() {
    mCursor.remove();
    mLyt2.dCsGameLytBase_0x14();
    mLyt1.dCsGameLytBase_0x14();
    d2d::dLytStructDList::sInstance->removeFromList1(&mStructC);
    mCursorResAcc.detach();
    mMain2DResAcc.detach();
    return SUCCEEDED;
}

void dCsGame_c::lytItemCursor_c::initializeState_Invisible() {}
void dCsGame_c::lytItemCursor_c::executeState_Invisible() {}
void dCsGame_c::lytItemCursor_c::finalizeState_Invisible() {}

void dCsGame_c::lytItemCursor_c::initializeState_Bow() {}
void dCsGame_c::lytItemCursor_c::executeState_Bow() {}
void dCsGame_c::lytItemCursor_c::finalizeState_Bow() {}

void dCsGame_c::lytItemCursor_c::initializeState_Dowsing() {}
void dCsGame_c::lytItemCursor_c::executeState_Dowsing() {}
void dCsGame_c::lytItemCursor_c::finalizeState_Dowsing() {}

void dCsGame_c::lytItemCursor_c::initializeState_Vacuum() {}
void dCsGame_c::lytItemCursor_c::executeState_Vacuum() {}
void dCsGame_c::lytItemCursor_c::finalizeState_Vacuum() {}

void dCsGame_c::lytItemCursor_c::initializeState_Pachinko() {}
void dCsGame_c::lytItemCursor_c::executeState_Pachinko() {}
void dCsGame_c::lytItemCursor_c::finalizeState_Pachinko() {}

void dCsGame_c::lytItemCursor_c::initializeState_HookShot() {}
void dCsGame_c::lytItemCursor_c::executeState_HookShot() {}
void dCsGame_c::lytItemCursor_c::finalizeState_HookShot() {}

void dCsGame_c::lytItemCursor_c::initializeState_PlayerCam() {}
void dCsGame_c::lytItemCursor_c::executeState_PlayerCam() {}
void dCsGame_c::lytItemCursor_c::finalizeState_PlayerCam() {}

void dCsGame_c::lytItemCursor_c::lytDowsingCsr_c::initializeState_NotFind() {}
void dCsGame_c::lytItemCursor_c::lytDowsingCsr_c::executeState_NotFind() {}
void dCsGame_c::lytItemCursor_c::lytDowsingCsr_c::finalizeState_NotFind() {}

void dCsGame_c::lytItemCursor_c::lytDowsingCsr_c::initializeState_ToFind() {}
void dCsGame_c::lytItemCursor_c::lytDowsingCsr_c::executeState_ToFind() {}
void dCsGame_c::lytItemCursor_c::lytDowsingCsr_c::finalizeState_ToFind() {}

void dCsGame_c::lytItemCursor_c::lytDowsingCsr_c::initializeState_Find() {}
void dCsGame_c::lytItemCursor_c::lytDowsingCsr_c::executeState_Find() {}
void dCsGame_c::lytItemCursor_c::lytDowsingCsr_c::finalizeState_Find() {}

void dCsGame_c::lytItemCursor_c::lytDowsingCsr_c::initializeState_FindMinimum() {}
void dCsGame_c::lytItemCursor_c::lytDowsingCsr_c::executeState_FindMinimum() {}
void dCsGame_c::lytItemCursor_c::lytDowsingCsr_c::finalizeState_FindMinimum() {}

void dCsGame_c::lytItemCursor_c::lytDowsingCsr_c::initializeState_ToNotFind() {}
void dCsGame_c::lytItemCursor_c::lytDowsingCsr_c::executeState_ToNotFind() {}
void dCsGame_c::lytItemCursor_c::lytDowsingCsr_c::finalizeState_ToNotFind() {}

void dCsGame_c::lytItemCursor_c::lytBowCsr_c::initializeState_Invisible() {}
void dCsGame_c::lytItemCursor_c::lytBowCsr_c::executeState_Invisible() {}
void dCsGame_c::lytItemCursor_c::lytBowCsr_c::finalizeState_Invisible() {}

void dCsGame_c::lytItemCursor_c::lytBowCsr_c::initializeState_Select() {}
void dCsGame_c::lytItemCursor_c::lytBowCsr_c::executeState_Select() {}
void dCsGame_c::lytItemCursor_c::lytBowCsr_c::finalizeState_Select() {}

void dCsGame_c::lytItemCursor_c::lytBowCsr_c::initializeState_Ready() {}
void dCsGame_c::lytItemCursor_c::lytBowCsr_c::executeState_Ready() {}
void dCsGame_c::lytItemCursor_c::lytBowCsr_c::finalizeState_Ready() {}

void dCsGame_c::lytItemCursor_c::lytBowCsr_c::initializeState_ToAimStart() {}
void dCsGame_c::lytItemCursor_c::lytBowCsr_c::executeState_ToAimStart() {}
void dCsGame_c::lytItemCursor_c::lytBowCsr_c::finalizeState_ToAimStart() {}

void dCsGame_c::lytItemCursor_c::lytBowCsr_c::initializeState_AimStart() {}
void dCsGame_c::lytItemCursor_c::lytBowCsr_c::executeState_AimStart() {}
void dCsGame_c::lytItemCursor_c::lytBowCsr_c::finalizeState_AimStart() {}

void dCsGame_c::lytItemCursor_c::lytBowCsr_c::initializeState_Draw() {}
void dCsGame_c::lytItemCursor_c::lytBowCsr_c::executeState_Draw() {}
void dCsGame_c::lytItemCursor_c::lytBowCsr_c::finalizeState_Draw() {}

void dCsGame_c::lytItemCursor_c::lytBowCsr_c::initializeState_Charge() {}
void dCsGame_c::lytItemCursor_c::lytBowCsr_c::executeState_Charge() {}
void dCsGame_c::lytItemCursor_c::lytBowCsr_c::finalizeState_Charge() {}

void dCsGame_c::lytItemCursor_c::lytPachinkoCsr_c::initializeState_Invisible() {}
void dCsGame_c::lytItemCursor_c::lytPachinkoCsr_c::executeState_Invisible() {}
void dCsGame_c::lytItemCursor_c::lytPachinkoCsr_c::finalizeState_Invisible() {}

void dCsGame_c::lytItemCursor_c::lytPachinkoCsr_c::initializeState_Select() {}
void dCsGame_c::lytItemCursor_c::lytPachinkoCsr_c::executeState_Select() {}
void dCsGame_c::lytItemCursor_c::lytPachinkoCsr_c::finalizeState_Select() {}

void dCsGame_c::lytItemCursor_c::lytPachinkoCsr_c::initializeState_ToDraw() {}
void dCsGame_c::lytItemCursor_c::lytPachinkoCsr_c::executeState_ToDraw() {}
void dCsGame_c::lytItemCursor_c::lytPachinkoCsr_c::finalizeState_ToDraw() {}

void dCsGame_c::lytItemCursor_c::lytPachinkoCsr_c::initializeState_Draw() {}
void dCsGame_c::lytItemCursor_c::lytPachinkoCsr_c::executeState_Draw() {}
void dCsGame_c::lytItemCursor_c::lytPachinkoCsr_c::finalizeState_Draw() {}

void dCsGame_c::lytItemCursor_c::lytPachinkoCsr_c::initializeState_Out() {}
void dCsGame_c::lytItemCursor_c::lytPachinkoCsr_c::executeState_Out() {}
void dCsGame_c::lytItemCursor_c::lytPachinkoCsr_c::finalizeState_Out() {}

void dCsGame_c::lytItemCursor_c::lytPachinkoCsr_c::initializeState_Charge() {}
void dCsGame_c::lytItemCursor_c::lytPachinkoCsr_c::executeState_Charge() {}
void dCsGame_c::lytItemCursor_c::lytPachinkoCsr_c::finalizeState_Charge() {}

void dCsGame_c::lytItemCursor_c::lytPachinkoCsr_c::initializeState_ChargeFull() {}
void dCsGame_c::lytItemCursor_c::lytPachinkoCsr_c::executeState_ChargeFull() {}
void dCsGame_c::lytItemCursor_c::lytPachinkoCsr_c::finalizeState_ChargeFull() {}


void dCsGame_c::lytItemCursor_c::lytCrawShotCsr_c::initializeState_Normal() {}
void dCsGame_c::lytItemCursor_c::lytCrawShotCsr_c::executeState_Normal() {}
void dCsGame_c::lytItemCursor_c::lytCrawShotCsr_c::finalizeState_Normal() {}

void dCsGame_c::lytItemCursor_c::lytCrawShotCsr_c::initializeState_ToLock() {}
void dCsGame_c::lytItemCursor_c::lytCrawShotCsr_c::executeState_ToLock() {}
void dCsGame_c::lytItemCursor_c::lytCrawShotCsr_c::finalizeState_ToLock() {}

void dCsGame_c::lytItemCursor_c::lytCrawShotCsr_c::initializeState_Lock() {}
void dCsGame_c::lytItemCursor_c::lytCrawShotCsr_c::executeState_Lock() {}
void dCsGame_c::lytItemCursor_c::lytCrawShotCsr_c::finalizeState_Lock() {}

void dCsGame_c::lytItemCursor_c::lytCrawShotCsr_c::initializeState_ToNormal() {}
void dCsGame_c::lytItemCursor_c::lytCrawShotCsr_c::executeState_ToNormal() {}
void dCsGame_c::lytItemCursor_c::lytCrawShotCsr_c::finalizeState_ToNormal() {}

void dCsGame_c::lytItemCursor_c::lytVacuumCsr_c::initializeState_Normal() {}
void dCsGame_c::lytItemCursor_c::lytVacuumCsr_c::executeState_Normal() {}
void dCsGame_c::lytItemCursor_c::lytVacuumCsr_c::finalizeState_Normal() {}

void dCsGame_c::lytItemCursor_c::lytVacuumCsr_c::initializeState_ToLock() {}
void dCsGame_c::lytItemCursor_c::lytVacuumCsr_c::executeState_ToLock() {}
void dCsGame_c::lytItemCursor_c::lytVacuumCsr_c::finalizeState_ToLock() {}

void dCsGame_c::lytItemCursor_c::lytVacuumCsr_c::initializeState_Lock() {}
void dCsGame_c::lytItemCursor_c::lytVacuumCsr_c::executeState_Lock() {}
void dCsGame_c::lytItemCursor_c::lytVacuumCsr_c::finalizeState_Lock() {}

void dCsGame_c::lytItemCursor_c::lytVacuumCsr_c::initializeState_ToNormal() {}
void dCsGame_c::lytItemCursor_c::lytVacuumCsr_c::executeState_ToNormal() {}
void dCsGame_c::lytItemCursor_c::lytVacuumCsr_c::finalizeState_ToNormal() {}
