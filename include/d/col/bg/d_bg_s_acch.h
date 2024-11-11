#ifndef D_BG_S_ACCH_H
#define D_BG_S_ACCH_H

#include "d/col/bg/d_bg_s_gnd_chk.h"
#include "d/col/bg/d_bg_s_roof_chk.h"
#include "d/col/bg/d_bg_s_sph_chk.h"
#include "d/col/bg/d_bg_s_wtr_chk.h"
#include "d/col/c/c_m3d_g_cir.h"
#include "d/col/c/c_m3d_g_cyl.h"
#include "d/col/c/c_m3d_g_lin.h"
#include "d/col/c/c_m3d_g_pla.h"

class dAcObjBase_c;

class dBgS_AcchCir : public cBgS_PolyInfo {
private:
    /* 0x2C */ cM3dGCir m_cir;
    /* 0x40 */ f32 mWallRR;
    /* 0x44 */ f32 mField_0x44;
    /* 0x48 */ f32 mWallH;
    /* 0x4C */ f32 mWallR;
    /* 0x50 */ f32 mWallHDirect;
    /* 0x54 */ s16 mWallAngleY;
    /* 0x56 */ u8 mFlags;

public:
    enum {
        /* 0x02 */ WALL_HIT = 2,
        /* 0x04 */ WALL_H_DIRECT = 4,
        /* 0x08 */ UNK_0x8 = 8,
        /* 0x10 */ UNK_0x10 = 0x10,
    };

    dBgS_AcchCir();
    virtual ~dBgS_AcchCir();

    bool ChkWallHit() const;

    void ClrWallHit() {
        mFlags &= ~WALL_HIT;
    }

    void SetWallR(f32);
    void CalcWallRR();
    void SetWall(f32, f32);
    void SetWallHDirect(f32 f) {
        mFlags |= WALL_H_DIRECT;
        mWallHDirect = f;
    }

    f32 GetWallH() {
        return mWallH;
    }
    f32 GetWallR() {
        return mWallR;
    }
    void SetWallH(f32 h) {
        mWallH = h;
    }
    void ClrWallHDirect() {
        mFlags &= ~WALL_H_DIRECT;
    }
    // bool ChkWallHit() {
    //     return mFlags & WALL_HIT;
    // }
    bool ChkUnk0x8() {
        return mFlags & UNK_0x8;
    }
    bool ChkUnk0x10() {
        return mFlags & UNK_0x10;
    }
    s16 GetWallAngleY() {
        return mWallAngleY;
    }
    bool ChkWallHDirect() {
        return mFlags & WALL_H_DIRECT;
    }
    f32 GetWallHDirect() {
        return mWallHDirect;
    }
    f32 GetWallRR() {
        return mWallRR;
    }
    void SetWallHit() {
        mFlags |= WALL_HIT;
    }
    void SetUnk0x8() {
        mFlags |= UNK_0x8;
    }
    void SetUnk0x10() {
        mFlags |= UNK_0x10;
    }
    void SetWallAngleY(s16 i_angle) {
        mWallAngleY = i_angle;
    }
    cM3dGCir *GetCirP() {
        return &m_cir;
    }

    void SetCir(mVec3_c &pos) {
        m_cir.Set(pos.x, pos.z, pos.y + GetWallH(), mWallR);
    }
};

// TODO: Needs to be relocated when Known
struct dBgS_Acch_0x1A8 {
    u8 mField_0x00;
    mVec3_c mField_0x04;
    mVec3_c mField_0x10;

    dBgS_Acch_0x1A8();

    void Init();
    void CalcPos(mVec3_c *);
};

