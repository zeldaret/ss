#ifndef D_BG_W_H
#define D_BG_W_H

#include <common.h>
#include <d/col/bg/d_bg_plc.h>
#include <d/col/bg/d_bg_w_base.h>
#include <d/col/c/c_m3d_g_aab.h>
#include <m/m_math.h>
#include <rvl/MTX.h>

class cBgS_GrpPassChk;
class cBgS_PolyPassChk;
class fBase_c;
struct cBgD_Vtx_t;
struct dBgS_CaptPoly;

class cBgW_TriElm {
public:
    /* 0x00 */ cM3dGPla m_plane;

    /*  */ cBgW_TriElm();
    /*  */ virtual ~cBgW_TriElm();
    /* 0x14  vtable */
}; // Size: 0x18

class cBgW_RwgElm {
public:
    /* 0x0 */ u16 m_next;

    /*  */ cBgW_RwgElm();
    /*  */ virtual ~cBgW_RwgElm();
}; // Size: 0x8

class cBgW_NodeTree : public cM3dGAab {
public:
    /*  */ cBgW_NodeTree();
    /*  */ virtual ~cBgW_NodeTree();
};

class cBgW_GrpElm {
public:
    /*  */ cBgW_GrpElm();
    /*  */ virtual ~cBgW_GrpElm();
    /* 0x4 */ cM3dGAab m_aab;
};

struct cBgW_BlkElm {
    /* 0x0 */ u16 m_roof_idx;
    /* 0x2 */ u16 m_wall_idx;
    /* 0x4 */ u16 m_gnd_idx;
}; // Size: 0x6

struct cBgD_Tri_t {
    /* 0x0 */ u16 m_vtx_idx0;
    /* 0x2 */ u16 m_vtx_idx1;
    /* 0x4 */ u16 m_vtx_idx2;
    /* 0x6 */ u16 m_id;
    /* 0x8 */ u16 m_grp;
}; // Size: 0xA

struct cBgD_Ti_t {
    /* 0x0 */ u32 m_info0;
    /* 0x4 */ u32 m_info1;
    /* 0x8 */ u32 m_info2;
    /* 0xC */ u32 m_passFlag;
};

struct cBgD_Blk_t {
    /* 0x0 */ u16 field_0x0;
};

struct cBgD_Tree_t {
    /* 0x0 */ u16 m_flag;
    /* 0x2 */ u16 m_parent_id;
    /* 0x4 */ u16 m_id[8];
}; // Size: 0x14

struct cBgD_Grp_t {
    /* 0x00 */ char *m_name;
    /* 0x04 */ mVec3_c m_scale;
    /* 0x10 */ mAng3_c m_rotation;
    /* 0x18 */ mVec3_c m_translation;
    /* 0x24 */ u16 m_parent;
    /* 0x26 */ u16 m_next_sibling;
    /* 0x28 */ u16 m_first_child;
    /* 0x2A */ u16 m_room_id;
    /* 0x2C */ u16 m_first_vtx_idx;
    /* 0x2E */ u16 m_tree_idx;
    /* 0x30 */ u32 m_info;
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
    };

    cBgW();
    void FreeArea();
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
    void UpdateMtx();
    bool Set(cBgD_t *pDzb, dBgPc *pPlc, u32 flags, mMtx_c *pMdlMtx, mVec3_c *pScale);
    bool RwgLineCheck(u16, cBgS_LinChk *);
    bool LineCheckRp(cBgS_LinChk *, int);
    bool LineCheckGrpRp(cBgS_LinChk *, int, int);
    bool RwgGroundCheckCommon(f32, u16, cBgS_GndChk *);
    bool RwgGroundCheckGnd(u16, cBgS_GndChk *);
    bool RwgGroundCheckWall(u16, cBgS_GndChk *);
    bool GroundCrossRp(cBgS_GndChk *, int);
    bool GroundCrossGrpRp(cBgS_GndChk *, int, int);
    void Lock();
    void CopyOldMtx();
    void Move();
    void RwgShdwDraw(int, cBgS_ShdwDraw *);
    void ShdwDrawRp(cBgS_ShdwDraw *, int);
    void ShdwDrawGrpRp(cBgS_ShdwDraw *, int);
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
    /* 0x014 */ virtual bool GetTopUnder(f32 *pOutTop, f32 *pOutUnder) const override; // ?
    /* 0x018 */ virtual bool ChkNotReady() const override;
    /* 0x01C */ virtual bool ChkLock() const override;
    /* 0x020 */ virtual bool ChkMoveBg() const override;
    /* 0x024 */ virtual u32 ChkMoveFlag() const override = 0;
    /* 0x028 */ virtual void GetTriPla(cBgS_PolyInfo const &, cM3dGPla *) const override;
    /* 0x02C */ virtual bool GetTriPnt(cBgS_PolyInfo const &, mVec3_c *, mVec3_c *, mVec3_c *) const override;
    /* 0x030 */ virtual const cM3dGAab *GetBnd() const override;
    /* 0x034 */ virtual u32 GetGrpInf(cBgS_PolyInfo const &) const override; // ?
    /* 0x038 */ virtual void OffMoveFlag() override = 0;
    /* 0x03C */ virtual void vt_0x3C() override = 0;
    /* 0x040 */ virtual bool LineCheck(cBgS_LinChk *) override; // ?
    /* 0x044 */ virtual bool GroundCross(cBgS_GndChk *) override;
    /* 0x048 */ virtual void ShdwDraw(cBgS_ShdwDraw *) override;
    /* 0x04C */ virtual void CaptPoly(dBgS_CaptPoly &) override = 0;
    /* 0x050 */ virtual bool WallCorrect(dBgS_Acch *) override = 0;      // ?
    /* 0x054 */ virtual bool WallCorrectSort(dBgS_Acch *) override = 0;  // ?
    /* 0x058 */ virtual bool RoofChk(dBgS_RoofChk *) override = 0;       // ?
    /* 0x05C */ virtual bool SplGrpChk(dBgS_SplGrpChk *) override = 0;   // ?
    /* 0x060 */ virtual bool SphChk(dBgS_SphChk *, void *) override = 0; // ?
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
    /* 0x0A0 */ virtual bool GetShdwThrough(int) override = 0;
    /* 0x0A4 */ virtual u32 GetUnderwaterRoofCode(int) override = 0;
    /* 0x0A8 */ virtual int GetCode0_0x80000000(cBgS_PolyInfo const &) override = 0;
    /* 0x0AC */ virtual int GetLinkNo(cBgS_PolyInfo const &) override = 0;
    /* 0x0B0 */ virtual s32 GetWallCode(cBgS_PolyInfo const &) override = 0;
    /* 0x0B4 */ virtual int GetPoltAtt0Material(cBgS_PolyInfo const &) override = 0;
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

    // u32 GetOldInvMtx(Mtx m) { return MTXInverse(m_inv_mtx, m); }
    // MtxP GetBaseMtxP() { return pm_base; }
    bool ChkNoCalcVtx() {
        return mFlags & NO_CALC_VTX_e;
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
    /* 0x1F0 */ dBgPlc *mpPolyCodes;
    /* 0x1F4 */ bool mNeedsFullTransform;
};

