#ifndef D_A_OBJ_SW_SWORD_BEAM_H
#define D_A_OBJ_SW_SWORD_BEAM_H

#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_s_acch.h"
#include "d/col/cc/d_cc_d.h"
#include "d/d_shadow.h"
#include "m/m3d/m_anmtexpat.h"
#include "m/m3d/m_anmtexsrt.h"
#include "m/m3d/m_smdl.h"
#include "m/m_quat.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "s/s_State.hpp"
#include "toBeSorted/actor_event.h"

class dAcOSwSwordBeam_c : public dAcObjBase_c {
public:
    dAcOSwSwordBeam_c() : mEvent(*this, nullptr), mStateMgr(*this) {}
    virtual ~dAcOSwSwordBeam_c() {}

    virtual bool createHeap() override;
    virtual int create() override;
    virtual int doDelete() override;
    virtual int actorExecute() override;
    virtual int actorExecuteInEvent() override;
    virtual void registerInEvent() override;
    virtual void unkVirtFunc_0x6C() override;
    virtual int draw() override;

    STATE_FUNC_DECLARE(dAcOSwSwordBeam_c, Wait);
    STATE_FUNC_DECLARE(dAcOSwSwordBeam_c, Rotate);
    STATE_FUNC_DECLARE(dAcOSwSwordBeam_c, RotateEnd);
    STATE_FUNC_DECLARE(dAcOSwSwordBeam_c, OnSwitch);
    STATE_FUNC_DECLARE(dAcOSwSwordBeam_c, End);

    void setSpawnedFromOtherActor(bool val) {
        mSpawnedFromOtherActor = val;
    }

    void setSomeFloatFromOtherActor(f32 val) {
        mSomeFloatFromOtherActor = val;
    }

    void setField_0xC90(f32 val) {
        field_0xC90 = val;
    }

    void setField_0xCAA(bool val) {
        field_0xCAA = val;
    }

private:
    s32 getEquippedSword();
    void fn_507_1A50();
    void fn_507_1AF0();
    bool fn_507_1F30();
    void fn_507_1F80();
    void fn_507_2130();
    void fn_507_22A0();

    bool checkSceneFlag();
    bool isleOfSongsCanGetHit();
    void setSwSwordBeamAtBoneWithFlag(char *bone_name, m3d::mdl_c *model, u8 *sceneflag);

    ActorEventRelated mEvent;
    nw4r::g3d::ResFile mResFile;
    m3d::smdl_c mMdl;
    dShadowCircle_c mShadow;
    m3d::anmTexSrt_c mAnmTexSrt;
    m3d::anmTexPat_c mAnmTexPat;
    dCcD_Unk mUnk1;
    dCcD_Unk mUnk2;
    dBgS_ObjAcch mObjAcch;
    dBgS_AcchCir mAcchCir;
    /* 0x??? */ STATE_MGR_DECLARE(dAcOSwSwordBeam_c);
    mQuat_c field_0xc58;
    mQuat_c field_0xc68;
    mQuat_c field_0xc78;
    f32 field_0xc88;
    f32 mSomeFloatFromOtherActor;
    f32 field_0xC90;
    s16 field_0xC94;
    s16 field_0xC96;
    s16 field_0xC98;
    s16 field_0xc9a;
    s16 field_0xC9C;
    s16 field_0xC9E;
    mAng field_0xCA0;
    s16 field_0xCA2;
    u8 mSubtype;
    u8 mSceneflag;

    u8 field_0xCA6;
    u8 mTimer;
    bool field_0xCA8;
    bool mSpawnedFromOtherActor;
    bool field_0xCAA;
    bool field_0xCAB;
    bool field_0xCAC;

    s32 mCsExitID;

    static dCcD_SrcUnk sCcSrc;
};

class dAcGoddessCrestHolder_c : public dAcObjBase_c {
protected:
    virtual void setCrestAtBone(char *bone_name, m3d::smdl_c *model, u8 *sceneflag);
    virtual void setCrestPosRot(m3d::smdl_c *mdl);
    virtual void vt_0x88(f32 &param);

    /* 0x330 */ dAcRef_c<dAcOSwSwordBeam_c> mSwSwordBeamRef;
    /* 0x33C */ u32 mBoneID;
    /* 0x340 */ mVec3_c mSwPos;
    /* 0x34C */ mVec3_c mSwScale;
    /* 0x358 */ mAng3_c mSwRot;
    /* 0x360 */ float mSendToSwSB;
    /* 0x364 */ bool mTransferToCrest;
    /* 0x365 */ u8 pad[3];
};

#endif
