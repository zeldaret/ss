#include "d/lyt/d_lyt_control_game.h"

#include "common.h"
#include "f/f_profile_name.h"

SPECIAL_BASE_PROFILE(LYT_CONTROL_GAME, dLytControlGame_c, fProfile::LYT_CONTROL_GAME, 0x9, 96);

dLytControlGame_c::dLytControlGame_c() : mStateMgr(*this), field_0x15C24(0), field_0x15C28(0) {
    sInstance = this;
}

dLytControlGame_c::~dLytControlGame_c() {
    sInstance = nullptr;
}
