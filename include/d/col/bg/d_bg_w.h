#ifndef D_BG_W_H
#define D_BG_W_H

#include "common.h"
#include "d/col/bg/d_bg_plc.h"
#include "d/col/bg/d_bg_w_base.h"
#include "d/col/c/c_m3d_g_aab.h"
#include "m/m_math.h" // IWYU pragma: export
#include "rvl/MTX.h"  // IWYU pragma: export


class cBgS_GrpPassChk;
class cBgS_PolyPassChk;
class fBase_c;
struct cBgD_Vtx_t;
struct dBgS_CaptPoly;

class cBgW_TriElm : public cM3dGPla {
public:
};

struct _RwgElm_t {
    u16 mNext;
};

class cBgW_RwgElm : public _RwgElm_t {
public:
    cBgW_RwgElm() {}
};

class cBgW_NodeTree : public cM3dGAab {};

class cBgW_GrpElm {
public:
    cM3dGAab mAab;
};

// Odd
struct _BlkElm_t {
    /* 0x0 */ u16 mRoofIdx;
    /* 0x2 */ u16 mWallIdx;
    /* 0x4 */ u16 mGndIdx;
};

class cBgW_BlkElm : public _BlkElm_t {
public:
    cBgW_BlkElm() {}
}; // Size: 0x6

struct cBgD_Tri_t {
    /* 0x0 */ u16 mVtxIdx0;
    /* 0x2 */ u16 mVtxIdx1;
    /* 0x4 */ u16 mVtxIdx2;
    /* 0x6 */ u16 mId;
    /* 0x8 */ u16 mGrp;
}; // Size: 0xA

struct cBgD_Ti_t {
    /* 0x0 */ u32 m_info0;
    /* 0x4 */ u32 m_info1;
    /* 0x8 */ u32 m_info2;
    /* 0xC */ u32 m_passFlag;
};

class cBgD_Blk_t {
public:
    /* 0x0 */ u16 field_0x0;
    cBgD_Blk_t();
};

struct cBgD_Tree_t {
    /* 0x0 */ u16 mFlag;
    /* 0x2 */ u16 m_parent_id;
    /* 0x4 */ u16 mId[8];

}; // Size: 0x14

struct cBgD_Grp_t {
    /* 0x00 */ char *mpName;
    /* 0x04 */ mVec3_c mScale;
    /* 0x10 */ mAng3_c mRotation;
    /* 0x18 */ mVec3_c mTranslation;
    /* 0x24 */ u16 mParent;
    /* 0x26 */ u16 mNextSibling;
    /* 0x28 */ u16 mFirstChild;
    /* 0x2A */ u16 mRoomId;
    /* 0x2C */ u16 mFirstVtxIdx;
    /* 0x2E */ u16 mTreeIdx;
    /* 0x30 */ u32 mInfo;
}; // Size: 0x34

struct cBgD_t {
    /* 0x00 */ int mVtxNum;         // vertex num
    /* 0x04 */ cBgD_Vtx_t *mVtxTbl; // vertex table
    /* 0x08 */ int mTriNum;         // triangle num
    /* 0x0C */ cBgD_Tri_t *mTriTbl; // triangle table
    /* 0x10 */ int mBlkNum;
    /* 0x14 */ cBgD_Blk_t *mBlkTbl;
    /* 0x18 */ int mTreeNum;
    /* 0x1C */ cBgD_Tree_t *mTreeTbl;
    /* 0x20 */ int mGrpNum;
    /* 0x24 */ cBgD_Grp_t *mGrpTbl;
    /* 0x28 */ int mTiNum;
    /* 0x2C */ cBgD_Ti_t *mTiTbl;
    /* 0x30 */ u32 mFlags;
};

class cBgW : public dBgW_Base {
public:
    enum Flags_e {
        MOVE_BG_e = 0x1,
        NO_CALC_VTX_e = 0x2,
        NO_VTX_TBL_e = 0x10,
        GLOBAL_e = 0x20,
        CBGW_UNK_FLAG_40 = 0x40,
        LOCK_e = 0x80,
        ROOF_REGIST_e = 0x200,
    };

    cBgW();
    void FreeArea();

    u32 GetOldInvMtx(mMtx_c *m);
    void UpdateMtx();
    void GlobalVtx();
    bool SetVtx();
    bool SetTri();

    void BlckConnect(u16 *, int *, int);
    void MakeBlckTransMinMax(mVec3_c *, mVec3_c *);
    void MakeBlckMinMax(int, mVec3_c *, mVec3_c *);
    void MakeBlckBnd(int, mVec3_c *, mVec3_c *);

