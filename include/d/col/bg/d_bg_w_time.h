#ifndef D_BG_W_TIME_H
#define D_BG_W_TIME_H

#include "d/col/bg/d_bg_w_base.h"
#include "d/col/c/c_m3d_g_cyl.h"

class dBgWTime : public dBgW_Base {
public:
    dBgWTime();

    /* vt at 0x08 */
    /* 0x008 */ virtual ~dBgWTime();
    /* 0x014 */ virtual bool GetTopUnder(f32 *pOutTop, f32 *pOutUnder) const override;
    /* 0x018 */ virtual bool ChkNotReady() const override;
    /* 0x01C */ virtual bool ChkLock() const override;
    /* 0x020 */ virtual bool ChkMoveBg() const override;
    /* 0x024 */ virtual u32 ChkMoveFlag() const override;
    /* 0x028 */ virtual void GetTriPla(cBgS_PolyInfo const &, cM3dGPla *) const;
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
    /* 0x100 */ virtual bool GetIsDraw(int) override;
    /* 0x104 */ virtual void DrawOnMap(int, bool) override;

    /* 0x12C */ cM3dGCyl mCyl;
    /* 0x140 */ cM3dGAab mBnd;
};

#endif
