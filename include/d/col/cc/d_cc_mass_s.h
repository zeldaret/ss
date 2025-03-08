#ifndef D_CC_MASS_OBJ_H
#define D_CC_MASS_OBJ_H

#include "common.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/col/c/c_cc_d.h"
#include "m/m_vec.h"

typedef void (*dCcMassS_ObjCallback)(dAcObjBase_c *, mVec3_c *, u32);

class dCcMassS_Obj {
private:
    /* 0x00 */ cCcD_Obj *mpObj;
    /* 0x04 */ u8 mPriority;
    /* 0x08 */ dCcMassS_ObjCallback mpCallback;
    /* 0x0C */ cCcD_DivideInfo mDivideInfo;
    /* 0x1C vtable*/

public:
    dCcMassS_Obj();
    virtual ~dCcMassS_Obj();
    void Set(cCcD_Obj *p_obj, u8 priority, dCcMassS_ObjCallback callback);
    void Clear();

    cCcD_Obj *GetObj() {
        return mpObj;
    }
    u8 GetPriority() const {
        return mPriority;
    }
    dCcMassS_ObjCallback GetCallback() const {
        return mpCallback;
    }
    cCcD_DivideInfo &GetDivideInfo() {
        return mDivideInfo;
    }
};

class dCcMassS_HitInf {
private:
    /* 0x00 */ cCcD_Obj *mpArea;
    /* 0x04 */ cCcD_Obj *mpAtObj;
    /* 0x08 */ cCcD_Obj *mpCoObj;
    /* 0x0C */ f32 mCoHitLen;
    /* 0x10 vtable */
public:
    dCcMassS_HitInf();
    virtual ~dCcMassS_HitInf();

    void ClearPointer();
    void SetAreaHitObj(cCcD_Obj *obj) {
        mpArea = obj;
    }
    void SetCoHitObj(cCcD_Obj *obj) {
        mpCoObj = obj;
    }
    void SetAtHitObj(cCcD_Obj *obj) {
        mpAtObj = obj;
    }
    void SetCoHitLen(f32 len) {
        mCoHitLen = len;
    }
    cCcD_Obj *GetAtHitObj() const {
        return mpAtObj;
    }
};

class dCcMassS_Mng {
public:
    /* 0x0000 */ cCcD_DivideArea mDivideArea;
    /* 0x0040 */ s32 mMassObjCount;
    /* 0x0044 */ dCcMassS_Obj mMassObjs[20];
    /* 0x02C4 */ s32 mMassAreaCount;
    /* 0x02C8 */ dCcMassS_Obj mMassAreas[5];
    /* 0x0368 */ cCcD_CylAttr mCylAttr;
    /* 0x03A8 */ u8 field_0x3A8;
    /* 0x03A9 */ u8 field_0x3A9;
    /* 0x03AA */ u8 field_0x3AA;
    /* 0x03AB */ u8 mResultCam;
    /* 0x03AC */ Vec mCamTopPos;
    /* 0x03B8 */ f32 mCamTopDist;
    /* 0x03BC */ Vec mCamBottomPos;
    /* 0x03C8 */ f32 mCamBottomDist;
    /* 0x03CC */ cCcD_CpsAttr mCpsAttr;
    /* 0x042C */ cCcD_DivideInfo mDivideInfo;
    /* 0x043C vtable */

public:
    dCcMassS_Mng();
    virtual ~dCcMassS_Mng();

    void Ct();

    void SetAttr(f32 radius, f32 height, u8 param_2, u8 param_3);
    void Prepare();
    u32 Chk(mVec3_c *p_xyz, dAcObjBase_c **p_actor, dCcMassS_HitInf *p_hitInf);
    void Clear();

    // Not original names, but made distinct for clarity (original was just one `Set)
    void SetObj(cCcD_Obj *p_obj, u8 priority);
    void SetArea(cCcD_Obj *p_obj, u8 priority, dCcMassS_ObjCallback callback);

    // void SetCam(cM3dGCps const &cps);
    // u8 GetResultCam() const;
    // void GetCamTopPos(Vec *p_out);
};

#endif
