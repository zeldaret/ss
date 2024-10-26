#include "d/col/bg/d_bg_s_acch.h"

#include "d/a/d_a_player.h"
#include "d/col/bg/d_bg_s.h"
#include "d/col/c/c_bg_s_lin_chk.h"
#include "d/col/c/c_bg_w.h"

dBgS_AcchCir::dBgS_AcchCir() {
    mWallRR = 0.0f;
    mField_0x44 = 0.0f;
    mWallH = 0.0f;
    mWallR = 0.0f;
    mWallHDirect = 0.0f;
    mWallAngleY = 0.0f;
    mFlags = 0;
}

dBgS_AcchCir::~dBgS_AcchCir() {}

bool dBgS_AcchCir::ChkWallHit() const {
    bool ret = false;
    if (mFlags & WALL_HIT) {
        if (dBgS::GetInstance()->ChkPolySafe(*this)) {
            ret = true;
        }
    }
    return ret;
}

void dBgS_AcchCir::SetWallR(f32 wallR) {
    mWallR = wallR;
}

void dBgS_AcchCir::SetWall(f32 wallH, f32 wallR) {
    mWallH = wallH;
    SetWallR(wallR);
}

dBgS_Acch::dBgS_Acch()
    : mFlags(0), mpPos(nullptr), mpOldPos(nullptr), mSpeed(0.0f, 0.0f, 0.0f), mpSpeed(nullptr), mpAngle(nullptr),
      mpShapeAngle(nullptr), mBgIndex(0), mField_0x094(0), mField_0x098(0), mpMyObj(nullptr), mTblSize(0),
      mpAcchCir(nullptr), mField_0x0A8(0.0f), mField_0x0AC(0.0f), mGroundHeight(-1e9f), mField_0x0B4(120.0f),
      mField_0x0C8(0.0f), mField_0x0CC(0.0f), mRoofHeight(1e9f), mField_0x0D4(1.0f), mField_0x0D8(0.0f),
      mpOutPolyInfo(nullptr), mRoofH_0x0E0(0.0f), mGroundH_0x0E4(0.0f), mField_0x0E8(1e9f), mField_0x1D0(-1.0f),
      mField_0x2F4(1000.0f), mField_0x390(0), mField_0x394(0) {
    SetPolyPassChk(GetPolyPassChkInfo());
    SetGrpPassChk(GetGrpPassChkInfo());

    SetRoofNone();
    mRoof.mField_0x7C = 1;

    mField_0x398.set(0.0f, 0.0f, 0.0f);
    mField_0x3A4.set(0.0f, 0.0f, 0.0f);
}

dBgS_Acch::~dBgS_Acch() {}

void dBgS_Acch::Set(
    mVec3_c *pPos, mVec3_c *pOldPos, dAcObjBase_c *pObj, int tblSize, dBgS_AcchCir *pAccCir, mVec3_c *pSpeed,
    mAng3_c *pAngle, mAng3_c *pShapeAngle
) {
    mpPos = pPos;
    mpOldPos = pOldPos;
    mpMyObj = pObj;

    mActorId = pObj->unique_ID;
    mpSpeed = pSpeed;
    if (pSpeed) {
        mSpeed = *pSpeed;
    } else {
        mSpeed.set(0.0f, 0.0f, 0.0f);
    }

    mTblSize = tblSize;
    mpAcchCir = pAccCir;
    mpAngle = pAngle;
    mpShapeAngle = pShapeAngle;
    mGroundHeight = mpPos->y;

    mField_0x1A8.Init();
}

void dBgS_Acch::Set(dAcObjBase_c *pObj, int tblSize, dBgS_AcchCir *pAcchCir) {
    mTblSize = tblSize;
    mpAcchCir = pAcchCir;
    mpMyObj = pObj;
    mActorId = pObj->getID();
    mpPos = &pObj->GetPostion();
    mpOldPos = &pObj->GetOldPosition();
    mpSpeed = &pObj->GetVelocity();
    if (mpSpeed) {
        mSpeed = pObj->GetVelocity();
    } else {
        mSpeed.set(0.0f, 0.0f, 0.0f);
    }
    mpAngle = &pObj->GetAngle();
    mpShapeAngle = &pObj->GetRotation();
}