    void MakeNodeTreeRp(int);
    void MakeNodeTreeGrpRp(int);
    void MakeNodeTree();

    bool Set(cBgD_t *pDzb, PLC *pPlc, u32 flags, mMtx_c *pMdlMtx, mVec3_c *pScale);

    void LineCheckGrpRp(cBgS_LinChk *, int);
    void LineCheckRp(cBgS_LinChk *, int);
    void RwgLineCheck(int, cBgS_LinChk *);

    void GroundCrossGrpRp(cBgS_GndChk *, int);
    void GroundCrossRp(cBgS_GndChk *, int);
    void RwgGroundCheckCommon(f32, u16, cBgS_GndChk *) {
        // Is Inlined
    }
    void RwgGroundCheckGnd(u16, cBgS_GndChk *);
    void RwgGroundCheckWall(u16, cBgS_GndChk *);

    void Lock();
    void CopyOldMtx();
    void Move();

    void ShdwDrawGrpRp(cBgS_ShdwDraw *, int);
    void ShdwDrawRp(cBgS_ShdwDraw *, int);
    void RwgShdwDraw(int, cBgS_ShdwDraw *);

    void GetTriPla(int idx, cM3dGPla *);

    void GetTrans(mVec3_c *) const;
    int GetPolyInfId(int) const;
    u32 GetPolyInf0(int, u32, u32) const;
    u32 GetMaskPolyInf0_NoShift(int, u32) const;
    u32 GetPolyInf1(int, u32, u32) const;
    u32 GetMaskPolyInf1_NoShift(int, u32) const;
    u32 GetPolyInf2(int, u32, u32) const;
    int GetTriGrp(int) const;

