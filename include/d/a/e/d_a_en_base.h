#ifndef D_A_EN_BASE_H
#define D_A_EN_BASE_H

#include "common.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/a/obj/d_a_obj_bomb.h"
#include "d/d_cc.h"
#include "m/m_angle.h"
#include "m/m_vec.h"

struct dAcEnData {
    // Unknown Size yet
};

// Ghidra: ActorEnemyBase
//   size:
// non-official name
class dAcEnBase_c : public dAcObjBase_c {
public:
    enum InteractionFlags_e {
        INTERACT_0x1 = (1 << 0),
        INTERACT_0x4 = (1 << 2),
        INTERACT_0x40 = (1 << 6),
        INTERACT_0x1000 = (1 << 12),
    };

public:
    dAcEnBase_c();
    virtual ~dAcEnBase_c();

    virtual void *getObjectListEntry() override {
        return &mEnemyLink;
    }

    virtual void vt_0x80() {
        return;
    }
    virtual void vt_0x84() {
        return;
    }
    virtual UNKWORD vt_0x88(UNKWORD a) {
        return a;
    };
    /* 0x330 */ dAcRef_c<dAcEnBase_c> mEnemyLink;
    /* 0x33C */ u32 mInteractionFlags;
    /* 0x340 */ u16 field_0x338;
    /* 0x342 */ u16 mHealth;
    /* 0x344 */ u8 _344[0x358 - 0x344];
    /* 0x358 */ mVec3_c mFinalBlowPosition;
    /* 0x364 */ u8 _364[0x374 - 0x364];
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

    // Returns 0-3 based on collision
    // 2
    int fn_800301b0(const mVec3_c &pos, mAng ang, bool, f32);
    void fn_800306d0();
    void fn_80030700();
    void fn_80030c20(u32 flags, f32, f32, f32, f32);

    s32 someEnemyDamageCollisionStuffMaybe(dColliderLinkedList &list, u16 *pOutFlags);

    bool ChkCrossPlayer(f32 height);

protected:
    // TODO: Make accessors for this list to auto-convert to dAcEnBase?
    static fLiMgBa_c sEnemyList;
};

#endif
