#ifndef D_BG_W_KCOL_H
#define D_BG_W_KCOL_H

#include "d/col/bg/d_bg_plc.h"
#include "d/col/bg/d_bg_w_base.h"
#include "d/col/c/c_bg_s_chk.h"
#include "d/col/c/c_m3d_g_aab.h"

class cBgS_GrpPassChk;
class cBgS_PolyPassChk;
struct dBgPc;
struct dBgS_CaptPoly;

struct KC_PrismData {
    /* 0x0 */ f32 mHeight;
    /* 0x4 */ u16 mPosIdx;
    /* 0x6 */ u16 mFaceNrmIdx;
    /* 0x8 */ u16 mEdgeNrmIdx1;
    /* 0xA */ u16 mEdgeNrmIdx2;
    /* 0xC */ u16 mEdgeNrmIdx3;
    /* 0xE */ u16 mAttribute;
}; // Size: 0x10

struct pkcdata {
    /* 0x00 */ mVec3_c *mpPositionData;
    /* 0x04 */ mVec3_c *mpNormalData;
    /* 0x08 */ KC_PrismData *mpPrismData;
    /* 0x0C */ u32 *mpBlockData;
    /* 0x10 */ u8 field_0x10[4];
    /* 0x14 */ mVec3_c mAreaMinPos;
    /* 0x20 */ u32 mAreaWidthMaskX;
    /* 0x24 */ u32 mAreaWidthMaskY;
    /* 0x28 */ u32 mAreaWidthMaskZ;
    /* 0x2C */ u32 mShiftBlockWidth;
    /* 0x30 */ u32 mShiftY;
    /* 0x34 */ u32 mShiftZ;
};

struct KCol_Header {
    /* 0x0 */ u32 pos_data_offset;
    /* 0x4 */ u32 nrm_data_offset;
    /* 0x8 */ u32 prism_data_offset;
    /* 0xC */ u32 block_data_offset;
};

class dBgWKCol : public dBgW_Base {
public:
    dBgWKCol();
    static void initKCollision(void *);
    void Set(void *pprism, void *plc);
    void GetTriNrm(KC_PrismData *, mVec3_c **) const;
    cM3dGPla GetTriPla(int) const;
    bool GetTriPnt(KC_PrismData const *, mVec3_c *, mVec3_c *, mVec3_c *) const;
    bool GetTriPnt(int, mVec3_c *, mVec3_c *, mVec3_c *) const;
    void GetPolyCode(int, dBgPc *) const;
    static bool ChkPolyThrough(int id, dBgPc *, cBgS_Chk *);
    bool ChkShdwDrawThrough(dBgPc *);

    /* vt at 0x08 */
    /* 0x008 */ virtual ~dBgWKCol() override;
    /* 0x014 */ virtual bool GetTopUnder(f32 *pOutTop, f32 *pOutUnder) const override;
    /* 0x018 */ virtual bool ChkNotReady() const override;
    /* 0x01C */ virtual bool ChkLock() const override;
    /* 0x020 */ virtual bool ChkMoveBg() const override;
    /* 0x024 */ virtual u32 ChkMoveFlag() const override;
    /* 0x028 */ virtual void GetTriPla(cBgS_PolyInfo const &, cM3dGPla *) const override;
    /* 0x02C */ virtual bool GetTriPnt(cBgS_PolyInfo const &, mVec3_c *, mVec3_c *, mVec3_c *) const override;
    /* 0x030 */ virtual const cM3dGAab *GetBnd() const override;
    /* 0x034 */ virtual u32 GetGrpInf(cBgS_PolyInfo const &) const override;
    /* 0x038 */ virtual void OffMoveFlag() override;
    /* 0x03C */ virtual void vt_0x3C() override;
    /* 0x040 */ virtual bool LineCheck(cBgS_LinChk *) override;
    /* 0x044 */ virtual bool GroundCross(cBgS_GndChk *) override;
    /* 0x048 */ virtual void ShdwDraw(cBgS_ShdwDraw *) override;
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
    /* 0x0FC */ virtual bool UpdateDraw(mAllocator_c *) override;
    /* 0x100 */ virtual bool GetIsDraw(int) override;
    /* 0x104 */ virtual void DrawOnMap(int, bool) override;

    KC_PrismData *GetPrismData(int poly_index) const {
        return &mpKCHead->mpPrismData[poly_index];
    }
    mVec3_c *GetTriPos(const KC_PrismData *pd) const {
        return &mpKCHead->mpPositionData[pd->mPosIdx];
    }
    mVec3_c *GetTriNrm(const KC_PrismData *pd) const {
        return &mpKCHead->mpNormalData[pd->mFaceNrmIdx];
    }
    mVec3_c *GetEdgeNrm1(const KC_PrismData *pd) const {
        return &mpKCHead->mpNormalData[pd->mEdgeNrmIdx1];
    }
    mVec3_c *GetEdgeNrm2(const KC_PrismData *pd) const {
        return &mpKCHead->mpNormalData[pd->mEdgeNrmIdx2];
    }
    mVec3_c *GetEdgeNrm3(const KC_PrismData *pd) const {
        return &mpKCHead->mpNormalData[pd->mEdgeNrmIdx3];
    }

private:
    /* 0x12C */ pkcdata *mpKCHead;
    /* 0x130 */ dBgPlc mCode;
    /* 0x134 */ cM3dGAab mBnd;
    /* 0x14C */ u32 mNumPrism;
    /* 0x150 */ u32 mAreaWidthMaskX;
    /* 0x154 */ u32 mAreaWidthMaskY;
    /* 0x158 */ u32 mAreaWidthMaskZ;
    /* 0x15C */ u32 mField0x15C;
};

#endif /* D_BG_D_BG_W_KCOL_H */