    /* vt at 0x08 */
    /* 0x008 */ virtual ~cBgW();
    /* 0x00C */ virtual bool ChkMemoryError() override;
    /* 0x014 */ virtual void GetTopUnder(f32 *pOutTop, f32 *pOutUnder) const override;
    /* 0x018 */ virtual bool ChkNotReady() const override;
    /* 0x01C */ virtual bool ChkLock() const override;
    /* 0x020 */ virtual bool ChkMoveBg() const override;
    /* 0x024 */ virtual u32 ChkMoveFlag() const override = 0;
    /* 0x028 */ virtual void GetTriPla(cBgS_PolyInfo const &, cM3dGPla *) const override;
    /* 0x02C */ virtual bool GetTriPnt(cBgS_PolyInfo const &, mVec3_c *, mVec3_c *, mVec3_c *) const override;
    /* 0x030 */ virtual const cM3dGAab *GetBnd() const override;
    /* 0x034 */ virtual u32 GetGrpInf(cBgS_PolyInfo const &) const override;
    /* 0x038 */ virtual void OffMoveFlag() override = 0;
    /* 0x03C */ virtual void vt_0x3C() override = 0;
    /* 0x040 */ virtual bool LineCheck(cBgS_LinChk *) override;
    /* 0x044 */ virtual bool GroundCross(cBgS_GndChk *) override;
    /* 0x048 */ virtual void ShdwDraw(cBgS_ShdwDraw *) override;
    /* 0x04C */ virtual void CaptPoly(dBgS_CaptPoly &) override = 0;
    /* 0x050 */ virtual bool WallCorrect(dBgS_Acch *) override = 0;
    /* 0x054 */ virtual bool WallCorrectSort(dBgS_Acch *) override = 0;
    /* 0x058 */ virtual bool RoofChk(dBgS_RoofChk *) override = 0;
    /* 0x05C */ virtual bool SplGrpChk(dBgS_SplGrpChk *) override = 0;
    /* 0x060 */ virtual bool SphChk(dBgS_SphChk *, void *) override = 0;
    /* 0x064 */ virtual s32 GetGrpRoomIndex(cBgS_PolyInfo const &) const override = 0;
    /* 0x068 */ virtual s32 GetExitId(cBgS_PolyInfo const &) override = 0;
    /* 0x06C */ virtual s32 GetZTargetThrough(cBgS_PolyInfo const &) override = 0;
    /* 0x070 */ virtual int GetSpecialCode(cBgS_PolyInfo const &) override = 0;
    /* 0x074 */ virtual int GetSpecialCode(int) override = 0;
    /* 0x078 */ virtual int GetCode0_0x30000000(cBgS_PolyInfo const &) override = 0;
    /* 0x07C */ virtual u32 GetPolyObjThrough(int) override = 0;
    /* 0x080 */ virtual u32 GetPolyCamThrough(int) override = 0;
    /* 0x084 */ virtual u32 GetPolyLinkThrough(int) override = 0;
    /* 0x088 */ virtual u32 GetPolyArrowThrough(int) override = 0;
    /* 0x08C */ virtual u32 GetPolySlingshotThrough(int) override = 0;
    /* 0x090 */ virtual u32 GetPolyBeetleThrough(int) override = 0;
    /* 0x094 */ virtual u32 GetPolyClawshotThrough(int) override = 0;
    /* 0x098 */ virtual u32 GetPolyBombThrough(int) override = 0;
    /* 0x09C */ virtual u32 GetPolyWhipThrough(int) override = 0;
    /* 0x0A0 */ virtual u32 GetShdwThrough(int) override = 0;
    /* 0x0A4 */ virtual u32 GetUnderwaterRoofCode(int) override = 0;
    /* 0x0A8 */ virtual int GetCode0_0x80000000(cBgS_PolyInfo const &) override = 0;
    /* 0x0AC */ virtual int GetLinkNo(cBgS_PolyInfo const &) override = 0;
    /* 0x0B0 */ virtual s32 GetWallCode(cBgS_PolyInfo const &) override = 0;
    /* 0x0B4 */ virtual int GetPolyAtt0Material(cBgS_PolyInfo const &) override = 0;
    /* 0x0B8 */ virtual int GetPolyAtt0(cBgS_PolyInfo const &) override;
    /* 0x0Bc */ virtual int GetPolyAtt1(cBgS_PolyInfo const &) override = 0;
    /* 0x0C0 */ virtual int GetGroundCode(cBgS_PolyInfo const &) override = 0;
    /* 0x0C4 */ virtual u32 GetCode1_0x02000000(int) override = 0;
    /* 0x0C8 */ virtual u32 GetCode1_0x04000000(int) override = 0;
    /* 0x0CC */ virtual u32 GetCode1_0x08000000(int) override = 0;
    /* 0x0D0 */ virtual u32 GetLightingCode(cBgS_PolyInfo const &) override = 0;
    /* 0x0D4 */ virtual s32 GetCamMoveBG(cBgS_PolyInfo const &) override = 0;
    /* 0x0D8 */ virtual s32 GetRoomCamId(cBgS_PolyInfo const &) override = 0;
    /* 0x0DC */ virtual s32 GetRoomPathId(cBgS_PolyInfo const &) override = 0;
    /* 0x0E0 */ virtual s32 GetRoomPathPntNo(cBgS_PolyInfo const &) override = 0;
    /* 0x0E4 */ virtual void CrrPos(cBgS_PolyInfo const &, void *, bool, mVec3_c *, mAng3_c *, mAng3_c *) override = 0;
    /* 0x0E8 */ virtual void
    TransPos(cBgS_PolyInfo const &, void *, bool, mVec3_c *, mAng3_c *, mAng3_c *) override = 0;
    /* 0x0EC */ virtual void
    MatrixCrrPos(cBgS_PolyInfo const &, void *, bool, mVec3_c *, mAng3_c *, mAng3_c *) override = 0;
    /* 0x100 */ virtual bool GetIsDraw(int) override = 0;
    /* 0x104 */ virtual void DrawOnMap(int, bool) override = 0;
    /* 0x108 */ virtual void CalcPlane();
    /* 0x10C */ virtual void ClassifyPlane();

    // MtxP GetBaseMtxP() { return pm_base; }

    bool ChkNoCalcVtx() {
        return mFlags & NO_CALC_VTX_e;
    }
    void SetNoCalcVtx() {
        mFlags |= NO_CALC_VTX_e;
    }
    void ClrNoCalcVtx() {
        mFlags &= ~NO_CALC_VTX_e;
    }
    void SetLock() {
        mFlags |= LOCK_e;
    }
    cBgD_Vtx_t *GetVtxTbl() const {
        return mpVtxTbl;
    }
    int GetVtxNum() const {
        return mpBgd->mVtxNum;
    }
    bool ChkRoofRegist() {
        return mFlags & ROOF_REGIST_e;
    }

public:
    /* 0x12C */ mMtx_c *mpMdlMtx; // Model Matrix
    /* 0x130 */ mMtx_c mInvMtx;
    /* 0x160 */ mMtx_c mMtx;
    /* 0x190 */ mMtx_c mMtxUnk;
    /* 0x1C0 */ mVec3_c mTransVel;
    /* 0x1CC */ mVec3_c *mpScale;
    /* 0x1D0 */ u16 mFlags;
    /* 0x1D2 */ u16 mRootGroupIdx;
    /* 0x1D4 */ cBgW_TriElm *mpTri;
    /* 0x1D8 */ cBgW_RwgElm *mpRwg;
    /* 0x1DC */ cBgD_Vtx_t *mpVtxTbl;
    /* 0x1E0 */ cBgD_t *mpBgd;
    /* 0x1E4 */ cBgW_BlkElm *mpBlk;
    /* 0x1E8 */ cBgW_GrpElm *mpGrp;
    /* 0x1EC */ cBgW_NodeTree *mpNodeTree;
    /* 0x1F0 */ dBgPlc mpPolyCodes;
    /* 0x1F4 */ bool mNeedsFullTransform;

