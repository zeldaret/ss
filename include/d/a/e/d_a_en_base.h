#ifndef D_A_EN_BASE_H
#define D_A_EN_BASE_H

#include "common.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/col/c/c_cc_d.h"
#include "d/d_cc.h"
#include "f/f_list_mg.h"
#include "f/f_list_nd.h"
#include "f/f_profile.h"
#include "m/m3d/m_mdl.h"
#include "m/m_angle.h"
#include "m/m_vec.h"

class dAcBomb_c;

struct dAcEnData {
    // Unknown Size yet
    /* 0x00 */ u8 _0x00[0x04 - 0x00];
    /* 0x04 */ u16 health;
    /* 0x06 */ u8 _0x06[0x0C - 0x06];
    /* 0x0C */ f32 field_0x0C;
    /* 0x10 */ f32 field_0x10;
    /* 0x14 */ f32 field_0x14;

    // This Structure corresponds to
    // cCcD_SrcGObjAt -> offset 0x0C
    struct ccRelated {
        u8 field_0x00;
        u8 field_0x01;
        u8 field_0x02;
        u8 field_0x03;
    };
    /* 0x18 */ ccRelated field_0x18[8];

    struct damgeRelated {
        u8 field_0x00;
        u8 field_0x01;
        u8 field_0x02;
        u8 field_0x03;
    };
    /* 0x38 */ damgeRelated field_0x38[32];
};

class dAcEnBase_c : public dAcObjBase_c {
public:
    enum InteractionFlags_e {
        INTERACT_0x1 = (1 << 0),
        INTERACT_0x4 = (1 << 2),
        INTERACT_0x10 = (1 << 4),
        INTERACT_0x40 = (1 << 6),
        INTERACT_0x100 = (1 << 8),
        INTERACT_0x1000 = (1 << 12),
        INTERACT_0x2000 = (1 << 13),
    };

public:
    dAcEnBase_c();
    virtual ~dAcEnBase_c();

    virtual int preExecute() override;
    virtual void postExecute(MAIN_STATE_e state) override;
    virtual int preDraw() override;

    virtual fLiNdBa_c *getObjectListEntry() override {
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

    bool checkInteractionFlags(u32 mask) const {
        return mInteractionFlags & mask;
    }
    void setInteractionFlags(u32 flag) {
        mInteractionFlags |= flag;
    }
    void unsetInteractionFlags(u32 flag) {
        mInteractionFlags &= ~flag;
    }
    void setFinalBlowPosition(const mVec3_c &v) {
        mFinalBlowPosition = v;
    }

    s32 getHealth() const {
        return mHealth;
    }

    static dAcEnBase_c *GetNextEnemy(dAcEnBase_c *pParent) {
        return static_cast<dAcEnBase_c *>(getNextObject(&sEnemyList, pParent));
    }
    static dAcEnBase_c *FindEnemy(fProfile::PROFILE_NAME_e profile, dAcEnBase_c *pParent = nullptr) {
        return static_cast<dAcEnBase_c *>(findObject(profile, &sEnemyList, pParent));
    }

    // TODO: Better names for the below 5 functions
    dAcObjBase_c *isPlayerOwnedActorNearby(ProfileName, const mVec3_c &pos, f32 radius);
    dAcBomb_c *isPlayerOwnedBombNearby(const mVec3_c &pos, f32 radius);
    dAcBomb_c *getBombWithinRadius(f32 radius);
    dAcObjBase_c *isPlayerOwnedProjectileNearby(const mVec3_c &pos, f32 radius);
    dAcObjBase_c *isProjectileNearby(f32 radius);

    void playDeathEffect0(const mVec3_c &pos, const mVec3_c *pScale, bool skipKill);
    void playDeathEffect1(const mVec3_c &pos, const mVec3_c *pScale, bool skipKill);
    void playDeathEffect2(const mVec3_c &pos, const mVec3_c *pScale, bool skipKill);

    void setEndDat(const char *oarc, const char *endatName);
    void setCcFromEnemyData(cCcD_Obj &cc, int idx);
    void setCcFromEnemyData(dColliderLinkedList &ccList, int idx);

    u16 calcHealth(u32 damage, cCcD_Obj *pCc);

    // Deals with dealing damage to Enemy
    // Returns a value 0-13 (similar to mStts rank?)
    int someEnemyDamageCollisionStuffMaybe(dColliderLinkedList &list, u16 *pOutMod);
    int fn_8002FDE0(cCcD_Obj &mCc, u16 *pOutMod);

    bool snapToGround(f32 range);
    void checkSlope(const mVec3_c &pos, s16 &, s16 &, f32);
    // Returns 0-3 based on collision
    // 0 - None
    // 1 - bool == true and Y linCross (+100.0f height)
    // 2 - gnd cross with gnd/water
    // 3 - Sand/Lava
    int fn_800301b0(const mVec3_c &pos, const mAng ang, bool, f32);

    bool lineCrossPlayer(f32 height);

    void setDamageFlash(m3d::mdl_c &mdl, u8 red, bool bAdjust, u8);

    // Changes the model to fade to black. Returns true on completion
    // bAdjust is used to scale the fade (1 -> scale, 0 -> dont scale)
    //    when having multiple models, this is only true once for the set of calls
    bool setDeathFadeout(m3d::mdl_c &mdl, u8 bAdjust, f32 rate);

    void fn_800306C0();

    void fn_800306D0();
    void fn_80030700();

    dAcObjBase_c *targetPlayerOrScrapper(f32 biasToScrapper);
    static void fn_800307E0(const mVec3_c &pPos, s32);
    static void fn_80030820();
    static bool fn_80030840(
        const mVec3_c &start, const mVec3_c &end, f32 distance, const mAng target, const mAng thresh, bool checkCross
    );
    static void fn_80030980(m3d::mdl_c &mdl, u8 removeStage, bool markDirty);
    bool isWithinCameraView(f32, f32);
    void fn_80030C20(u32 flags, f32, f32, f32, f32);

public:
    /* 0x330 */ fLiNdBa_c mEnemyLink;
    /* 0x33C */ u32 mInteractionFlags;
    /* 0x340 */ u16 field_0x338;
    /* 0x342 */ u16 mHealth;
    /* 0x344 */ u8 _0x344[0x346 - 0x344];
    /* 0x346 */ u8 field_0x346;
    /* 0x347 */ u8 _0x347[0x358 - 0x347];
    /* 0x358 */ mVec3_c mFinalBlowPosition;
    /* 0x364 */ f32 field_0x364;
    /* 0x364 */ f32 field_0x368;
    /* 0x36C */ u8 _0x36C[0x374 - 0x36C];
    /* 0x374 */ const dAcEnData *mpEnemyData;

protected:
    // TODO: Make accessors for this list to auto-convert to dAcEnBase?
    static fLiMgBa_c sEnemyList;
    static const dAcEnData sDefaultEnData;

    static s32 lbl_80571930;     // No clue
    static s32 lbl_80571934;     // No clue
    static mVec3_c lbl_80597020; // No clue
};

#endif
