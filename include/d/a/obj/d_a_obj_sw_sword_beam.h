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

class dAcOSwSwordBeam_c;

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

    /* 0x330 */ ActorEventRelated mEvent;
    /* 0x380 */ nw4r::g3d::ResFile mResFile;
    /* 0x384 */ m3d::smdl_c mMdl;
    /* 0x3A0 */ dShadowCircle_c mShadow;
    /* 0x3A8 */ m3d::anmTexSrt_c mAnmTexSrt;
    /* 0x3D4 */ m3d::anmTexPat_c mAnmTexPat;
    /* 0x400 */ dCcD_Unk mUnk1;
    /* 0x608 */ dCcD_Unk mUnk2;
    /* 0x810 */ dBgS_ObjAcch mObjAcch;
    /* 0xBC0 */ dBgS_AcchCir mAcchCir;
    /* 0xC1C */ STATE_MGR_DECLARE(dAcOSwSwordBeam_c);
    /* 0xC58 */ mQuat_c field_0xC58;
    /* 0xC68 */ mQuat_c field_0xC68;
    /* 0xC78 */ mQuat_c field_0xC78;
    /* 0xC88 */ f32 field_0xC88;
    /* 0xC8C */ f32 mSomeFloatFromOtherActor;
    /* 0xC90 */ f32 field_0xC90;
    /* 0xC94 */ mAng field_0xC94;
    /* 0xC96 */ mAng field_0xC96;
    /* 0xC98 */ mAng field_0xC98;
    /* 0xC9A */ mAng field_0xC9A;
    /* 0xC9C */ mAng field_0xC9C;
    /* 0xC9E */ s16 field_0xC9E;
    /* 0xCA0 */ mAng field_0xCA0;
    /* 0xCA2 */ mAng field_0xCA2;
    /* 0xCA4 */ u8 mSubtype;
    /* 0xCA5 */ u8 mSceneflag;
    /* 0xCA6 */ u8 field_0xCA6;
    /* 0xCA7 */ u8 mTimer;
    /* 0xCA8 */ bool field_0xCA8;
    /* 0xCA9 */ bool mSpawnedFromOtherActor;
    /* 0xCAA */ bool field_0xCAA;
    /* 0xCAB */ bool field_0xCAB;
    /* 0xCAC */ bool field_0xCAC;
    /* 0xCB0 */ s32 mCsExitID;

    static dCcD_SrcUnk sCcSrc;
    static const s16 lbl_507_data_0;
    static const s16 lbl_507_data_50;
};

#endif