bool dBgS_Acch::fn_8033f5b0(mVec3_c *pPos, mVec3_c *pOldPos, f32 height) {
    *pOldPos = *pPos;
    cBgS_LinChk line;
    mVec3_c v1, v2;

    v1 = *GetOldPos();
    v1.y += height;

    v2 = *pPos;
    v2.y += height;

    line.Set2(v1, v2, mActorId);

    bool ret = false;

    line.SetExtChk(*this);

    if (dBgS::GetInstance()->LineCross(&line)) {
        ret = true;
        *pPos = line.mLin.mEnd;
        cM3dGPla pla;
        dBgS::GetInstance()->GetTriPla(line, &pla);
        if (!cBgW_CheckBGround(pla.mNormal.y)) {
            VEC3Add(pPos, pla.mNormal, pPos);
            pPos->y -= height;
        }
    }
    mpPos = pPos;
    mpOldPos = pOldPos;

    return ret;
}

void dBgS_Acch::GroundCheck(dBgS &bgs, bool param2) {
    if (!(mFlags & GRND_NONE)) {
        mVec3_c gnd_pos = *GetPos();

        // This Ordering is weird
        f32 temp = (mField_0x0B4 - mField_0x0A8) + mField_0x0AC;
        if (mFlags & ACCH_FLAG_0x20000) {
            if (temp <= 1.1f) {
                temp = 1.1f;
            }
        }
        gnd_pos.y += temp;

        if (!(mFlags & GND_THIN_CELLING_OFF)) {
            mVec3_c old_pos(*GetOldPos());
            old_pos.y += 0.25f;
            mVec3_c pos(*GetPos());
            bgs.MoveBgCrrPos(mGnd, Chk_0x10000000(), &old_pos, nullptr, nullptr, false, false);
            bgs.MoveBgCrrPos(mGnd, Chk_0x10000000(), &pos, nullptr, nullptr, false, false);

            cBgS_LinChk line;
            line.Set2(pos, old_pos, mActorId);

            line.SetExtChk(*this);
            line.mBackFlag = true;
            if (!bgs.LineCross(&line)) {
                dBgS_RoofChk roof;
                roof.mActorId = mGnd.mActorId;
                roof.SetPos(&pos);
                f32 roof_height = bgs.RoofChk(&roof);
                cM3dGPla plane;
                if (roof_height != 1e9f) {
                    bgs.GetTriPla(roof, &plane);
                    if (gnd_pos.y > roof_height) {
                        gnd_pos.y = roof_height;
                    }
                }
            }
        }
        mField_0x0AC = 0.0f;
        if (gnd_pos.y > mRoofHeight) {
            gnd_pos.y = mRoofHeight + 0.25f;
        }

        if (gnd_pos.y < mpPos->y + mField_0x0E8 &&
            mpPos->y + mField_0x0E8 < (mField_0x0B4 - mField_0x0A8) + mField_0x0AC + mpPos->y) {
            gnd_pos.y = mpPos->y + mField_0x0E8;
        }

        mGnd.SetExtChk(*this);
        mGnd.SetPos(&gnd_pos);

        mGroundHeight = bgs.GroundCross(&mGnd);
        Clr_0x10000000();
        if (mGroundHeight != -1e9f) {
            dBgS::GetInstance()->SetLightingCode(GetMyObj(), mGnd);
            mField_0x0CC = mGroundHeight + mField_0x0A8;
            if (mField_0x0CC > mField_0x0C8) {
                mpPos->y = mField_0x0CC;
                if (ChkClrSpeedY() && mpSpeed) {
                    mpSpeed->y = 0.0f;
                }
                bgs.GetTriPla(mGnd, &mPlane_0x0B8);
                SetGroundFind();
                if (param2) {
                    SetGroundHit();
                }

                Set_0x10000000();
                if (!Chk_0x8000000() && !Chk_0x80000000()) {
                    Set_0x8000000();
                    bgs.RideCallBack(mGnd, GetMyObj());
                }

                if (!(Chk_0x4000000())) {
                    SetGroundLanding();
                }
            }
        }

        if (Chk_0x4000000() && !i_ChkGroundHit()) {
            SetGroundAway();
        }

    } else {
        mGroundHeight = -1e9f;
    }
}

