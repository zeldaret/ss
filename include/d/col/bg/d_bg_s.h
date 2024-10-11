#ifndef D_BG_S_H
#define D_BG_S_H

#include "common.h"
#include "d/a/d_a_base.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/a/obj/d_a_obj_bg.h"
#include "d/col/bg/d_bg_w.h"
#include "d/col/bg/d_bg_w_base.h"
#include "d/col/bg/d_bg_w_kcol.h"

class dBgW;

class cBgS_ChkElm {
public:
    /* 0x00 */ dBgW_Base *mpBgW;
    /* 0x04 */ dAcRef_c<dAcObjBase_c> mObj;
    /* 0x10 vtable */

public:
    cBgS_ChkElm();
    virtual ~cBgS_ChkElm();
    void Init();
    void Release();

    void Regist2(dBgW_Base *, dAcObjBase_c *);

    bool ChkUsed() const {
        if (mpBgW == nullptr) {
            return false;
        }
        const dAcObjBase_c *pObj = mObj.get();
        return !(pObj && pObj->ChkProperty_0x40000000());
    }

    bool CheckAll(cBgS_Chk *other) const {
        bool check = ChkUsed();
        if (check) {
            const cPartition &part = mpBgW->GetPartition();
            if ((part.mX & other->mPartition.mX) && (part.mZ & other->mPartition.mZ) &&
                (part.mY & other->mPartition.mY)) {
                check = true;
            } else {
                check = false;
            }
        }
        if (check) {
            check = !other->ChkSameActorPid(mObj.get());
        }
        if (check) {
            check = !(other->mField_0x0C & mpBgW->GetField_0x20());
        }
        if (check) {
            check = (other->mField_0x0E & mpBgW->GetField_0x22());
        }
        return check;
    }
};

class cBgS {
public:
    /* 0x0000 */ cBgS_ChkElm mChkElem[BG_ID_MAX];
    /* 0x2EE0 */ s32 mSetCounter;
    /* 0x2EE4 vtable */
    virtual ~cBgS();
    virtual void Ct();
    virtual void Dt();

    /* 0x2EE8 */ dBgWKCol *mpBgKCol;
    /* 0x2EEC */ dAcRef_c<dAcObg_c> mAcOBg;
    /* 0x2EF8 */ u32 mField_0x2EF8;

public:
    cBgS();
    static void fn_80339de0(dBgW_Base *);
    bool Regist(dBgW_Base *, dAcObjBase_c *);
    bool Release(dBgW_Base *);
    static void ConvDzb(void *);
    bool LineCross(cBgS_LinChk *);
    f32 GroundCross(cBgS_GndChk *);
    void ShdwDraw(cBgS_ShdwDraw *);
    void fn_8033a1e0();

    bool RegistKCol(dBgWKCol *, dAcObg_c *);

    const dAcObjBase_c *GetActorPointer(int) const;
    dBgW_Base *GetBgWBasePointer(cBgS_PolyInfo const &) const;
    bool ChkPolySafe(cBgS_PolyInfo const &) const;
    s32 GetGrpRoomId(cBgS_PolyInfo const &) const;
    bool GetTriPla(cBgS_PolyInfo const &, cM3dGPla *) const;
    bool GetTriPnt(cBgS_PolyInfo const &, mVec3_c *, mVec3_c *, mVec3_c *) const;
    u32 GetGrpInf(cBgS_PolyInfo const &) const;

    const dAcObjBase_c *GetActorPointer(const cBgS_PolyInfo &info) const;
}; // Size: 0x2EFC

class dBgS_Acch;

u8 dKy_pol_sound_get(cBgS_PolyInfo const *param_0);

