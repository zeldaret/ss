#ifndef D_A_OBJ_FAIRY_H
#define D_A_OBJ_FAIRY_H

#include <d/a/obj/d_a_obj_base.h>
#include <m/m3d/m_anmmdl.h>
#include <m/m3d/m_shadow.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcObjFairy_c : public dAcObjBase_c {
public:
    dAcObjFairy_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcObjFairy_c() {}

    virtual bool createHeap() override;
    virtual int create() override;
    virtual int doDelete() override;
    virtual int actorExecute() override;
    virtual int draw() override;


    STATE_FUNC_DECLARE(dAcObjFairy_c, Wait);
    STATE_FUNC_DECLARE(dAcObjFairy_c, Avoid);
    STATE_FUNC_DECLARE(dAcObjFairy_c, PlayerAvoid);
    STATE_FUNC_DECLARE(dAcObjFairy_c, CureStart);
    STATE_FUNC_DECLARE(dAcObjFairy_c, Cure);
    STATE_FUNC_DECLARE(dAcObjFairy_c, CureEnd);
    STATE_FUNC_DECLARE(dAcObjFairy_c, CatchDemo);


private:

    bool shouldAvoidLink() const;
    bool isCuring() const;
    bool canTargetWithBugNet() const;

    /* 0x330 */ UNKWORD field_0x330;
    /* 0x334 */ m3d::mdlAnmChr mModel;
    /* 0x39C */ m3d::mShadowCircle_c mShadow;
    /* 0x3A4 */ u8 field_0x3A4[0x4B0 - 0x3A4];
    /* 0x4B0 */ f32 field_0x4B0;
    /* 0x4B4 */ u8 field_0x4B4[0xA50 - 0x4B4];
    /* 0xA50 */ STATE_MGR_DECLARE(dAcObjFairy_c);
    /* 0xA8C */ u8 field_0xA8C[0xB89 - 0xA8C];
    /* 0xB89 */ u8 field_0xB89;
    
    // TODO collision, effects
};

#endif 