void dBgS_Acch::RoofCheck(dBgS &bgs) {
    if (mGroundHeight == -1e9f) {
        return;
    }
    if (mFlags & LINE_DOWN && mField_0x0CC > mField_0x0D8 &&
        (mGroundHeight <= mRoofHeight || cM3d_IsZero(mGroundHeight - mRoofHeight))) {
        mpPos->y = (mGroundHeight + mRoofHeight) * 0.5f;
    } else if (mpPos->y > mField_0x0D8 && mField_0x0D8 > mGroundHeight) {
        mpPos->y = mField_0x0D8;
    }
    if (mFlags & ROOF_NONE) {
        return;
    }
    if (mGroundHeight >= mRoofHeight) {
        mRoof.SetExtChk(*this);
        ClrRoofHit();
        mVec3_c roof_pos = *GetPos();
        roof_pos.y -= 0.25f;
        mRoof.SetPos(&roof_pos);
        mRoofHeight = bgs.RoofChk(&mRoof);
    }
}

void dBgS_Acch::GroundRoofProc(dBgS &bgs, bool param2) {
    mField_0x0D8 = 1e9f;
    if (!(mFlags & ROOF_NONE)) {
        mRoof.SetExtChk(*this);
        ClrRoofHit();
        mVec3_c roof_pos(*GetPos());
        roof_pos.y -= 0.25f;
        mRoof.SetPos(&roof_pos);
        mRoofHeight = bgs.RoofChk(&mRoof);

        if (mRoofHeight != 1e9f) {
            if (mpPos->y + mField_0x0D4 > mRoofHeight) {
                mField_0x0D8 = mRoofHeight - mField_0x0D4;
                SetRoofHit();
            } else if (mField_0x0C8 + mField_0x0D4 > mRoofHeight) {
                mField_0x0D8 = mRoofHeight - mField_0x0D4;
                SetRoofHit();
            }
        }
    }

    if (!(mFlags & GRND_NONE)) {
        mFlags &= ~GROUND_FIND;
        GroundCheck(bgs, param2);
        RoofCheck(bgs);
    } else {
        if (mField_0x0D8 < mpPos->y) {
            mpPos->y = mField_0x0D8;
        }
        mFlags &= ~ACCH_FLAG_0x10000000;
        mGroundHeight = -1e9f;
    }
}

void dBgS_Acch::LineCheck(dBgS &bgs) {
    if (!(mFlags & ACCH_FLAG_0x20000000)) {
        mFlags |= ACCH_FLAG_0x20000000;

        dBgS_RoofChk roof;
        roof.mActorId = mGnd.mActorId;

        mVec3_c roof_pos = *GetOldPos();
        roof_pos.y -= 0.25f;

        roof.SetPos(&roof_pos);

        mRoofH_0x0E0 = dBgS::GetInstance()->RoofChk(&roof);

        dBgS_GndChk gnd;
        gnd.mActorId = mGnd.mActorId;

        mVec3_c gnd_pos = *GetOldPos();
        gnd_pos.y += 0.25f;

        gnd.SetPos(&gnd_pos);

        mGroundH_0x0E4 = dBgS::GetInstance()->GroundCross(&gnd);
    }

    cBgS_LinChk line;
    mField_0x0E8 = 1e9f;

    int i = 0;
    do {
        mVec3_c old_pos = *GetOldPos();
        mVec3_c pos = *GetPos();

        f32 cirWallH = GetWallH(i);

        if (mRoofH_0x0E0 - 0.25f < old_pos.y + cirWallH && mGroundH_0x0E4 + 0.25f > old_pos.y + cirWallH) {
            cirWallH = (mRoofH_0x0E0 + mGroundH_0x0E4) * 0.5f - old_pos.y;
        } else if (mRoofH_0x0E0 - 0.25f < old_pos.y + cirWallH) {
            cirWallH = (mRoofH_0x0E0 - old_pos.y) - 0.25f;
        } else if (mGroundH_0x0E4 + 0.25f > old_pos.y + cirWallH) {
            cirWallH = (mGroundH_0x0E4 - old_pos.y) + 0.25f;
        }

        if (mField_0x0E8 > cirWallH) {
            mField_0x0E8 = cirWallH;
        }

        old_pos.y += cirWallH;
        pos.y += cirWallH;

        if (GetSpeedY() < 0.0f) {
            if (GetMyObj() != nullptr && GetMyObj()->isActorPlayer() &&
                !static_cast<dAcPy_c *>(GetMyObj())->CheckPlayerFly() && ChkLink()) {
                pos.y -= GetSpeedY();
            }
        }

        if (old_pos.x != pos.x || old_pos.y != pos.y || old_pos.z != pos.z) {
            line.Set2(old_pos, pos, mActorId);

            if (ChkCirUnk0x8(i)) {
                line.mPreGroundChk = true;
            } else {
                line.mPreGroundChk = false;
            }
            if (ChkCirUnk0x10(i)) {
                line.mPreRoofChk = true;
            } else {
                line.mPreRoofChk = false;
            }

            line.SetExtChk(*this);

            if (bgs.LineCross(&line)) {
                *GetPos() = line.GetLinEnd();
                OnLineCheckHit();
                if (mpOutPolyInfo) {
                    *mpOutPolyInfo = line;
                }
                cM3dGPla plane;
                bgs.GetTriPla(line, &plane);
                if (!cBgW_CheckBGround(plane.mNormal.y)) {
                    mVec3_c *pPos = GetPos();
                    VEC3Add(*pPos, plane.mNormal, *pPos);
                    if (!cM3d_IsZero(
                            nw4r::math::FSqrt(plane.mNormal.x * plane.mNormal.x + plane.mNormal.z * plane.mNormal.z)
                        )) {
                        SetWallHDirect(i, GetPos()->y);
                    }
                    GetPos()->y -= GetWallH(i);

                } else {
                    GetPos()->y -= 1.0f;
                    mFlags |= ACCH_FLAG_0x20000;
                }
            }
        }
        i++;
    } while (i < GetTblSize());
}