class dBgW;
class dAcObjBase_c;
typedef void (*dBgW_RideCallback)(dBgW *, dAcBase_c *, dAcObjBase_c *);
typedef void (*dBgW_ArrowStickCallback)(dBgW *, fBase_c *, fBase_c *);
typedef void (*dBgW_UnkCallback)(dBgW *, fBase_c *, fBase_c *);
typedef void (*dBgW_CrrFunc)(dBgW *, void *, cBgS_PolyInfo const &, bool, mVec3_c *, mAng3_c *, mAng3_c *);

class dBgW : public cBgW {
public:
    /* */ dBgW();
    /* */ void Move();
    /* */ void positionWallCorrect(dBgS_Acch *, f32, cM3dGPla &, mAng3_c *pUpperPos, f32);
    /* */ bool RwgWallCorrect(dBgS_Acch *, u16);
    /* */ bool WallCorrectRp(dBgS_Acch *, int);
    /* */ bool WallCorrectGrpRp(dBgS_Acch *, int, int);
    /* */ void RwgWallCorrectSort(dBgS_Acch *, u16);
    /* */ void WallCorrectRpSort(dBgS_Acch *, int);
    /* */ bool WallCorrectGrpRpSort(dBgS_Acch *, int, int);
    /* */ bool RwgRoofChk(u16, dBgS_RoofChk *);
    /* */ bool RoofChkRp(dBgS_RoofChk *, int);
    /* */ bool RoofChkGrpRp(dBgS_RoofChk *, int, int);
    /* */ bool RwgSplGrpChk(u16, dBgS_SplGrpChk *);
    /* */ bool SplGrpChkRp(dBgS_SplGrpChk *, int);
    /* */ bool SplGrpChkGrpRp(dBgS_SplGrpChk *, int, int);
    /* */ void RwgCaptPoly(int, dBgS_CaptPoly &);
    /* */ void CaptPolyRp(dBgS_CaptPoly &, int);
    /* */ void CaptPolyGrpRp(dBgS_CaptPoly &, int, int);
    /* */ bool RwgSphChk(u16, dBgS_SphChk *, void *);
    /* */ bool SphChkRp(dBgS_SphChk *, void *, int);
    /* */ bool SphChkGrpRp(dBgS_SphChk *, void *, int, int);

    /* vt at 0x08 */
    /* 0x008 */ virtual ~dBgW();
    /* 0x020 */ virtual bool ChkMoveBg() const override;
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
    /* 0x0A0 */ virtual bool GetShdwThrough(int) override;
    /* 0x0A4 */ virtual u32 GetUnderwaterRoofCode(int) override;
    /* 0x0A8 */ virtual int GetCode0_0x80000000(cBgS_PolyInfo const &) override;
    /* 0x0AC */ virtual int GetLinkNo(cBgS_PolyInfo const &) override;
    /* 0x0B0 */ virtual s32 GetWallCode(cBgS_PolyInfo const &) override;
    /* 0x0B4 */ virtual int GetPoltAtt0Material(cBgS_PolyInfo const &) override;
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
    /* 0x0F0 */ virtual void CallRideCallback(dAcBase_c *, dAcBase_c *) override;
    /* 0x0F4 */ virtual void CallArrowStickCallback(dAcBase_c *, dAcBase_c *) override;
    /* 0x0F8 */ virtual bool CallUnkCallback(dAcBase_c *, dAcBase_c *) override;
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
};

#endif /* D_BG_D_BG_W_H */
