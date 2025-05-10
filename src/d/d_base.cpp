#include "d/d_base.h"

// .sbss
/* 805750c0 */ u32 dBase_c::s_ExecuteControlFlags;
/* 805750c0 */ u32 dBase_c::s_DrawControlFlags;
/* 805750c0 */ u32 dBase_c::s_NextExecuteControlFlags;

dBase_c::dBase_c() : fBase_c() {
    baseProperties = (*fProfile::sProfileList)[profile_name]->mBaseProperties;
}

void dBase_c::postDraw(fBase_c::MAIN_STATE_e status) {
    if (status != CANCELED) {
        baseProperties |= 0x100;
    } else {
        baseProperties &= ~0x100;
    }
    fBase_c::postDraw(status);
}

int dBase_c::preExecute() {
    if (fBase_c::preExecute() == 0) {
        return NOT_READY;
    }
    if (s_ExecuteControlFlags && !isBasePropertyFlag(s_ExecuteControlFlags)) {
        return NOT_READY;
    }
    return SUCCEEDED;
}

void dBase_c::postExecute(fBase_c::MAIN_STATE_e status) {
    if (status != CANCELED) {
        baseProperties |= 0x4;
    } else {
        baseProperties &= ~0x4;
    }
    fBase_c::postExecute(status);
}

int dBase_c::preDraw() {
    if (fBase_c::preDraw() == NOT_READY) {
        return NOT_READY;
    }
    if (s_DrawControlFlags && !isBasePropertyFlag(s_DrawControlFlags)) {
        return NOT_READY;
    }
    return SUCCEEDED;
}

void dBase_c::resetFlags() {
    s_ExecuteControlFlags = 0;
    s_DrawControlFlags = 0;
    s_NextExecuteControlFlags = 0;
}

bool dBase_c::isActorPlayer() {
    return profile_name == fProfile::PLAYER;
}

int dBase_c::loadAsyncCallback() {
    return 2;
}

void dBase_c::unloadCallback() {
    return;
}

void dBase_c::initLoader() {
    fBase_c::sLoadAsyncCallback = loadAsyncCallback;
    fBase_c::sUnloadCallback = unloadCallback;
}

dBase_c *dBase_c::createBase(ProfileName profName, dBase_c *parent, u32 param, u8 grouptype) {
    return static_cast<dBase_c *>(fBase_c::createChild(profName, parent, param, grouptype));
}

dBase_c *dBase_c::createRoot(ProfileName profName, u32 param, u8 groupType) {
    return static_cast<dBase_c *>(fBase_c::createRoot(profName, param, groupType));
}

// dBase_c::~dBase_c() {}
