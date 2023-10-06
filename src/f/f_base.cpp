#include "f/f_base.h"

/*
.text File Range:
    [802e12f0 - 802e2680]
*/

// This file will be ported from https://github.com/NSMBW-Community/NSMBW-Decomp/blob/master/source/dol/framework/f_base.cpp

fBaseID_e fBase_c::m_rootUniqueID = FIRST_ID;
ProfileName fBase_c::m_tmpCtProfName;
u32 fBase_c::m_tmpCtParam;
u8 fBase_c::m_tmpCtGroupType;
fTrNdBa_c *fBase_c::m_tmpCtConnectParent;

//   0x80575bb0: sLoadAsyncCallback__7fBase_c
//   0x80575bb4: sUnloadCallback__7fBase_c
int (*fBase_c::sLoadAsyncCallback)();
void (*fBase_c::sUnloadCallback)();

/* 802e12f0 */
fBase_c::fBase_c() : manager(this) {}

/* 802e1480 */
fBase_c::~fBase_c() {}

/* 802e1500 */
int fBase_c::commonPack(int (fBase_c::*doFunc)(), int (fBase_c::*preFunc)(), void (fBase_c::*postFunc)(MAIN_STATE_e)) {}

/* 802e15c0 */
int fBase_c::create() {
    return SUCCEEDED;
}

/* 802e15d0 */
int fBase_c::preCreate() {}

/* 802e15e0 */
void fBase_c::postCreate(MAIN_STATE_e state) {}

/* 802e1670 */
int fBase_c::doDelete() {}

/* 802e1680 */
int fBase_c::createPack() {}

/* 802e1730 */
int fBase_c::preDelete() {}

/* 802e17a0 */
void fBase_c::postDelete(MAIN_STATE_e state) {}

/* 802e15d0 */
int fBase_c::deletePack() {}

/* 802e1910 */
int fBase_c::execute() {}

/* 802e1920 */
int fBase_c::preExecute() {}

/* 802e1950 */
void fBase_c::postExecute(MAIN_STATE_e state) {}

/* 802e1960 */
int fBase_c::executePack() {}

/* 802e1a90 */
int fBase_c::draw() {}

/* 802e1aa0 */
int fBase_c::preDraw() {}

/* 802e1ad0 */
void fBase_c::postDraw(MAIN_STATE_e state) {}

/* 802e1ae0 */
int fBase_c::drawPack() {}

/* 802e1b90 */
void fBase_c::deleteReady() {}

/* 802e1ba0 */
int fBase_c::connectProc() {}

/* 802e1e00 */
void fBase_c::deleteRequest() {}

/* 802e1e80 */
// addNewActorToDrawList

/* 802e1f10 */
// getFirstActor

/* 802e1f30 */
// getNextActor

/* 802e1f90 */
fBase_c* fBase_c::getConnectParent() const {}

/* 802e1fb0 */
fBase_c* fBase_c::getConnectChild() const {}

/* 802e1fd0 */
fBase_c* fBase_c::getConnectBrNext() const {}

/* 802e1ff0 */
// FUN_802e1ff0

/* 802e2090 */
bool fBase_c::setConnectChild(fBase_c* child) {}

/* 802e20e0 */
bool fBase_c::entryFrmHeap(unsigned long size, EGG::Heap *parentHeap) {}

/* 802e22e0 */
bool fBase_c::entryFrmHeapNonAdjust(unsigned long size, EGG::Heap *parentHeap) {}

/* 802e23a0 */
bool fBase_c::createHeap() {}

/* 802e23b0 */
void* fBase_c::operator new(size_t) {}

/* 802e2410 */
void fBase_c::operator delete(void*) {}

/* 802e2420 */
void fBase_c::runCreate() {}

/* 802e24a0 */
fBase_c* fBase_c::getChildProcessCreateState() const {}

/* 802e2510 */
bool fBase_c::checkChildProcessCreateState() const {}

/* 802e2540 */
void fBase_c::setTmpCtData(ProfileName profName, fTrNdBa_c *connectParent, unsigned long param, u8 groupType) {}

/* 802e2560 */
fBase_c* fBase_c::fBase_make(ProfileName profName, fTrNdBa_c *connectParent, unsigned long param, u8 groupType) {}

/* 802e2600 */
fBase_c* fBase_c::createChild(ProfileName profName, fBase_c *parent, unsigned long param, u8 groupType) {}

/* 802e2640 */
fBase_c* fBase_c::createRoot(ProfileName profName, unsigned long param, u8 groupType) {}
