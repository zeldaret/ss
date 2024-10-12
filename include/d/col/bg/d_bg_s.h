#ifndef D_BG_S_H
#define D_BG_S_H

#include "common.h"
#include "d/a/d_a_base.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/a/obj/d_a_obj_bg.h"
#include "d/col/bg/d_bg_w.h"
#include "d/col/bg/d_bg_w_base.h"
#include "d/col/bg/d_bg_w_kcol.h"
#include "egg/gfx/eggCpuTexture.h"
#include "toBeSorted/tlist.h"

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
    bool ReleaseKCol(dBgWKCol *);

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

struct dMapGradation {
    dMapGradation()
        : mHasDifferingGrad(false), mGradHigh(0.0f), mGradLow(0.0f), mHasGradation(false), mColorR(0), mColorG(0),
          mColorB(0), mColorA(0) {}
    /* 0x00 */ bool mHasDifferingGrad;
    /* 0x04 */ f32 mGradHigh;
    /* 0x08 */ f32 mGradLow;
    /* 0x0C */ bool mHasGradation;
    /* 0x0D */ u8 mColorR;
    /* 0x0E */ u8 mColorG;
    /* 0x0F */ u8 mColorB;
    /* 0x10 */ u8 mColorA;
};

struct MapLineSegment {
    MapLineSegment();
    virtual ~MapLineSegment();
    virtual void Draw(int, mMtx_c *, bool, int);

    void fn_8033e9a0();
    void fn_8033e8b0();
    void fn_8033e9c0();
    void Append();
    void Remove();

    /* 0x4 */ bool bShow;
    /* 0x8 */ TListNode mLink;
};

class dBgS : public cBgS {
private:
    static dBgS *spInstance;
    static const void *spSolidMatTex[31];
    static const void *spLiquidMatTex[5];
    typedef TList<MapLineSegment, offsetof(MapLineSegment, mLink)> MapLineList;

public:
    /* 0x2EF8 */ EGG::CpuTexture *mMapTexture;
    /* 0x2EFC */ dBgW_Base *mColllisionTbl[BG_ID_MAX];
    /* 0x385C */ s32 mColllisionTblLen;
    /* 0x3860 */ bool mInSkyKeep;
    /* 0x3861 */ u32 mUnk_0x3861[10];
    /* 0x388C */ MapLineList mList_0x388C;
    /* 0x3894 */ dMapGradation mMapGradation;

    dBgS();
    ~dBgS();
    void Ct();
    void Dt();
    void ClrMoveFlag();
    void Move();
    void Regist(dBgW_Base *, int);
    static dBgS *GetInstance();
    bool Regist(dBgW_Base *, dAcObjBase_c *);   // Registers Actor to Bg
    bool RegistBg(dBgW_Base *, dAcObjBase_c *); // Registers Bg
    bool UnRegist(dBgW_Base *);
    bool ChkMoveBG(cBgS_PolyInfo const &, bool);
    u32 ChkShadowThrough(cBgS_PolyInfo const &);
    int GetSpecialCode(cBgS_PolyInfo const &);
    s32 GetWallCode(cBgS_PolyInfo const &);
    // s32 GetExitId(cBgS_PolyInfo const &);
    static int GetMapCode(int, int, bool);
    int GetPolyMaterial(cBgS_PolyInfo const &);
    int GetPolyAtt0(cBgS_PolyInfo const &);
    int GetPolyAtt1(cBgS_PolyInfo const &);
    int GetLightingCode(cBgS_PolyInfo const &);
    int GetGroundCode(cBgS_PolyInfo const &);
    int GetCode1_0x02000000(cBgS_PolyInfo const &);
    s32 GetRoomCamId(cBgS_PolyInfo const &);
    s32 GetRoomId(cBgS_PolyInfo const &);

    bool GetPolyObjectThrough(cBgS_PolyInfo const &);
    bool GetPolyCameraThrough(cBgS_PolyInfo const &);
    bool GetPolyShadowThrough(cBgS_PolyInfo const &);
    bool GetPolyLinkThrough(cBgS_PolyInfo const &);
    bool GetPolyArrowThrough(cBgS_PolyInfo const &);
    bool GetPolyBombThrough(cBgS_PolyInfo const &);
    bool GetPolyBeetleThrough(cBgS_PolyInfo const &);
    bool GetPolyClawshotThrough(cBgS_PolyInfo const &);
    bool GetPolyThrough_Code1_0x04000000(cBgS_PolyInfo const &);
    bool GetPolyThrough_Code1_0x08000000(cBgS_PolyInfo const &);
    bool GetPolyWhipThrough(cBgS_PolyInfo const &);
    bool GetPolySlingshotThrough(cBgS_PolyInfo const &);

