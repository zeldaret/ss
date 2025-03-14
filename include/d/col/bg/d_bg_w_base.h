#ifndef D_BG_W_BASE_H
#define D_BG_W_BASE_H

#include "d/a/d_a_base.h"
#include "d/col/c/c_bg_s_poly_info.h"
#include "d/col/c/c_bg_w.h"
#include "d/col/c/c_m3d_g_aab.h"
#include "d/col/c/c_m3d_g_pla.h"
#include "d/col/c/c_partition.h"
#include "m/m_allocator.h"
#include "m/m_math.h"

class cBgS_GndChk;
class cBgS_LinChk;
class cBgS_ShdwDraw;
class dBgS_Acch;
struct dBgS_CaptPoly;
class dBgS_RoofChk;
class dBgS_SphChk;
class dBgS_SplGrpChk;

class dAcObjBase_c;

// TODO
class dBgW_Base_0x18 {
public:
    /* 0x00 */ u8 field_0x00;
    dBgW_Base_0x18();
    /* vt at 0x04 */
    virtual ~dBgW_Base_0x18();
};

// TODO
class dBgW_Base_MapRelated {
public:
    void Clear() {
        mpIdx = nullptr;
        mCount = 0;
    }
    /* 0x0 */ int *mpIdx;
    /* 0x4 */ int mCount;
};

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

    typedef dAcObjBase_c *(*PushPull_CallBack)(dAcObjBase_c *, dAcObjBase_c *, dBgW_Base::PushPullLabel);

    dBgW_Base();
    void ClrDBgWBase();
    void CalcDiffShapeAngleY(s16);
    BOOL InitMapStuff(mAllocator_c *pAllocator);
    bool fn_8034AD70() const;
    bool fn_8034ADA0() const;
    void RegistBg(int id);
    void UnRegistBg();
    bool ChkReady() const;
    void Set0x2F(u8);

    /* vt at 0x08 */
    /* 0x008 */ virtual ~dBgW_Base();
    /* 0x00C */ virtual bool ChkMemoryError();
    /* 0x010 */ virtual void SetOldShapeAngleY(s16);
    /* 0x014 */ virtual void GetTopUnder(f32 *pOutTop, f32 *pOutUnder) const = 0; // ?
    /* 0x018 */ virtual bool ChkNotReady() const = 0;
    /* 0x01C */ virtual bool ChkLock() const = 0;
    /* 0x020 */ virtual bool ChkMoveBg() const = 0;
    /* 0x024 */ virtual u32 ChkMoveFlag() const = 0;
    /* 0x028 */ virtual void GetTriPla(cBgS_PolyInfo const &, cM3dGPla *) const = 0;
    /* 0x02C */ virtual bool GetTriPnt(cBgS_PolyInfo const &, mVec3_c *, mVec3_c *, mVec3_c *) const = 0;
    /* 0x030 */ virtual const cM3dGAab *GetBnd() const = 0;
    /* 0x034 */ virtual u32 GetGrpInf(cBgS_PolyInfo const &) const = 0;
    /* 0x038 */ virtual void OffMoveFlag() = 0;
    /* 0x03C */ virtual void vt_0x3C() = 0;
    /* 0x040 */ virtual bool LineCheck(cBgS_LinChk *) = 0;
    /* 0x044 */ virtual bool GroundCross(cBgS_GndChk *) = 0;
    /* 0x048 */ virtual void ShdwDraw(cBgS_ShdwDraw *) = 0;
    /* 0x04C */ virtual void CaptPoly(dBgS_CaptPoly &) = 0;
    /* 0x050 */ virtual bool WallCorrect(dBgS_Acch *) = 0;
    /* 0x054 */ virtual bool WallCorrectSort(dBgS_Acch *) = 0;
    /* 0x058 */ virtual bool RoofChk(dBgS_RoofChk *) = 0;
    /* 0x05C */ virtual bool SplGrpChk(dBgS_SplGrpChk *) = 0;
    /* 0x060 */ virtual bool SphChk(dBgS_SphChk *, void *) = 0;
    /* 0x064 */ virtual s32 GetGrpRoomIndex(cBgS_PolyInfo const &) const = 0;
    /* 0x068 */ virtual s32 GetExitId(cBgS_PolyInfo const &) = 0;
    /* 0x06C */ virtual s32 GetZTargetThrough(cBgS_PolyInfo const &) = 0;
    /* 0x070 */ virtual int GetSpecialCode(cBgS_PolyInfo const &) = 0;
    /* 0x074 */ virtual int GetSpecialCode(int) = 0;
    /* 0x078 */ virtual int GetCode0_0x30000000(cBgS_PolyInfo const &) = 0;
    /* 0x07C */ virtual u32 GetPolyObjThrough(int) = 0;
    /* 0x080 */ virtual u32 GetPolyCamThrough(int) = 0;
    /* 0x084 */ virtual u32 GetPolyLinkThrough(int) = 0;
    /* 0x088 */ virtual u32 GetPolyArrowThrough(int) = 0;
    /* 0x08C */ virtual u32 GetPolySlingshotThrough(int) = 0;
    /* 0x090 */ virtual u32 GetPolyBeetleThrough(int) = 0;
    /* 0x094 */ virtual u32 GetPolyClawshotThrough(int) = 0;
    /* 0x098 */ virtual u32 GetPolyBombThrough(int) = 0;
    /* 0x09C */ virtual u32 GetPolyWhipThrough(int) = 0;
    /* 0x0A0 */ virtual u32 GetShdwThrough(int) = 0;
    /* 0x0A4 */ virtual u32 GetUnderwaterRoofCode(int) = 0;
    /* 0x0A8 */ virtual int GetCode0_0x80000000(cBgS_PolyInfo const &) = 0;
    /* 0x0AC */ virtual int GetLinkNo(cBgS_PolyInfo const &) = 0;
    /* 0x0B0 */ virtual s32 GetWallCode(cBgS_PolyInfo const &) = 0;
    /* 0x0B4 */ virtual int GetPolyAtt0Material(cBgS_PolyInfo const &) = 0;
    /* 0x0B8 */ virtual int GetPolyAtt0(cBgS_PolyInfo const &) = 0;
    /* 0x0Bc */ virtual int GetPolyAtt1(cBgS_PolyInfo const &) = 0;
    /* 0x0C0 */ virtual int GetGroundCode(cBgS_PolyInfo const &) = 0;
    /* 0x0C4 */ virtual u32 GetCode1_0x02000000(int) = 0;
    /* 0x0C8 */ virtual u32 GetCode1_0x04000000(int) = 0;
    /* 0x0CC */ virtual u32 GetCode1_0x08000000(int) = 0;
    /* 0x0D0 */ virtual u32 GetLightingCode(cBgS_PolyInfo const &) = 0;
    /* 0x0D4 */ virtual s32 GetCamMoveBG(cBgS_PolyInfo const &) = 0;
    /* 0x0D8 */ virtual s32 GetRoomCamId(cBgS_PolyInfo const &) = 0;
    /* 0x0DC */ virtual s32 GetRoomPathId(cBgS_PolyInfo const &) = 0;
    /* 0x0E0 */ virtual s32 GetRoomPathPntNo(cBgS_PolyInfo const &) = 0;
    /* 0x0E4 */ virtual void CrrPos(cBgS_PolyInfo const &, void *, bool, mVec3_c *, mAng3_c *, mAng3_c *) = 0;
    /* 0x0E8 */ virtual void TransPos(cBgS_PolyInfo const &, void *, bool, mVec3_c *, mAng3_c *, mAng3_c *) = 0;
    /* 0x0EC */ virtual void MatrixCrrPos(cBgS_PolyInfo const &, void *, bool, mVec3_c *, mAng3_c *, mAng3_c *) = 0;
    /* 0x0F0 */ virtual void CallRideCallback(dAcObjBase_c *, dAcObjBase_c *);
    /* 0x0F4 */ virtual void CallArrowStickCallback(dAcObjBase_c *, dAcObjBase_c *, mVec3_c &);
    /* 0x0F8 */ virtual bool CallUnkCallback(dAcObjBase_c *, dAcObjBase_c *);
    /* 0x0FC */ virtual bool UpdateDraw(mAllocator_c *);
    /* 0x100 */ virtual bool GetIsDraw(int) = 0;
    /* 0x104 */ virtual void DrawOnMap(int, bool) = 0;

    u16 GetRegistId() const {
        return mRegistId;
    }

    PushPull_CallBack GetPushPullCallback() const {
        return mpPushPull_cb;
    }
    s16 GetDiffShapeAngleY() {
        return mDiffShapeAngleY;
    }
    int GetRoomId() {
        return mRoomId;
    }

    void ClearMap() {
        for (int i = 0; i < 31; i++) {
            mMapRelated[i].Clear();
        }
    }
    void ClearMapCount() {
        for (int i = 0; i < 31; i++) {
            mMapRelated[i].mCount = 0;
        }
    }
    void SetMapCount(int c) {
        for (int i = 0; i < 31; i++) {
            mMapRelated[i].mCount = c;
        }
    }

    void SetPushPullCallback(PushPull_CallBack i_callBack) {
        mpPushPull_cb = i_callBack;
    }
    void SetRoomId(int id) {
        mRoomId = id;
    }
    bool ChkPriority(int prio) {
        return mPriority == prio;
    }
    void SetPriority(PRIORITY priority) {
        mPriority = priority;
    }
    void SetUnkBase(u8 val) {
        mField_0x18.field_0x00 = val;
    }
    void OnStickWall() {
        field_0x24 |= 1;
    }
    void OnStickRoof() {
        field_0x24 |= 2;
    }

    void OnFlag0x20() {
        field_0x24 |= 0x20;
    }

    cPartition &GetPartition() {
        return mPartitionInfo;
    }

    u16 GetField_0x20() const {
        return field_0x20;
    }
    u16 GetField_0x22() const {
        return field_0x22;
    }
    void SetField_0x22(u16 val) {
        field_0x22 = val;
    }

    bool ChkStickWall() const {
        return field_0x24 & 1;
    }
    bool ChkStickRoof() const {
        return field_0x24 & 2;
    }

    bool Chk0x24_0x20() const {
        return field_0x24 & 0x20;
    }

    // private:
    /* 0x08 */ cPartition mPartitionInfo;
    /* 0x18 */ dBgW_Base_0x18 mField_0x18;
    /* 0x20 */ u16 field_0x20;
    /* 0x22 */ u16 field_0x22;
    /* 0x24 */ u16 field_0x24;
    /* 0x26 */ s16 mOldShapeAngleY;
    /* 0x28 */ s16 mDiffShapeAngleY;
    /* 0x2A */ u16 mRegistId;
    /* 0x2C */ u8 mPriority;
    /* 0x2D */ u8 mRoomId;
    /* 0x2E */ u8 field_0x2E;
    /* 0x2F */ s8 field_0x2F;
    /* 0x30 */ PushPull_CallBack mpPushPull_cb;
    /* 0x34 */ dBgW_Base_MapRelated mMapRelated[31];
}; // Size: 0x12C

#endif
