#include "d/col/bg/d_bg_w_time.h"

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

bool dBgWTime::LineCheck(cBgS_LinChk *) {}

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

bool dBgWTime::SphChk(dBgS_SphChk *, void *) {}

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
