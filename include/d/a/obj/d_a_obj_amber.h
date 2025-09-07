#ifndef D_A_OBJ_AMBER_H
#define D_A_OBJ_AMBER_H

#include "d/a/obj/d_a_obj_base.h"
#include "d/d_shadow.h"
#include "m/m3d/m_smdl.h"
#include "m/m_quat.h"
#include "m/m_vec.h"

class dAcOAmber_c : public dAcObjBase_c {
public:
    dAcOAmber_c() {}
    virtual ~dAcOAmber_c() {}

    virtual bool createHeap() override;
    virtual int actorCreate() override;
    virtual int actorPostCreate() override;
    virtual int draw() override;
    virtual void registerInEvent() override;
    virtual void unkVirtFunc_0x6C() override;

private:
    /* 0x330 */ m3d::smdl_c mMdl;
    /* 0x34C */ dShadowCircle_c mShadowCircle;
    /* 0x354 */ mVec3_c mMin;
    /* 0x360 */ mVec3_c mMax;
    /* 0x36C */ mQuat_c mShadowRot;
    /* 0x37C */ f32 field_0x37c;
};

#endif