extern "C" void *fn_80359C80(dBgS_SphChk *, cBgD_Vtx_t *, int, int, int, cM3dGPla *, void *);

void dBgS_Acch::SphCheck() {
    f32 height = mField_0x1D0;

    mSph.SetExtChk(*this);
    mSph.mCallback = fn_80359C80;

    if (height < 0.0f) {
        height = 1e9f;
        if (height > mField_0x0A8 && mField_0x0A8 > 0.0f) {
            height = mField_0x0A8;
        }
        if (height > mField_0x0D4 && mField_0x0D4 > 1.0f) {
            height = mField_0x0D4;
        }
        height *= 0.8f;
    }
    mSph.Set(mpPos, height);
    mField_0x1A8.Init();
    dBgS::GetInstance()->SphChk(&mSph, &mField_0x1A8);
    mField_0x1A8.CalcPos(mpPos);
}

void dBgS_Acch::CrrPos(dBgS &bgs) {
    if (!(mFlags & ACCH_FLAG_0x1)) {
        bgs.MoveBgCrrPos(mGnd, ChkGroundHit(), mpPos, mpAngle, mpShapeAngle, false, false);

        GroundCheckInit(bgs);
        u32 prev = mField_0x394;

        Init();

        // Not in the mood to mess with these inlines lol
    }
}

f32 dBgS_Acch::GetWallAllR() {
    f32 ret = 0.0f;

    int i = 0;
    do {
        if (ret < mpAcchCir[i].GetWallR()) {
            ret = mpAcchCir[i].GetWallR();
        }
        ++i;
    } while (i < mTblSize);
    return ret;
}

void dBgS_Acch::CalcWallBmdCyl() {
    if (mTblSize <= 0) {
        mWallCyl.Set(*mpPos, 0.0f, 0.0f);
    } else {
        f32 speed = GetSpeedY();

        // TOD Stuff
        f32 dvar8 = GetWallAllR();
        f32 dvar9 = mpAcchCir->GetWallH();
        f32 dvar10 = mpAcchCir->GetWallH();

        // if (mTblSize >= 1) {
        //     for (int i = 0; i < mTblSize; i++) {
        //         f32 tmp = mpAcchCir[i].GetWallH();
        //         if (dvar9 > tmp) {
        //             dvar9 = mpAcchCir[i].GetWallH();
        //         }

        // f32 tmp2 = mpAcchCir[i].GetWallH();
        // if (dvar10 < tmp2) {
        //     dvar10 = mpAcchCir[i].GetWallH();
        // }
        // }
        // }

        mVec3_c xyz(*GetPos());

        // TODO: Stuff

        mWallCyl.Set(xyz, dvar8, dvar10 - dvar9);
    }
}

