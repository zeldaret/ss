#include "d/col/bg/d_bg_w_time.h"

#include "d/col/bg/d_bg_s_lin_chk.h"
#include "d/col/bg/d_bg_s_sph_chk.h"
#include "d/col/bg/d_bg_w.h"

dBgWTime::dBgWTime() {}

dBgWTime::~dBgWTime() {}

bool dBgWTime::GetTopUnder(f32 *pOutTop, f32 *pOutUnder) const {
    f32 height = mCyl.mHeight;
    f32 posY = mCyl.GetC().y;
    *pOutTop = posY + height;
    *pOutUnder = posY;
}

bool dBgWTime::ChkNotReady() const {
    return false;
}

bool dBgWTime::ChkLock() const {
    return false;
}

bool dBgWTime::ChkMoveBg() const {
    return true;
}

u32 dBgWTime::ChkMoveFlag() const {
    return false;
}

void dBgWTime::GetTriPla(cBgS_PolyInfo const &info, cM3dGPla *pPla) const {
    *pPla = info.GetPlane();
}

// part of c_m3d but idk a name for it yet
extern "C" bool fn_803350A0(const cM3dGPla *, mMtx_c *);

bool dBgWTime::GetTriPnt(cBgS_PolyInfo const &info, mVec3_c *pA, mVec3_c *pB, mVec3_c *pC) const {
    mMtx_c mtx;

    if (!fn_803350A0(&info.GetPlane(), &mtx)) {
        return false;
    }

    pA->set(0.f, 0.f, 1.f);
    pB->set(1.f, 0.f, -1.f);
    pC->set(-1.f, 0.f, -1.f);

    PSMTXMultVec(mtx, *pA, *pA);
    PSMTXMultVec(mtx, *pB, *pB);
    PSMTXMultVec(mtx, *pC, *pC);

    *pA += info.GetField0x20();
    *pB += info.GetField0x20();
    *pC += info.GetField0x20();

    return true;
}

const cM3dGAab *dBgWTime::GetBnd() const {
    return &mBnd;
}

u32 dBgWTime::GetGrpInf(cBgS_PolyInfo const &) const {
    return 0;
}

void dBgWTime::OffMoveFlag() {}

void dBgWTime::vt_0x3C() {
    mPartitionInfo.fn_803391f0(&mBnd);
}

bool dBgWTime::LineCheck(cBgS_LinChk *pLine) {
    mVec3_c &linStart = (mVec3_c &)pLine->GetLinStart();
    mVec3_c &linEnd = (mVec3_c &)pLine->GetLinEnd();

    bool bStart = mCyl.Cross(linStart);
    bool bEnd = mCyl.Cross(linEnd);

    if (bStart && bEnd) {
        return false;
    }

    if (!bStart && bEnd) {
        if (!pLine->ChkFrontFlag()) {
            return false;
        }
    } else if (bStart && !bEnd) {
        if (!pLine->ChkBackFlag()) {
            return false;
        }
    } else if (!pLine->ChkFrontFlag()) {
        return false;
    }

    cM3dGLin lin;
    lin.Set(linStart, linEnd);
    mVec3_c vec0;
    mVec3_c vec1;
    if (!cM3d_Cross_CylLin(&mCyl, &lin, vec0, vec1)) {
        return false;
    }
    if (!fn_803537a0(&vec0, pLine)) {
        return false;
    }
    fn_80353a00(pLine, &vec0);
    linEnd = vec0;
    return true;
}

bool dBgWTime::GroundCross(cBgS_GndChk *) {
    return false;
}

void dBgWTime::ShdwDraw(cBgS_ShdwDraw *) {}

void dBgWTime::CaptPoly(dBgS_CaptPoly &) {}

bool dBgWTime::WallCorrect(dBgS_Acch *) {}

bool dBgWTime::WallCorrectSort(dBgS_Acch *pAcch) {
    return WallCorrect(pAcch);
}

bool dBgWTime::RoofChk(dBgS_RoofChk *) {
    return false;
}

bool dBgWTime::SplGrpChk(dBgS_SplGrpChk *) {
    return false;
}

