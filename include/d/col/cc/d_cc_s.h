#include "common.h"
#include "d/col/c/c_cc_d.h"
#include "d/col/cc/d_cc_mass_s.h"
#include "m/m_angle.h"
#include "m/m_mtx.h"
#include "m/m_vec.h"

struct mMtxPair {
    mMtx_c mMtx;
    mMtx_c mInv;
};

class dCcS {
private:
    /* 0x0000 */ cCcD_Obj *mpObjAt[0x200];
    /* 0x0800 */ cCcD_Obj *mpObjTg[0x300];
    /* 0x1400 */ cCcD_Obj *mpObjCo[0x200];
    /* 0x1c00 */ cCcD_Obj *mpObj[0x340];
    /* 0x2900 */ UNKTYPE *mpUnk[0x100];
    /* 0x2D00 */ u16 mObjAtCount;
    /* 0x2d02 */ u16 mObjTgCount;
    /* 0x2D04 */ u16 mObjCoCount;
    /* 0x2D06 */ u16 mObjCount;
    /* 0x2D08 */ u32 mUnkCount;
    /* 0x2D0C */ cCcD_DivideArea mDivideArea;
    /* 0x2D4C */ bool mbAreaSet;
    /* 0x2D50 */ mMtxPair mAreas[16]; // could be a structure?
    /* 0x3350 */ int mAreaCount;
    /* 0x3354*/ dCcMassS_Mng mMassMng;

public:
    dCcS();
    virtual ~dCcS();

    // This is a remnant of cCcS, which dCcS::Ct calls. Idk the correct layout
    void Ct_cCcS();

    void Ct();
    void Dt();

    static bool ChkRemoveActor(dAcObjBase_c *pActor);
    static bool ChkNoHitAtTg(cCcD_Obj *, cCcD_Obj *);
    static bool ChkAtTgCommonHitInf(cCcD_Obj *, cCcD_Obj *);
    void ChkAtTg();

    static bool ChkNoHitCo(cCcD_Obj *, cCcD_Obj *);
    static void SetCoCommonHitInf(cCcD_Obj *, mVec3_c *, cCcD_Obj *, mVec3_c *, f32);
    void ChkCo();

    static bool ChkSheildFrontRange(cCcD_Obj *, cCcD_Obj *);
    static bool ChkShield(cCcD_Obj *, cCcD_Obj *, cCcD_ShieldChkCallback *);
    static bool ChkAtTgHitAfterCross(bool, bool, cCcD_Obj *, cCcD_Obj *);
    static void SetCoObjInf(bool, bool, cCcD_Obj *, cCcD_Obj *);
    static void SetPosCorrect(cCcD_Obj *, mVec3_c *, cCcD_Obj *, mVec3_c *, f32);
    static void CalcParticleAngle(cCcD_Obj *, cCcD_Obj *, mAng3_c *, bool);
    static void fn_80356fd0(mVec3_c *, cCcD_Obj *, cCcD_Obj *);
    static u32 ProcAtTgHitmark(cCcD_Obj *, cCcD_Obj *, mVec3_c *, u32, mVec3_c *);
    static void SetAtTgObjInf(bool, bool, cCcD_Obj *, cCcD_Obj *, mVec3_c *, bool);

    bool ChkCamera(mVec3_c &, mVec3_c &, dAcObjBase_c *, dAcObjBase_c *, dAcObjBase_c *, f32);
    bool
    ChkCameraPoint(const mVec3_c &, const mVec3_c &, mVec3_c &, dAcObjBase_c *, dAcObjBase_c *, dAcObjBase_c *, f32);

    bool ChkCo(const mVec3_c &, dAcObjBase_c *);
    bool fn_80357c90(const mVec3_c &, const mVec3_c &, u32, UNKTYPE **);
    void Set(cCcD_Obj *);
    void MoveAfterCheck();
    void RemoveDeadObj();
    void CalcArea();
    void Move();
    void MassClear();
    void SetArea(mMtx_c *);
    void ClearArea();
    void AdjustHitPos();

    static dCcS *GetInstance();
    static dCcS *sInstance;
};
