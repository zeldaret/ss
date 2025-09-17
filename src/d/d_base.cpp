#include "d/d_base.h"

u32 dBase_c::s_ExecuteControlFlags;
u32 dBase_c::s_DrawControlFlags;
u32 dBase_c::s_NextExecuteControlFlags;

dBase_c::dBase_c() : fBase_c() {
    mBaseProperties = (*fProfile::sProfileList)[profile_name]->mBaseProperties;
}

void dBase_c::postDraw(fBase_c::MAIN_STATE_e status) {
    if (status != CANCELED) {
        setBaseProperty(BASE_PROP_0x100);
    } else {
        unsetBaseProperty(BASE_PROP_0x100);
    }
    fBase_c::postDraw(status);
}

int dBase_c::preExecute() {
    if (NOT_READY == fBase_c::preExecute()) {
        return NOT_READY;
    }
    if (s_ExecuteControlFlags && !checkBaseProperty(s_ExecuteControlFlags)) {
        return NOT_READY;
    }
    return SUCCEEDED;
}

void dBase_c::postExecute(fBase_c::MAIN_STATE_e status) {
    if (status != CANCELED) {
        setBaseProperty(BASE_PROP_0x4);
    } else {
        unsetBaseProperty(BASE_PROP_0x4);
    }
    fBase_c::postExecute(status);
}

int dBase_c::preDraw() {
    if (fBase_c::preDraw() == NOT_READY) {
        return NOT_READY;
    }
    if (s_DrawControlFlags && !checkBaseProperty(s_DrawControlFlags)) {
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
