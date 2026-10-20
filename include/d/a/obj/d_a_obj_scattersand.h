#ifndef D_A_OBJ_SCATTERSAND_H
#define D_A_OBJ_SCATTERSAND_H

#include "d/a/d_a_base.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_w.h"
#include "d/col/cc/d_cc_d.h"
#include "m/m3d/m_smdl.h"
#include "m/m_vec.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "toBeSorted/time_area_mgr.h"

class dTgInsect_c;
class dAcOScatterSand : public dAcObjBase_c {
public:
    dAcOScatterSand() {}
    virtual ~dAcOScatterSand() {}

    virtual int doDelete() override;
    virtual int preExecute() override;
    virtual int draw() override;
    virtual bool createHeap() override;
    virtual int actorCreate() override;
    virtual int actorPostCreate() override;
    virtual int actorExecute() override;

    void calcTimer();
    f32 fn_110_480() const;
    void revealInside();
    bool isVisible();

    f32 getSomeScale() {
        return mScale.y * 41.65f / 100.0f;
    }

    bool getField_0x6E4() const {
        return field_0x6E4;
    }
    void setField_0x6E4(bool b = true) {
        field_0x6E4 = b;
    }
    void setField_0x6DC(s16 time) {
        field_0x6DC = time;
    }

private:
    /* 0x330 */ nw4r::g3d::ResFile mRes;
    /* 0x334 */ m3d::smdl_c mMdl;
    /* 0x350 */ dCcD_Cyl mCyl;
    /* 0x4A0 */ TimeAreaStruct mTimeArea;
    /* 0x4AC */ dAcRef_c<dTgInsect_c> mInsect;
    /* 0x4B8 */ dBgW mBgW;
    /* 0x6C8 */ mVec3_c mSpawnScale;
    /* 0x6D4 */ f32 mSpawnScaleY;
    /* 0x6D8 */ s16 field_0x6D8;
    /* 0x6DA */ s16 field_0x6DA;
    /* 0x6DC */ s16 field_0x6DC;
    /* 0x6DE */ s16 field_0x6DE;
    /* 0x6E0 */ u8 mClearedSceneflag;
    /* 0x6E1 */ u8 field_0x6E1;
    /* 0x6E2 */ u8 field_0x6E2;
    /* 0x6E3 */ u8 field_0x6E3;
    /* 0x6E4 */ bool field_0x6E4;
};

#endif
