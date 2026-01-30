#include "d/d_title_manager.h"

SPECIAL_BASE_PROFILE(TITLE_MANAGER, dTitleMgr_c, fProfile::TITLE_MANAGER, 0x2AE, 0);

STATE_DEFINE(dTitleMgr_c, Init);
STATE_DEFINE(dTitleMgr_c, OpeningDemo);
STATE_DEFINE(dTitleMgr_c, CheckSave);
STATE_DEFINE(dTitleMgr_c, Title);
STATE_DEFINE(dTitleMgr_c, FileSelect);
STATE_DEFINE(dTitleMgr_c, Input);

dTitleMgr_c::dTitleMgr_c() : mStateMgr(*this) {}
dTitleMgr_c::~dTitleMgr_c() {}

void dTitleMgr_c::initializeState_Init() {}
void dTitleMgr_c::executeState_Init() {}
void dTitleMgr_c::finalizeState_Init() {}
void dTitleMgr_c::initializeState_OpeningDemo() {}
void dTitleMgr_c::executeState_OpeningDemo() {}
void dTitleMgr_c::finalizeState_OpeningDemo() {}
void dTitleMgr_c::initializeState_CheckSave() {}
void dTitleMgr_c::executeState_CheckSave() {}
void dTitleMgr_c::finalizeState_CheckSave() {}
void dTitleMgr_c::initializeState_Title() {}
void dTitleMgr_c::executeState_Title() {}
void dTitleMgr_c::finalizeState_Title() {}
void dTitleMgr_c::initializeState_FileSelect() {}
void dTitleMgr_c::executeState_FileSelect() {}
void dTitleMgr_c::finalizeState_FileSelect() {}
void dTitleMgr_c::initializeState_Input() {}
void dTitleMgr_c::executeState_Input() {}
void dTitleMgr_c::finalizeState_Input() {}