    void WallCorrect(dBgS_Acch *, bool);
    f32 RoofChk(dBgS_RoofChk *);
    bool SplGrpChk(dBgS_SplGrpChk *);
    bool SphChk(dBgS_SphChk *, void *);
    // void MoveBgCrrPos(
    //     cBgS_PolyInfo const &, bool param_1, mVec3_c *i_pos, mAng3_c *i_angle, mAng3_c *i_shapeAngle,
    //     bool param_5, bool param_6
    // );
    // MoveBgTransPos(cBgS_PolyInfo const &i_poly, bool param_1, mVec3_c *i_pos, mAng3_c *i_angle, mAng3_c
    // *i_shapeAngle)

    // u32 GetUnderwaterRoofCode(cBgS_PolyInfo const &);
    // s32 GetCamMoveBG(cBgS_PolyInfo const &);
    // s32 GetRoomPathId(cBgS_PolyInfo const &);
    // s32 GetRoomPathPntNo(cBgS_PolyInfo const &);
    // int GetGrpSoundId(cBgS_PolyInfo const &);
    // u32 ChkGrpInf(cBgS_PolyInfo const &, u32);
    // bool GetPolyAttackThrough(cBgS_PolyInfo const &);
    // u32 ChkPolyHSStick(cBgS_PolyInfo const &);
    // void WallCorrectSort(dBgS_Acch *);
    // void MoveBgMatrixCrrPos(cBgS_PolyInfo const &, bool, mVec3_c *, mAng3_c *, mAng3_c *);

    void MoveBgCrrPos(cBgS_PolyInfo const &, bool, mVec3_c *, mAng3_c *, mAng3_c *, bool, bool);
    void MoveBgTransPos(cBgS_PolyInfo const &, bool, mVec3_c *, mAng3_c *, mAng3_c *);

    void RideCallBack(cBgS_PolyInfo const &, dAcBase_c *);
    void ArrowStickCallBack(cBgS_PolyInfo const &, dAcBase_c *, mVec3_c &);
    void UnkCallback(cBgS_PolyInfo const &, dAcBase_c *);
    dAcBase_c *PushPullCallBack(cBgS_PolyInfo const &, dAcBase_c *, s16, dBgW_Base::PushPullLabel);

    void UpdateScrollTex();
    void SetupMapGX(mMtx_c *);
    void SetupMapMaterial(int matIdx, bool, s32 roomId);
    UNKTYPE *GetMapAccessor();
    void DrawMap(u8 roomId, mMtx_c *, bool bColor, int);
    void SetupScrollGX();
    void SetupScrollMaterial(int matIdx, s32, bool);
    void DrawMapScroll(u8 roomId, mMtx_c *, bool bColor, int);
    void DrawSkyKeepMap(mMtx_c *, int);
    bool ConfigureMapTexture(EGG::Heap *);

    void InitMapParts();
    void AppendMapSegment(MapLineSegment *);
    void RemoveMapSegment(MapLineSegment *);
    void DrawMapSegments(int, mMtx_c *, bool, int);
    void ClearMapSegments();

    void SetLightingCode(dAcObjBase_c *, const cBgS_GndChk &);
    void SetLightingCode(dAcObjBase_c *, f32);
    int GetLightingCode(const mVec3_c *);
    bool GetPolyPreventObjOnly(const cBgS_PolyInfo &);
    bool GetMapGradationColor(GXColor *);
};

void dBgS_MoveBGProc_Trans(
    dBgW *i_bgw, void *i_actor_ptr, cBgS_PolyInfo const &i_poly, bool param_3, mVec3_c *i_pos, mAng3_c *i_angle,
    mAng3_c *i_shapeAngle
);

void dBgS_MoveBGProc_Typical(
    dBgW *param_0, void *param_1, cBgS_PolyInfo const &param_2, bool param_3, mVec3_c *param_4, mAng3_c *param_5,
    mAng3_c *param_6
);

void dBgS_MoveBGProc_RotY(
    dBgW *param_0, void *param_1, cBgS_PolyInfo const &param_2, bool param_3, mVec3_c *param_4, mAng3_c *param_5,
    mAng3_c *param_6
);

void dBgS_MoveBGProc_TypicalRotY(
    dBgW *param_0, void *param_1, cBgS_PolyInfo const &param_2, bool param_3, mVec3_c *param_4, mAng3_c *param_5,
    mAng3_c *param_6
);

bool dBgS_CheckBWallPoly(cBgS_PolyInfo const &);
bool dBgS_CheckBGroundPoly(cBgS_PolyInfo const &);
bool dBgS_CheckBRoofPoly(cBgS_PolyInfo const &);
f32 dBgS_GetNY(cBgS_PolyInfo const &poly);
mVec3_c dBgS_GetN(cBgS_PolyInfo const &);

#endif /* D_BG_D_BG_S_H */