class dBgS;
class dBgS_Acch : public cBgS_Chk, public dBgS_Chk {
public:
    enum {
        /* 0x 0000 0001 */ ACCH_FLAG_0x1 = (1 << 0),
        /* 0x 0000 0002 */ GRND_NONE = (1 << 1),
        /* 0x 0000 0004 */ WALL_NONE = (1 << 2),
        /* 0x 0000 0008 */ ROOF_NONE = (1 << 3),
        /* 0x 0000 0010 */ WALL_HIT = (1 << 4),
        /* 0x 0000 0020 */ GROUND_HIT = (1 << 5),
        /* 0x 0000 0040 */ GROUND_FIND = (1 << 6),
        /* 0x 0000 0080 */ GROUND_LANDING = (1 << 7),
        /* 0x 0000 0100 */ GROUND_AWAY = (1 << 8),
        /* 0x 0000 0200 */ ROOF_HIT = (1 << 9),
        /* 0x 0000 0400 */ WATER_NONE = (1 << 10),
        /* 0x 0000 0800 */ WATER_HIT = (1 << 11),
        /* 0x 0000 1000 */ WATER_IN = (1 << 12),
        /* 0x 0000 2000 */ LINE_CHECK = (1 << 13),
        /* 0x 0000 4000 */ LINE_CHECK_NONE = (1 << 14),
        /* 0x 0000 8000 */ CLR_SPEED_Y = (1 << 15),
        /* 0x 0001 0000 */ LINE_CHECK_HIT = (1 << 16),
        /* 0x 0002 0000 */ ACCH_FLAG_0x20000 = (1 << 17),
        /* 0x 0010 0000 */ MOVE_BG_ONLY = (1 << 20),
        /* 0x 0020 0000 */ GND_THIN_CELLING_OFF = (1 << 21),
        /* 0x 0040 0000 */ WALL_SORT = (1 << 22),
        /* 0x 0080 0000 */ LINE_DOWN = (1 << 23),

        /* 0x 0200 0000 */ ACCH_FLAG_0x2000000 = (1 << 25),
        /* 0x 0400 0000 */ ACCH_FLAG_0x4000000 = (1 << 26),
        /* 0x 0800 0000 */ ACCH_FLAG_0x8000000 = (1 << 27),
        /* 0x 1000 0000 */ ACCH_FLAG_0x10000000 = (1 << 28),
        /* 0x 2000 0000 */ ACCH_FLAG_0x20000000 = (1 << 29),
        /* 0x 8000 0000 */ ACCH_FLAG_0x80000000 = (1 << 31),
    };

    dBgS_Acch();

    void Init() {
        mField_0x394 = 0;
        ClrGroundAway();
        ClrWallHit();
        ClrGroundLanding();
        i_ClrGroundHit();
        int i = 0;
        do {
            mpAcchCir[i].ClrWallHit();
            mpAcchCir[i].ClearPi();
            mpAcchCir[i].ClrWallHDirect();
            ++i;
        } while (i < mTblSize);
    }

    void Set(mVec3_c *, mVec3_c *, dAcObjBase_c *, int, dBgS_AcchCir *, mVec3_c *, mAng3_c *, mAng3_c *);
    void Set(dAcObjBase_c *, int, dBgS_AcchCir *);
    bool fn_8033f5b0(mVec3_c *, mVec3_c *, f32);
    void LineCheck(dBgS &);
    void SphCheck();
    void GroundCheckInit(dBgS &) {
        if (!(mFlags & GRND_NONE)) {
            mGroundHeight = -1e9f;
            mGnd.SetExtChk(*this);

            if (i_ChkGroundHit()) {
                Set_0x4000000();
            } else {
                Clr_0x4000000();
            }
        }
    }
    void GroundCheck(dBgS &, bool);
    void RoofCheck(dBgS &);
    void GroundRoofProc(dBgS &, bool);
    void CrrPos(dBgS &);
    f32 GetWallAllR();
    void SetWallCir();
    void CalcWallBmdCyl();
    void SetGroundUpY(f32);

    bool fn_80340ca0(cBgS_PolyInfo &info) const;
    void Draw(dBgS &); // Name Assumed, no info

    void SetMoveBGOnly();
    void ClrMoveBGOnly();

    bool ChkGndHit() const;
    bool ChkRoofHit() const;
    bool ChkWallHit(int *) const;

