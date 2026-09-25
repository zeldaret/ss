#ifndef D_A_BIRD_H
#define D_A_BIRD_H

#include "d/a/obj/d_a_obj_base.h"
#include "m/m_vec.h"
#include "s/s_State.hpp"

class dBird_c : public dAcObjBase_c {
public:
    dBird_c() : mStateMgr(*this) {}
    virtual ~dBird_c() {}

    STATE_FUNC_DECLARE(dBird_c, Wait);
    STATE_FUNC_DECLARE(dBird_c, PreMove);
    STATE_FUNC_DECLARE(dBird_c, Move);
    STATE_FUNC_DECLARE(dBird_c, Hide);
    STATE_FUNC_DECLARE(dBird_c, RideDemo);
    STATE_FUNC_DECLARE(dBird_c, FirstRideDemo);
    STATE_FUNC_DECLARE(dBird_c, OutAreaDemo);
    STATE_FUNC_DECLARE(dBird_c, RideOff);
    STATE_FUNC_DECLARE(dBird_c, OnGround);
    STATE_FUNC_DECLARE(dBird_c, Tornado);
    STATE_FUNC_DECLARE(dBird_c, SaveStartDemo);
    STATE_FUNC_DECLARE(dBird_c, MiniGameWait);
    STATE_FUNC_DECLARE(dBird_c, MiniGameMove);
    STATE_FUNC_DECLARE(dBird_c, MiniGameEnd);
    STATE_FUNC_DECLARE(dBird_c, MiniGameLevelUpDemo);

    // fn_129_8DD0, only referenced by d_a_obj_bird_sp
    void accelerateTowards(mVec3_c &targetDirection);

    void fn_129_C4A0(mVec3_c &out, u16);
    bool fn_129_CFB0();

    s16 getNumDashes() const {
        return mNumDashes;
    }
    const mVec3_c &getField_0xFAC() const {
        return field_0xFAC;
    }
    s16 getField_0xFCC() const {
        return field_0xFCC;
    }

    static dBird_c *getInstance() {
        return spInstance;
    }

    dAcObjRef_c &getRef_0xEB8() {
        return field_0xEB8;
    }

private:
    static dBird_c *spInstance;
    /* 0x??? */ STATE_MGR_DECLARE(dBird_c);
    u8 _0x36C[0xEB8 - 0x36C];
    /* 0xEB8 */ dAcObjRef_c field_0xEB8;
    u8 _0xEC4[0xF8E - 0xEC4];
    /* 0xF8E */ s16 mNumDashes;
    /* 0xFB0 */ u8 _0xFB90[0xFAC - 0xF90];
    /* 0xFAC */ mVec3_c field_0xFAC;
    /* 0xFB8 */ u8 _0xFB8[0xFCC - 0xFB8];
    /* 0xFCC */ s16 field_0xFCC;
};

#endif