    static bool sLineCheck;
    static bool sGndCheck;
    static bool sWallCheck;
    static bool sRoofCheck;
    static bool sSplGrpCheck;
    static bool sSphCheck;
};

class dBgW;
class dAcObjBase_c;
typedef void (*dBgW_RideCallback)(dBgW *, dAcObjBase_c *, dAcObjBase_c *);
typedef void (*dBgW_ArrowStickCallback)(dBgW *, dAcObjBase_c *, dAcObjBase_c *, mVec3_c &);
typedef bool (*dBgW_UnkCallback)(dBgW *, dAcObjBase_c *, dAcObjBase_c *);
typedef void (*dBgW_CrrFunc)(dBgW *, void *, cBgS_PolyInfo const &, bool, mVec3_c *, mAng3_c *, mAng3_c *);

class dBgW : public cBgW {
public:
    dBgW();
    void Move();
    void positionWallCorrect(dBgS_Acch *, f32, cM3dGPla &, mVec3_c *pUpperPos, f32);

    void RwgWallCorrect(dBgS_Acch *, u16);
    void WallCorrectRp(dBgS_Acch *, int);
    void WallCorrectGrpRp(dBgS_Acch *, int);

    void RwgWallCorrectSort(dBgS_Acch *, u16);
    void WallCorrectRpSort(dBgS_Acch *, int);
    void WallCorrectGrpRpSort(dBgS_Acch *, int);

    void RwgRoofChk(u16, dBgS_RoofChk *, f32);
    void RwgRoofChkRoof(u16, dBgS_RoofChk *);
    void RwgRoofChkWall(u16, dBgS_RoofChk *);
    void RoofChkRp(dBgS_RoofChk *, int);
    void RoofChkGrpRp(dBgS_RoofChk *, int);

    void RwgSplGrpChk(u16, dBgS_SplGrpChk *);
    void SplGrpChkRp(dBgS_SplGrpChk *, int);
    void SplGrpChkGrpRp(dBgS_SplGrpChk *, int);

    void RwgCaptPoly(u16, dBgS_CaptPoly &);
    void CaptPolyRp(dBgS_CaptPoly &, int);
    void CaptPolyGrpRp(dBgS_CaptPoly &, int);

    void RwgSphChk(u16, dBgS_SphChk *, void *);
    void SphChkRp(dBgS_SphChk *, void *, int);
    void SphChkGrpRp(dBgS_SphChk *, void *, int);

    bool GetMapCode(int polyIdx, int *pOut);