bool dBgWTime::SphChk(dBgS_SphChk *pSph, void *pDat) {
    mVec3_c a, b, c;
    cM3dGPla pla;
    mVec3_c cylSphDiff = mCyl.mCenter - pSph->mCenter;
    f32 SphRad = pSph->mRadius;
    f32 dist = EGG::Math<f32>::sqrt(cylSphDiff.x * cylSphDiff.x + cylSphDiff.z * cylSphDiff.z);
    f32 CylRad = mCyl.mRadius;
    if (dist > SphRad + CylRad || dist < CylRad - SphRad) {
        return false;
    }
    mVec3_c diff = pSph->mCenter - mCyl.mCenter;
    diff.y = 0.0f;

    f32 mag = VEC3Len(diff);
    if (cM3d_IsZero(mag)) {
        return false;
    }

    diff *= 1.0f / mag;
    diff *= mCyl.mRadius;

    mVec3_c adj;

    adj.y = pSph->mCenter.y + diff.y;
    adj.x = mCyl.mCenter.x + diff.x;
    adj.z = mCyl.mCenter.z + diff.z;

    if (!fn_803537a0(&cylSphDiff, pSph)) {
        fn_80353a00(pSph, &adj);

        GetTriPnt(*pSph, &a, &b, &c);

        sVtx[0].x = a.x;
        sVtx[0].y = a.y;
        sVtx[0].z = a.z;
        sVtx[1].x = b.x;
        sVtx[1].y = b.y;
        sVtx[1].z = b.z;
        sVtx[2].x = c.x;
        sVtx[2].y = c.y;
        sVtx[2].z = c.z;
        pla = pSph->GetPlane();
        pSph->mField_0x80 = dist;
        pSph->mField_0x84.set(adj.x, adj.y, adj.z);
        pSph->mCallback(pSph, sVtx, 0, 1, 2, &pla, pDat);
        return true;
    }
    return false;
}

s32 dBgWTime::GetGrpRoomIndex(cBgS_PolyInfo const &) const {
    return 0x3F;
}

s32 dBgWTime::GetExitId(cBgS_PolyInfo const &) {
    return 0xFF;
}

s32 dBgWTime::GetZTargetThrough(cBgS_PolyInfo const &) {
    return 0;
}

int dBgWTime::GetSpecialCode(cBgS_PolyInfo const &) {
    return 0;
}

int dBgWTime::GetSpecialCode(int) {
    return 0;
}

int dBgWTime::GetCode0_0x30000000(cBgS_PolyInfo const &) {
    return 0;
}

u32 dBgWTime::GetPolyObjThrough(int) {
    return 0;
}

u32 dBgWTime::GetPolyCamThrough(int) {
    return 0;
}

u32 dBgWTime::GetPolyLinkThrough(int) {
    return 0;
}

u32 dBgWTime::GetPolyArrowThrough(int) {
    return 0;
}

u32 dBgWTime::GetPolySlingshotThrough(int) {
    return 0;
}

u32 dBgWTime::GetPolyBeetleThrough(int) {
    return 0;
}

u32 dBgWTime::GetPolyClawshotThrough(int) {
    return 0;
}

u32 dBgWTime::GetPolyBombThrough(int) {
    return 0;
}

u32 dBgWTime::GetPolyWhipThrough(int) {
    return 0;
}

u32 dBgWTime::GetShdwThrough(int) {
    return 0;
}

u32 dBgWTime::GetUnderwaterRoofCode(int) {
    return 1;
}

int dBgWTime::GetCode0_0x80000000(cBgS_PolyInfo const &) {
    return 0;
}

int dBgWTime::GetLinkNo(cBgS_PolyInfo const &) {
    return 0xFF;
}

s32 dBgWTime::GetWallCode(cBgS_PolyInfo const &) {
    return 2 /* Cannot Run Up */;
}

int dBgWTime::GetPolyAtt0Material(cBgS_PolyInfo const &) {
    return 0;
}

int dBgWTime::GetPolyAtt0(cBgS_PolyInfo const &) {
    return 0;
}

int dBgWTime::GetPolyAtt1(cBgS_PolyInfo const &) {
    return 0;
}

int dBgWTime::GetGroundCode(cBgS_PolyInfo const &) {
    return 6 /* POLY_GROUND_6 */;
}

u32 dBgWTime::GetCode1_0x02000000(int) {
    return 0;
}

u32 dBgWTime::GetCode1_0x04000000(int) {
    return 0;
}

u32 dBgWTime::GetCode1_0x08000000(int) {
    return 0;
}

u32 dBgWTime::GetLightingCode(cBgS_PolyInfo const &) {
    return 0xF;
}

s32 dBgWTime::GetCamMoveBG(cBgS_PolyInfo const &) {
    return 0;
}

s32 dBgWTime::GetRoomCamId(cBgS_PolyInfo const &) {
    return 0;
}

s32 dBgWTime::GetRoomPathId(cBgS_PolyInfo const &) {
    return 0xFF;
}

s32 dBgWTime::GetRoomPathPntNo(cBgS_PolyInfo const &) {
    return 0xFF;
}

void dBgWTime::CrrPos(cBgS_PolyInfo const &, void *, bool, mVec3_c *, mAng3_c *, mAng3_c *) {}

void dBgWTime::TransPos(cBgS_PolyInfo const &, void *, bool, mVec3_c *, mAng3_c *, mAng3_c *) {}

void dBgWTime::MatrixCrrPos(cBgS_PolyInfo const &, void *, bool, mVec3_c *, mAng3_c *, mAng3_c *) {}

bool dBgWTime::GetIsDraw(int) {
    return false;
}

void dBgWTime::DrawOnMap(int, bool) {}
