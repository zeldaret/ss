#ifndef D_A_EN_BASE_H
#define D_A_EN_BASE_H

#include "d/a/obj/d_a_obj_base.h"
#include "d/a/obj/d_a_obj_bomb.h"

struct dAcEnData {
    // Unknown Size yet
};

// Ghidra: ActorEnemyBase
//   size:
// non-official name
class dAcEnBase_c : public dAcObjBase_c {
public:
    dAcEnBase_c();
    virtual ~dAcEnBase_c();
    /* 0x330 */ dAcRef_c<dAcEnBase_c> mEnemyLink;
    /* 0x33C */ u32 mInteractionFlags;
    /* 0x340 */ u16 field_0x338;
    /* 0x342 */ u16 mHealth;
    /* 0x344 */ u8 _344[0x374 - 0x344];
    /* 0x374 */ dAcEnData *mpEnemyData;

    bool checkInteractionFlags(u32 mask) {
        return mInteractionFlags & mask;
    }
    void setInteractionFlags(u32 flag) {
        mInteractionFlags |= flag;
    }
    void resetInteractionFlags(u32 flag) {
        mInteractionFlags &= ~flag;
    }

    dAcBomb_c *getBombWithinRadius(f32 radius); // fn_8002f700

    // Deals with dealing damage to Enemy
    // Returns a value 0-13 (similar to mStts rank?)
    int fn_8002fde0(cCcD_Obj &mCc, u16 *pTgOut);
    void fn_800306d0();
    void fn_80030700();
    void fn_80030c20(u32 flags, f32, f32, f32, f32);

protected:
    // TODO: Make accessors for this list to auto-convert to dAcEnBase?
    static fLiMgBa_c sEnemyList;
};

#endif