    void SetGndThinCellingOff();
    void ClrGndThinCellingOff();
    void Set_0x2000000();
    void Clr_0x2000000();
    void OnWallSort();
    dBgS_AcchCir &GetCir(int idx);
    void fn_80340f70(const mVec3_c *);
    void fn_80341000();

    f32 GetWallAllLowH();   // is inline
    f32 GetWallAllLowH_R(); // is inline
    f32 GetWallAddY(Vec &);
    void SetNowActorInfo(int, void *, unsigned int);
    void SetWallPolyIndex(int, int);
    void CalcMovePosWork();
    void CalcWallRR();
    bool ChkGndThinCellingOff();
    bool ChkWallSort();
    bool ChkLineDown();
    void ClrGroundHit();

    virtual ~dBgS_Acch();

    f32 GetSpeedY() {
        if (mpSpeed) {
            return mpSpeed->y;
        }
        return 0.0f;
    }

    mVec3_c *GetPos() {
        return mpPos;
    }
    mVec3_c *GetOldPos() {
        return mpOldPos;
    }
    f32 GetGroundH() const {
        return mGroundHeight;
    }
    f32 GetRoofHeight() const {
        return mRoofHeight;
    }
    int GetTblSize() {
        return mTblSize;
    }
    void SetLin() {
        mLine.SetStartEnd(*mpOldPos, *mpPos);
    }
    bool ChkGroundFind() {
        return mFlags & GROUND_FIND;
    }
    bool ChkGroundHit() {
        return mFlags & GROUND_HIT;
    }
    bool ChkGroundLanding() {
        return mFlags & GROUND_LANDING;
    }
    void ClrGroundLanding() {
        mFlags &= ~GROUND_LANDING;
    }
    void ClrGroundAway() {
        mFlags &= ~GROUND_AWAY;
    }
    void ClrWallHit() {
        mFlags &= ~WALL_HIT;
    }
    void SetRoofNone() {
        mFlags |= ROOF_NONE;
    }
    void SetRoofHit() {
        mFlags |= ROOF_HIT;
    }
    void SetWaterNone() {
        mFlags |= WATER_NONE;
    }
    bool ChkWallHit() {
        return mFlags & WALL_HIT;
    }
    void OffLineCheckHit() {
        mFlags &= ~LINE_CHECK_HIT;
    }
    void OffLineCheck() {
        mFlags &= ~LINE_CHECK;
    }
    bool ChkLineCheckNone() {
        return mFlags & LINE_CHECK_NONE;
    }
    bool ChkLineCheck() {
        return mFlags & LINE_CHECK;
    }
    bool Chk_0x4000000() const {
        return mFlags & ACCH_FLAG_0x4000000;
    }
    void Set_0x4000000() {
        mFlags |= ACCH_FLAG_0x4000000;
    }
    void Clr_0x4000000() {
        mFlags &= ~ACCH_FLAG_0x4000000;
    }
    void Clr_0x10000000() {
        mFlags &= ~ACCH_FLAG_0x10000000;
    }
    void Set_0x10000000() {
        mFlags |= ACCH_FLAG_0x10000000;
    }
    bool Chk_0x10000000() {
        return mFlags & ACCH_FLAG_0x10000000;
    }

    void Clr_0x20000() {
        mFlags &= ~ACCH_FLAG_0x20000;
    }
    void Set_0x20000() {
        mFlags |= ACCH_FLAG_0x20000;
    }
    bool Chk_0x20000() {
        return mFlags & ACCH_FLAG_0x20000;
    }
    void Clr_0x80000000() {
        mFlags &= ~ACCH_FLAG_0x80000000;
    }
    void Set_0x80000000() {
        mFlags |= ACCH_FLAG_0x80000000;
    }
    bool Chk_0x80000000() {
        return mFlags & ACCH_FLAG_0x80000000;
    }
    void Clr_0x8000000() {
        mFlags &= ~ACCH_FLAG_0x8000000;
    }
    void Set_0x8000000() {
        mFlags |= ACCH_FLAG_0x8000000;
    }
    bool Chk_0x8000000() {
        return mFlags & ACCH_FLAG_0x8000000;
    }