    /* vt at 0x08 */
    /* 0x008 */ virtual ~dBgW();
    /* 0x024 */ virtual u32 ChkMoveFlag() const override;
    /* 0x038 */ virtual void OffMoveFlag() override;
    /* 0x03C */ virtual void vt_0x3C() override;
    /* 0x04C */ virtual void CaptPoly(dBgS_CaptPoly &) override;
    /* 0x050 */ virtual bool WallCorrect(dBgS_Acch *) override;
    /* 0x054 */ virtual bool WallCorrectSort(dBgS_Acch *) override;
    /* 0x058 */ virtual bool RoofChk(dBgS_RoofChk *) override;
    /* 0x05C */ virtual bool SplGrpChk(dBgS_SplGrpChk *) override;
    /* 0x060 */ virtual bool SphChk(dBgS_SphChk *, void *) override;
    /* 0x064 */ virtual s32 GetGrpRoomIndex(cBgS_PolyInfo const &) const override;
    /* 0x068 */ virtual s32 GetExitId(cBgS_PolyInfo const &) override;
    /* 0x06C */ virtual s32 GetZTargetThrough(cBgS_PolyInfo const &) override;
    /* 0x070 */ virtual int GetSpecialCode(cBgS_PolyInfo const &) override;
    /* 0x074 */ virtual int GetSpecialCode(int) override;
    /* 0x078 */ virtual int GetCode0_0x30000000(cBgS_PolyInfo const &) override;
    /* 0x07C */ virtual u32 GetPolyObjThrough(int) override;
    /* 0x080 */ virtual u32 GetPolyCamThrough(int) override;
    /* 0x084 */ virtual u32 GetPolyLinkThrough(int) override;
    /* 0x088 */ virtual u32 GetPolyArrowThrough(int) override;
    /* 0x08C */ virtual u32 GetPolySlingshotThrough(int) override;
    /* 0x090 */ virtual u32 GetPolyBeetleThrough(int) override;
    /* 0x094 */ virtual u32 GetPolyClawshotThrough(int) override;
    /* 0x098 */ virtual u32 GetPolyBombThrough(int) override;
    /* 0x09C */ virtual u32 GetPolyWhipThrough(int) override;
    /* 0x0A0 */ virtual u32 GetShdwThrough(int) override;
    /* 0x0A4 */ virtual u32 GetUnderwaterRoofCode(int) override;
    /* 0x0A8 */ virtual int GetCode0_0x80000000(cBgS_PolyInfo const &) override;
    /* 0x0AC */ virtual int GetLinkNo(cBgS_PolyInfo const &) override;
    /* 0x0B0 */ virtual s32 GetWallCode(cBgS_PolyInfo const &) override;
    /* 0x0B4 */ virtual int GetPolyAtt0Material(cBgS_PolyInfo const &) override;
    /* 0x0B8 */ virtual int GetPolyAtt0(cBgS_PolyInfo const &) override;
    /* 0x0Bc */ virtual int GetPolyAtt1(cBgS_PolyInfo const &) override;
    /* 0x0C0 */ virtual int GetGroundCode(cBgS_PolyInfo const &) override;
    /* 0x0C4 */ virtual u32 GetCode1_0x02000000(int) override;
    /* 0x0C8 */ virtual u32 GetCode1_0x04000000(int) override;
    /* 0x0CC */ virtual u32 GetCode1_0x08000000(int) override;
    /* 0x0D0 */ virtual u32 GetLightingCode(cBgS_PolyInfo const &) override;
    /* 0x0D4 */ virtual s32 GetCamMoveBG(cBgS_PolyInfo const &) override;
    /* 0x0D8 */ virtual s32 GetRoomCamId(cBgS_PolyInfo const &) override;
    /* 0x0DC */ virtual s32 GetRoomPathId(cBgS_PolyInfo const &) override;
    /* 0x0E0 */ virtual s32 GetRoomPathPntNo(cBgS_PolyInfo const &) override;
    /* 0x0E4 */ virtual void CrrPos(cBgS_PolyInfo const &, void *, bool, mVec3_c *, mAng3_c *, mAng3_c *) override;
    /* 0x0E8 */ virtual void TransPos(cBgS_PolyInfo const &, void *, bool, mVec3_c *, mAng3_c *, mAng3_c *) override;
    /* 0x0EC */ virtual void
    MatrixCrrPos(cBgS_PolyInfo const &, void *, bool, mVec3_c *, mAng3_c *, mAng3_c *) override;
    /* 0x0F0 */ virtual void CallRideCallback(dAcObjBase_c *, dAcObjBase_c *) override;
    /* 0x0F4 */ virtual void CallArrowStickCallback(dAcObjBase_c *, dAcObjBase_c *, mVec3_c &) override;
    /* 0x0F8 */ virtual bool CallUnkCallback(dAcObjBase_c *, dAcObjBase_c *) override;
    /* 0x0FC */ virtual bool UpdateDraw(mAllocator_c *) override;
    /* 0x100 */ virtual bool GetIsDraw(int) override;
    /* 0x104 */ virtual void DrawOnMap(int, bool) override;
    /* 0x110 */ virtual u32 GetWallAtt(int);

    void SetCrrFunc(dBgW_CrrFunc func) {
        mpCrrFunc = func;
    }
    void SetRideCallback(dBgW_RideCallback func) {
        mpRide_cb = func;
    }
    void OnMoveFlag() {
        mFlags |= 1;
    }

private:
    /* 0x1F8 */ u32 mField_0x1F8;
    /* 0x1FC */ dBgW_CrrFunc mpCrrFunc;
    /* 0x200 */ dBgW_RideCallback mpRide_cb;
    /* 0x204 */ dBgW_ArrowStickCallback mpArrowStick_cb;
    /* 0x208 */ dBgW_UnkCallback mpUnk_cb;
    /* 0x20C */ u8 mFlags;

    static mVec3_c sWallCorrectPos;
};

#endif
