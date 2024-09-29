#ifndef D_A_OBJ_ARROW_H
#define D_A_OBJ_ARROW_H

#include <d/a/obj/d_a_obj_base.h>
#include <m/m3d/m_smdl.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>
#include <toBeSorted/cc/d_cc_shape_colliders.h>
#include <toBeSorted/effects_struct.h>

class dAcArrow_c : public dAcObjBase_c {
public:
    dAcArrow_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcArrow_c();

    virtual bool createHeap() override;
    virtual int create() override;
    virtual int doDelete() override;
    virtual int actorExecute() override;
    virtual int draw() override;

    STATE_FUNC_DECLARE(dAcArrow_c, Wait);
    STATE_FUNC_DECLARE(dAcArrow_c, Move);
    STATE_FUNC_DECLARE(dAcArrow_c, ActorStop);
    STATE_FUNC_DECLARE(dAcArrow_c, BgStop);
    STATE_FUNC_DECLARE(dAcArrow_c, Bound);

    void hitCallback(dCcD_GObjInf* i_objInfA, dAcObjBase_c* i_actorB, dCcD_GObjInf* i_objInfB);

private:

    static dCcD_SrcGObjInf sCc1;
    static dCcD_SrcSph sCc2;
    static const dCcD_SrcSph sCc3;

    /* 0x330 */ nw4r::g3d::ResFile mResFile;
    /* 0x334 */ m3d::smdl_c mModel;
    /* 0x350 */ dAcRef_c<dAcObjBase_c> mRef1;
    /* 0x35C */ dCcD_Unk1 mCollider1;
    /* 0x4CC */ dCcD_Sph mCollider2;
    /* 0x61C */ u8 field_0x61C[0x648 - 0x61C];
    /* 0x648 */ EffectsStruct mEffects;
    /* 0x67C */ u32 field_0x67C;
    /* 0x680 */ u8 mSubType;
    /* 0x681 */ u8 field_0x681;
    /* 0x682 */ u8 mDespawnTimer;
    /* 0x684 */ s16 field_0x684;
    /* 0x686 */ u8 field_0x686[0x688 - 0x686];
    /* 0x688 */ u16 field_0x688;
    /* 0x68A */ u16 field_0x68A;
    /* 0x68C */ u8 field_0x68C[0x6A8 - 0x68C];
    /* 0x6A8 */ f32 field_0x6A8;
    /* 0x6AC */ u8 field_0x6AC[0x6F8 - 0x6AC];
    /* 0x6F8 */ dAcRef_c<dAcObjBase_c> mRef2;
    /* 0x704 */ STATE_MGR_DECLARE(dAcArrow_c);
};

#endif
