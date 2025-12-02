#ifndef D_T_INSECT_H
#define D_T_INSECT_H

#include "d/a/d_a_base.h"
#include "m/m_vec.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

class dTgInsect_c : public dAcBase_c {
public:
    dTgInsect_c() : mStateMgr(*this, sStateID::null), unk26C(0), unk270(0), unk274(0) {}
    virtual ~dTgInsect_c() {}

    STATE_FUNC_DECLARE(dTgInsect_c, Wait);
    STATE_FUNC_DECLARE(dTgInsect_c, WaitCreate);
    STATE_FUNC_DECLARE(dTgInsect_c, WaitForceEscape);
    STATE_FUNC_DECLARE(dTgInsect_c, End);

private:
    /* 0x0FC */ STATE_MGR_DECLARE(dTgInsect_c);
    /* 0x138 */ dAcRef_c<dAcBase_c> mLinks[16];
    /* 0x1F8 */ u8 unk1F8[0x208-0x1F8];
    /* 0x208 */ s32 unk208[16];
    /* 0x248 */ s32 mInsectCount;
    /* 0x24C */ u8 unk24C;
    /* 0x24D */ u8 unk24D;
    /* 0x24E */ u8 unk24E;
    /* 0x24F */ u8 unk24F;
    /* 0x250 */ u8 unk250[16];
    /* 0x260 */ u8 unk260[0x26C-0x260];
    /* 0x26C */ s32 unk26C;
    /* 0x270 */ s32 unk270;
    /* 0x274 */ s32 unk274;

    s32 getSpawnSubtype() const {
        return (mParams >> 8 & 0xF);
    }
    s32 getSubtype() const {
        return (mParams >> 4 & 0xF);
    }
    void spawnInsect(s32 index);
    bool shouldSpawn();
    bool someGroundCheck(const mVec3_c &pos,s32 param_3);
};

#endif
