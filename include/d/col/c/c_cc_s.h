#ifndef C_CC_S_H
#define C_CC_S_H

#include "common.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/col/c/c_cc_d.h"
#include "m/m_mtx.h"
#include "m/m_vec.h"

/**
 * This Still seems to be exist, but looks like was heavily revised.
 * No more virtual functions
 * a couple more entry arrays (a Matrix one and a model one)
 */

enum WeightType {
    WeightType_0 = 0,
    WeightType_1 = 1,
    WeightType_2 = 2,
};

class cCcS {
public:
    /* 0x0000 */ cCcD_GObj *mpObjAt[0x200];
    /* 0x0800 */ cCcD_GObj *mpObjTg[0x300];
    /* 0x1400 */ cCcD_GObj *mpObjCo[0x200];
    /* 0x1c00 */ cCcD_GObj *mpObj[0x340];
    /* 0x2900 */ UNKTYPE *mpUnk[0x100];
    /* 0x2D00 */ u16 mObjAtCount;
    /* 0x2d02 */ u16 mObjTgCount;
    /* 0x2D04 */ u16 mObjCoCount;
    /* 0x2D06 */ u16 mObjCount;
    /* 0x2D08 */ u32 mUnkCount;
    /* 0x2D0C */ cCcD_DivideArea mDivideArea;
    /* 0x2D4C */ bool mbAreaSet;
    /* 0x2D50 */ mMtx_c mAreas[16][2]; // could be a structure?
    /* 0x3350 */ int mAreaCount;

    cCcS() {}

    void Ct();
    void Dt();

    void Set(cCcD_GObj *);

    void ChkCo(mVec3_c *, dAcObjBase_c *);
    bool fn_80357c90(mVec3_c *, mVec3_c *, u32, UNKTYPE *);

    WeightType GetWt(u8) const;
    void ClrCoHitInf();
    void ClrTgHitInf();
    void ClrAtHitInf();
    bool ChkNoHitAtTg(cCcD_Obj *, cCcD_Obj *);
    void ChkAtTg();
    bool ChkNoHitCo(cCcD_Obj *, cCcD_Obj *);
    void CalcTgPlusDmg(cCcD_Obj *, cCcD_Obj *, cCcD_Stts *, cCcD_Stts *);
    void SetAtTgCommonHitInf(cCcD_Obj *, cCcD_Obj *, cXyz *);
    void SetCoCommonHitInf(cCcD_Obj *, cXyz *, cCcD_Obj *, cXyz *, f32);
    void SetPosCorrect(cCcD_Obj *, cXyz *, cCcD_Obj *, cXyz *, f32);
    void CalcArea();
    void Move();
    void DrawClear();
    void SetCoGObjInf(bool, bool, cCcD_GObjInf *, cCcD_GObjInf *, cCcD_Stts *, cCcD_Stts *, cCcD_GStts *, cCcD_GStts *);
    void
    SetAtTgGObjInf(bool, bool, cCcD_Obj *, cCcD_Obj *, cCcD_GObjInf *, cCcD_GObjInf *, cCcD_Stts *, cCcD_Stts *, cCcD_GStts *, cCcD_GStts *, cXyz *);
    bool ChkNoHitGAtTg(cCcD_GObjInf const *, cCcD_GObjInf const *, cCcD_GStts *, cCcD_GStts *);
    bool
    ChkAtTgHitAfterCross(bool, bool, cCcD_GObjInf const *, cCcD_GObjInf const *, cCcD_Stts *, cCcD_Stts *, cCcD_GStts *, cCcD_GStts *);
    bool ChkNoHitGCo(cCcD_Obj *, cCcD_Obj *);
    ~cCcS() {}
    void MoveAfterCheck();
    void SetCoGCorrectProc(cCcD_Obj *, cCcD_Obj *);
}; // Size = 0x284C

#endif /* C_CC_S_H */