    void ClrRoofHit() {
        mFlags &= ~ROOF_HIT;
    }
    void ClrWaterHit() {
        mFlags &= ~WATER_HIT;
    }
    void SetWaterHit() {
        mFlags |= WATER_HIT;
    }
    void ClrWaterIn() {
        mFlags &= ~WATER_IN;
    }
    void SetWaterIn() {
        mFlags |= WATER_IN;
    }
    const u32 MaskWaterIn() {
        return mFlags & WATER_IN;
    }
    const bool ChkWaterIn() {
        return MaskWaterIn();
    }
    void ClrGroundFind() {
        mFlags &= ~GROUND_FIND;
    }
    u32 MaskRoofHit() {
        return mFlags & ROOF_HIT;
    }
    // bool ChkRoofHit() {
    //     return MaskRoofHit();
    // }
    void OffClrSpeedY() {
        mFlags |= CLR_SPEED_Y;
    }
    bool ChkClrSpeedY() {
        return !(mFlags & CLR_SPEED_Y);
    }
    void SetGroundFind() {
        mFlags |= GROUND_FIND;
    }
    void SetGroundHit() {
        mFlags |= GROUND_HIT;
    }
    void SetGroundLanding() {
        mFlags |= GROUND_LANDING;
    }
    void SetGroundAway() {
        mFlags |= GROUND_AWAY;
    }
    const u32 MaskWaterHit() {
        return mFlags & WATER_HIT;
    }
    const bool ChkWaterHit() {
        return MaskWaterHit();
    }
    void ClrWaterNone() {
        mFlags &= ~WATER_NONE;
    }
    // void SetWaterCheckOffset(f32 offset) {
    //     m_wtr_chk_offset = offset;
    // }
    void OnLineCheck() {
        mFlags |= LINE_CHECK;
    }
    void ClrRoofNone() {
        mFlags &= ~ROOF_NONE;
    }
    bool CheckGroundHitPlus() {
        return mFlags & ACCH_FLAG_0x4000000;
    }
    // void SetRoofCrrHeight(f32 height) {
    //     m_roof_crr_height = height;
    // }
    // void SetWtrChkMode(int mode) {
    //     m_wtr_mode = mode;
    // }
    void SetGrndNone() {
        mFlags |= GRND_NONE;
    }
    void ClrGrndNone() {
        mFlags &= ~GRND_NONE;
    }
    bool ChkMoveBGOnly() const {
        return mFlags & MOVE_BG_ONLY;
    }
    void SetWallHit() {
        mFlags |= WALL_HIT;
    }
    void ClrWallNone() {
        mFlags &= ~WALL_NONE;
    }
    void OnLineCheckNone() {
        mFlags |= LINE_CHECK_NONE;
    }
    void OffLineCheckNone() {
        mFlags &= ~LINE_CHECK_NONE;
    }
    void SetWallNone() {
        mFlags |= WALL_NONE;
    }
    void OnLineCheckHit() {
        mFlags |= LINE_CHECK_HIT;
    }
    cM3dGCyl *GetWallBmdCylP() {
        return &mWallCyl;
    }
    dAcObjBase_c *GetMyObj() {
        return mpMyObj;
    }

    cM3dGCir *GetWallCirP(int index) {
        // JUT_ASSERT(index <= mTblSize);
        return mpAcchCir[index].GetCirP();
    }

    f32 GetWallH(int i_no) {
        return mpAcchCir[i_no].GetWallH();
    }
    f32 GetWallR(int i_no) {
        return mpAcchCir[i_no].GetWallR();
    }
    bool ChkWallHDirect(int i_no) {
        return mpAcchCir[i_no].ChkWallHDirect();
    }
    f32 GetWallHDirect(int i_no) {
        return mpAcchCir[i_no].GetWallHDirect();
    }
    void SetWallHDirect(int i_no, f32 f) {
        mpAcchCir[i_no].SetWallHDirect(f);
    }
    f32 GetWallRR(int i_no) {
        return mpAcchCir[i_no].GetWallRR();
    }
    void SetWallCirHit(int i_no) {
        mpAcchCir[i_no].SetWallHit();
    }
    void SetWallAngleY(int i_no, s16 i_angle) {
        mpAcchCir[i_no].SetWallAngleY(i_angle);
    }
    bool ChkCirUnk0x8(int i_no) {
        return mpAcchCir[i_no].ChkUnk0x8();
    }
    bool ChkCirUnk0x10(int i_no) {
        return mpAcchCir[i_no].ChkUnk0x10();
    }