void dBgS_Acch::SetGroundUpY(f32 param_0) {
    mField_0x0AC = param_0 - mField_0x0A8;
    mField_0x0A8 = param_0;
}

bool dBgS_Acch::fn_80340ca0(cBgS_PolyInfo &info) const {
    int idx = 0;
    if (ChkGndHit()) {
        info.SetPolyInfo(mGnd);
        return false;
    }

    if (ChkWallHit(&idx)) {
        info.SetPolyInfo(mpAcchCir[idx]);
        return false;
    }

    if (ChkRoofHit()) {
        info.SetPolyInfo(mRoof);
        return false;
    }

    return true;
}

void dBgS_Acch::Draw(dBgS &) {
    // Name inferred from being called from (NPC?) draw functions.
    // Maybe a debug visual?
}

void dBgS_Acch::SetMoveBGOnly() {
    mFlags |= MOVE_BG_ONLY;
    mWtr.OnMoveBGOnly();
}

void dBgS_Acch::ClrMoveBGOnly() {
    mFlags &= ~MOVE_BG_ONLY;
    mWtr.OffMoveBGOnly();
}

bool dBgS_Acch::ChkGndHit() const {
    bool ret = false;

    if (mFlags & GROUND_HIT) {
        if (dBgS::GetInstance()->ChkPolySafe(mGnd)) {
            ret = true;
        }
    }
    return ret;
}

bool dBgS_Acch::ChkRoofHit() const {
    bool ret = false;

    if (mFlags & ROOF_HIT) {
        if (dBgS::GetInstance()->ChkPolySafe(mRoof)) {
            ret = true;
        }
    }
    return ret;
}

bool dBgS_Acch::ChkWallHit(int *pOutIndex) const {
    if (mFlags & WALL_HIT) {
        int i = 0;
        do {
            if (mpAcchCir[i].ChkWallHit()) {
                if (pOutIndex) {
                    *pOutIndex = i;
                }
                return true;
            }
            i++;
        } while (i < mTblSize);
    }
    return false;
}

void dBgS_Acch::SetGndThinCellingOff() {
    mFlags |= GND_THIN_CELLING_OFF;
}

void dBgS_Acch::ClrGndThinCellingOff() {
    mFlags &= ~GND_THIN_CELLING_OFF;
}

void dBgS_Acch::Set_0x2000000() {
    mFlags |= ACCH_FLAG_0x2000000;
}

void dBgS_Acch::Clr_0x2000000() {
    mFlags &= ~ACCH_FLAG_0x2000000;
}

void dBgS_Acch::OnWallSort() {
    mFlags |= WALL_SORT;
}

dBgS_AcchCir &dBgS_Acch::GetCir(int idx) {
    return mpAcchCir[idx];
}

void dBgS_Acch::fn_80340f70(const mVec3_c *pVec) {
    bool adjusted = false;

    if (mField_0x398.x > pVec->x) {
        mField_0x398.x = pVec->x;
        adjusted = true;
    } else if (mField_0x3A4.x < pVec->x) {
        mField_0x3A4.x = pVec->x;
        adjusted = true;
    }
    if (mField_0x398.z > pVec->z) {
        mField_0x398.z = pVec->z;
        adjusted = true;
    } else if (mField_0x3A4.z < pVec->z) {
        mField_0x3A4.z = pVec->z;
        adjusted = true;
    }

    if (!adjusted && mField_0x394 > 0) {
        mField_0x394--;
    }
}

void dBgS_Acch::fn_80341000() {
    mpPos->x += mField_0x398.x + mField_0x3A4.x;
    mpPos->z += mField_0x398.z + mField_0x3A4.z;
}

dBgS_ObjAcch::dBgS_ObjAcch() {
    SetObj();
}
dBgS_ObjAcch::~dBgS_ObjAcch() {}

dBgS_LinkAcch::dBgS_LinkAcch() {
    SetLink();
}
dBgS_LinkAcch::~dBgS_LinkAcch() {}

dBgS_BombAcch::dBgS_BombAcch() {
    SetBomb();
}
dBgS_BombAcch::~dBgS_BombAcch() {}
