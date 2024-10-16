#ifndef D_A_E_KG_H
#define D_A_E_KG_H

#include "d/a/e/d_a_en_base.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

class dAcEkg_c : public dAcEnBase_c {
public:
    dAcEkg_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcEkg_c() {}

    STATE_FUNC_DECLARE(dAcEkg_c, Wait);
    STATE_FUNC_DECLARE(dAcEkg_c, Walk);
    STATE_FUNC_DECLARE(dAcEkg_c, Find);
    STATE_FUNC_DECLARE(dAcEkg_c, Chase);
    STATE_FUNC_DECLARE(dAcEkg_c, Step);
    STATE_FUNC_DECLARE(dAcEkg_c, Damage);
    STATE_FUNC_DECLARE(dAcEkg_c, PlayerLost);
    STATE_FUNC_DECLARE(dAcEkg_c, Return);
    STATE_FUNC_DECLARE(dAcEkg_c, WindBlow);
    STATE_FUNC_DECLARE(dAcEkg_c, Reverse);
    STATE_FUNC_DECLARE(dAcEkg_c, BlowReturn);
    STATE_FUNC_DECLARE(dAcEkg_c, JumpReady);
    STATE_FUNC_DECLARE(dAcEkg_c, Jump);
    STATE_FUNC_DECLARE(dAcEkg_c, JumpAttack_Hold);
    STATE_FUNC_DECLARE(dAcEkg_c, Release);
    STATE_FUNC_DECLARE(dAcEkg_c, Landing);
    STATE_FUNC_DECLARE(dAcEkg_c, WallWait);
    STATE_FUNC_DECLARE(dAcEkg_c, WallWalk);
    STATE_FUNC_DECLARE(dAcEkg_c, WallFind);
    STATE_FUNC_DECLARE(dAcEkg_c, WallStep);
    STATE_FUNC_DECLARE(dAcEkg_c, WallJumpReady);
    STATE_FUNC_DECLARE(dAcEkg_c, WallJump);
    STATE_FUNC_DECLARE(dAcEkg_c, WindBlowAtWall);
    STATE_FUNC_DECLARE(dAcEkg_c, HitAtWall);
    STATE_FUNC_DECLARE(dAcEkg_c, DamageAtWall);
    STATE_FUNC_DECLARE(dAcEkg_c, RoofWait);
    STATE_FUNC_DECLARE(dAcEkg_c, RoofWalk);
    STATE_FUNC_DECLARE(dAcEkg_c, RoofFall);
    STATE_FUNC_DECLARE(dAcEkg_c, HangOn);
    STATE_FUNC_DECLARE(dAcEkg_c, UnderWait);
    STATE_FUNC_DECLARE(dAcEkg_c, UnderWalk);
    STATE_FUNC_DECLARE(dAcEkg_c, UnderStep);
    STATE_FUNC_DECLARE(dAcEkg_c, UnderJump);
    STATE_FUNC_DECLARE(dAcEkg_c, SoundReaction);
    STATE_FUNC_DECLARE(dAcEkg_c, SoundReactionAtWallRoof);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcEkg_c);
};

#endif