    f32 GetCx() const {
        return mpPos->x;
    }
    f32 GetCz() const {
        return mpPos->z;
    }

    // inline dupe
    void i_ClrGroundHit() {
        mFlags &= ~GROUND_HIT;
    }
    bool i_ChkGroundHit() const {
        return mFlags & GROUND_HIT;
    }

    dBgS_GndChk &GetGnd() {
        return mGnd;
    }

public:
    /* 0x040 */ u32 mFlags;
    /* 0x044 */ mVec3_c *mpPos;
    /* 0x048 */ mVec3_c *mpOldPos;
    /* 0x04C */ mVec3_c mSpeed;
    /* 0x058 */ mVec3_c *mpSpeed;
    /* 0x05C */ mAng3_c *mpAngle;
    /* 0x060 */ mAng3_c *mpShapeAngle;
    /* 0x064 */ cM3dGLin mLine;
    /* 0x07C */ cM3dGCyl mWallCyl;
    /* 0x090 */ int mBgIndex;
    /* 0x094 */ void *mField_0x094;
    /* 0x098 */ u32 mField_0x098;
    /* 0x09C */ dAcObjBase_c *mpMyObj;
    /* 0x0A0 */ int mTblSize;
    /* 0x0A4 */ dBgS_AcchCir *mpAcchCir;
    /* 0x0A8 */ f32 mField_0x0A8;
    /* 0x0AC */ f32 mField_0x0AC;
    /* 0x0B0 */ f32 mGroundHeight;
    /* 0x0B4 */ f32 mField_0x0B4;
    /* 0x0B8 */ cM3dGPla mPlane_0x0B8;
    /* 0x0C8 */ f32 mField_0x0C8;
    /* 0x0CC */ f32 mField_0x0CC;
    /* 0x0D0 */ f32 mRoofHeight;
    /* 0x0D4 */ f32 mField_0x0D4;
    /* 0x0D8 */ f32 mField_0x0D8;
    /* 0x0DC */ cBgS_PolyInfo *mpOutPolyInfo;
    /* 0x0E0 */ f32 mRoofH_0x0E0;
    /* 0x0E4 */ f32 mGroundH_0x0E4;
    /* 0x0E8 */ f32 mField_0x0E8;
    /* 0x0EC */ dBgS_SphChk mSph;
    /* 0x1A8 */ dBgS_Acch_0x1A8 mField_0x1A8;
    /* 0x1C4 */ u8 _0[0x1D0 - 0x1C4];
    /* 0x1D0 */ f32 mField_0x1D0;
    /* 0x1D4 */ dBgS_GndChk mGnd;
    /* 0x264 */ dBgS_RoofChk mRoof;
    /* 0x2F4 */ f32 mField_0x2F4;
    /* 0x2F8 */ dBgS_WtrChk mWtr;
    /* 0x390 */ u8 mField_0x390;
    /* 0x394 */ s32 mField_0x394;
    /* 0x398 */ mVec3_c mField_0x398;
    /* 0x3A4 */ mVec3_c mField_0x3A4;
};

class dBgS_ObjAcch : public dBgS_Acch {
public:
    dBgS_ObjAcch();
    virtual ~dBgS_ObjAcch();
};

class dBgS_LinkAcch : public dBgS_Acch {
public:
    dBgS_LinkAcch();
    virtual ~dBgS_LinkAcch();
};

class dBgS_BombAcch : public dBgS_Acch {
public:
    dBgS_BombAcch();
    virtual ~dBgS_BombAcch();
};

#endif
