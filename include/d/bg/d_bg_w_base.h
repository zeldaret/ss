#ifndef D_BG_D_BG_W_BASE_H
#define D_BG_D_BG_W_BASE_H

/**
 *  This File will really need some modification
 */

#include "c/c_bg_s_poly_info.h"
#include "c/c_bg_w.h"
#include "m/m_aabb.h"
#include "m/m_math.h"
#include "m/m_plane.h"

class cBgS_GndChk;
class cBgS_LinChk;
class cBgS_ShdwDraw;
class dBgS_Acch;
struct dBgS_CaptPoly;
class dBgS_RoofChk;
class dBgS_SphChk;
class dBgS_SplGrpChk;

class dBgW_Base : public cBgW_BgId {
public:
    enum PushPullLabel {
        PPLABEL_NONE = 0,
        PPLABEL_PUSH = 1,
        PPLABEL_PULL = 2,
        PPLABEL_4 = 4,
        PPLABEL_HEAVY = 8,
    };

    enum PRIORITY {
        PRIORITY_0,
    };

    // typedef fopAc_ac_c *(*PushPull_CallBack)(fopAc_ac_c *, fopAc_ac_c *, s16, dBgW_Base::PushPullLabel);

    dBgW_Base();
    void ClrDBgWBase();
    void CalcDiffShapeAngleY(s16);

    virtual ~dBgW_Base();
    virtual bool ChkMemoryError();
    virtual bool ChkNotReady() const = 0;
    virtual bool ChkLock() const = 0;
    virtual bool ChkMoveBg() const = 0;
    virtual u32 ChkMoveFlag() const = 0;
    virtual mPlane_c GetTriPla(cBgS_PolyInfo const &) const = 0;
    virtual bool GetTriPnt(cBgS_PolyInfo const &, mVec3_c *, mVec3_c *, mVec3_c *) const = 0;
    virtual mAABB_c *GetBnd() const = 0;
    virtual u32 GetGrpInf(cBgS_PolyInfo const &) const = 0;
    virtual void OffMoveFlag() = 0;
    virtual void GetTopUnder(f32 *, f32 *) const = 0;
    virtual void SetOldShapeAngleY(s16);
    virtual bool LineCheck(cBgS_LinChk *) = 0;
    virtual bool GroundCross(cBgS_GndChk *) = 0;
    virtual void ShdwDraw(cBgS_ShdwDraw *) = 0;
    virtual void CaptPoly(dBgS_CaptPoly &) = 0;
    virtual bool WallCorrect(dBgS_Acch *) = 0;
    virtual bool WallCorrectSort(dBgS_Acch *) = 0;
    virtual bool RoofChk(dBgS_RoofChk *) = 0;
    virtual bool SplGrpChk(dBgS_SplGrpChk *) = 0;
    virtual bool SphChk(dBgS_SphChk *, void *) = 0;
    virtual s32 GetGrpRoomIndex(cBgS_PolyInfo const &) const = 0;
    virtual s32 GetExitId(cBgS_PolyInfo const &) = 0;
    virtual s32 GetPolyColor(cBgS_PolyInfo const &) = 0;
    virtual BOOL GetHorseNoEntry(cBgS_PolyInfo const &) = 0;
    virtual int GetSpecialCode(cBgS_PolyInfo const &) = 0;
    virtual int GetSpecialCode(int) = 0;
    virtual int GetMagnetCode(cBgS_PolyInfo const &) = 0;
    virtual u32 GetPolyObjThrough(int) = 0;
    virtual u32 GetPolyCamThrough(int) = 0;
    virtual u32 GetPolyLinkThrough(int) = 0;
    virtual u32 GetPolyArrowThrough(int) = 0;
    virtual u32 GetPolyHSStick(int) = 0;
    virtual u32 GetPolyBoomerangThrough(int) = 0;
    virtual u32 GetPolyRopeThrough(int) = 0;
    virtual u32 GetPolyBombThrough(int) = 0;
    virtual bool GetShdwThrough(int) = 0;
    virtual u32 GetUnderwaterRoofCode(int) = 0;
    virtual int GetMonkeyBarsCode(cBgS_PolyInfo const &) = 0;
    virtual int GetLinkNo(cBgS_PolyInfo const &) = 0;
    virtual s32 GetWallCode(cBgS_PolyInfo const &) = 0;
    virtual int GetPolyAtt0(cBgS_PolyInfo const &) = 0;
    virtual int GetPolyAtt1(cBgS_PolyInfo const &) = 0;
    virtual int GetGroundCode(cBgS_PolyInfo const &) = 0;
    virtual u32 GetIronBallThrough(int) = 0;
    virtual u32 GetAttackThrough(int) = 0;
    virtual s32 GetCamMoveBG(cBgS_PolyInfo const &) = 0;
    virtual s32 GetRoomCamId(cBgS_PolyInfo const &) = 0;
    virtual s32 GetRoomPathId(cBgS_PolyInfo const &) = 0;
    virtual s32 GetRoomPathPntNo(cBgS_PolyInfo const &) = 0;
    virtual u8 GetPolyGrpRoomInfId(cBgS_PolyInfo const &) = 0;
    virtual int GetGrpSoundId(cBgS_PolyInfo const &) = 0;
    virtual void CrrPos(cBgS_PolyInfo const &, void *, bool, mVec3_c *, mAng3_c *, mAng3_c *) = 0;
    virtual void TransPos(cBgS_PolyInfo const &, void *, bool, mVec3_c *, mAng3_c *, mAng3_c *) = 0;
    virtual void MatrixCrrPos(cBgS_PolyInfo const &, void *, bool, mVec3_c *, mAng3_c *, mAng3_c *) = 0;
    // virtual void CallRideCallBack(fopAc_ac_c *, fopAc_ac_c *);
    // virtual void CallArrowStickCallBack(fopAc_ac_c *, fopAc_ac_c *, mVec3_c &);

    // PushPull_CallBack GetPushPullCallback() const { return m_pushPull_Callback; }
    s16 GetDiffShapeAngleY() { return m_diff_ShapeAngleY; }
    int GetRoomId() { return m_roomId; }
    bool chkStickWall() { return field_0xb & 1; }
    bool chkStickRoof() { return field_0xb & 2; }
    bool ChkPushPullOk() const { return m_pushPull_Ok; }
    // void SetPushPullCallback(PushPull_CallBack i_callBack) { m_pushPull_Callback = i_callBack; }
    void SetRoomId(int id) { m_roomId = id; }
    bool ChkPriority(int prio) { return m_priority == prio; }
    void SetPriority(PRIORITY priority) { m_priority = priority; }
    void onStickWall() { field_0xb |= 1; }
    void onStickRoof() { field_0xb |= 2; }
    void OnPushPullOk() { m_pushPull_Ok = true; }
    void OffPushPullOk() { m_pushPull_Ok = false; }

private:
    /* 0x08 */ u8 m_priority;
    /* 0x09 */ u8 m_roomId;
    /* 0x0A */ u8 field_0xa;
    /* 0x0B */ u8 field_0xb;
    /* 0x0C */ s16 m_old_ShapeAngleY;
    /* 0x0E */ s16 m_diff_ShapeAngleY;
    /* 0x14 */ bool m_pushPull_Ok;
}; // Size: 0x18

#endif