class dBgS : public cBgS {
private:
    static dBgS sInstance;

public:
    dBgS();
    ~dBgS();
    void Ct();
    void Dt();
    void ClrMoveFlag();
    void Move();
    bool Regist(dBgW_Base *, fBase_c *);
    bool UnRegist(dBgW_Base *);
    bool ChkMoveBG(cBgS_PolyInfo const &);
    bool ChkMoveBG_NoDABg(cBgS_PolyInfo const &);
    s32 GetExitId(cBgS_PolyInfo const &);
    s32 GetPolyColor(cBgS_PolyInfo const &);
    BOOL GetHorseNoEntry(cBgS_PolyInfo const &);
    int GetSpecialCode(cBgS_PolyInfo const &);
    int GetMagnetCode(cBgS_PolyInfo const &);
    int GetMonkeyBarsCode(cBgS_PolyInfo const &);
    u32 GetUnderwaterRoofCode(cBgS_PolyInfo const &);
    s32 GetWallCode(cBgS_PolyInfo const &);
    int GetPolyAtt0(cBgS_PolyInfo const &);
    int GetPolyAtt1(cBgS_PolyInfo const &);
    int GetGroundCode(cBgS_PolyInfo const &);
    s32 GetCamMoveBG(cBgS_PolyInfo const &);
    s32 GetRoomCamId(cBgS_PolyInfo const &);
    s32 GetRoomPathId(cBgS_PolyInfo const &);
    s32 GetRoomPathPntNo(cBgS_PolyInfo const &);
    int GetGrpSoundId(cBgS_PolyInfo const &);
    u32 ChkGrpInf(cBgS_PolyInfo const &, u32);
    s32 GetRoomId(cBgS_PolyInfo const &);
    bool GetPolyAttackThrough(cBgS_PolyInfo const &);
    u32 ChkPolyHSStick(cBgS_PolyInfo const &);
    void WallCorrect(dBgS_Acch *);
    void WallCorrectSort(dBgS_Acch *);
    f32 RoofChk(dBgS_RoofChk *);
    bool SplGrpChk(dBgS_SplGrpChk *);
    bool SphChk(dBgS_SphChk *, void *);
    void MoveBgCrrPos(
        cBgS_PolyInfo const &i_poly, bool param_1, mVec3_c *i_pos, mAng3_c *i_angle, mAng3_c *i_shapeAngle,
        bool param_5, bool param_6
    );
    void
    MoveBgTransPos(cBgS_PolyInfo const &i_poly, bool param_1, mVec3_c *i_pos, mAng3_c *i_angle, mAng3_c *i_shapeAngle);
    void MoveBgMatrixCrrPos(cBgS_PolyInfo const &, bool, mVec3_c *, mAng3_c *, mAng3_c *);
    void RideCallBack(cBgS_PolyInfo const &, fBase_c *);
    void ArrowStickCallBack(cBgS_PolyInfo const &, fBase_c *, mVec3_c &);
    fBase_c *PushPullCallBack(cBgS_PolyInfo const &, fBase_c *, s16, dBgW_Base::PushPullLabel);

    bool WaterChk(dBgS_SplGrpChk *chk) {
        return SplGrpChk(chk);
    }
    u32 GetMtrlSndId(const cBgS_PolyInfo &param_0) {
        return dKy_pol_sound_get(&param_0);
    }

    static dBgS &GetInstance();
};

bool dBgS_CheckBGroundPoly(cBgS_PolyInfo const &);
bool dBgS_CheckBRoofPoly(cBgS_PolyInfo const &);
bool dBgS_CheckBWallPoly(cBgS_PolyInfo const &);
void dBgS_MoveBGProc_Typical(
    dBgW *param_0, void *param_1, cBgS_PolyInfo const &param_2, bool param_3, mVec3_c *param_4, mAng3_c *param_5,
    mAng3_c *param_6
);
void dBgS_MoveBGProc_TypicalRotY(
    dBgW *param_0, void *param_1, cBgS_PolyInfo const &param_2, bool param_3, mVec3_c *param_4, mAng3_c *param_5,
    mAng3_c *param_6
);
void dBgS_MoveBGProc_Trans(
    dBgW *i_bgw, void *i_actor_ptr, cBgS_PolyInfo const &i_poly, bool param_3, mVec3_c *i_pos, mAng3_c *i_angle,
    mAng3_c *i_shapeAngle
);
f32 dBgS_GetNY(cBgS_PolyInfo const &poly);

#endif /* D_BG_D_BG_S_H */
