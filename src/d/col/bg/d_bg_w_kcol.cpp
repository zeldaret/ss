#include "d/col/bg/d_bg_w_kcol.h"

#include "d/col/bg/d_bg_s_gnd_chk.h"
#include "d/col/bg/d_bg_s_lin_chk.h"

dBgWKCol::dBgWKCol() : mpKCHead(nullptr) {
    mNumPrism = 0;
    mField0x15C = 0;
}

dBgWKCol::~dBgWKCol() {
    mpKCHead = nullptr;
    mField0x15C = 0;
}

void dBgWKCol::initKCollision(void *pFile) {
    KCol_Header *pHeader = (KCol_Header *)pFile;
    pkcdata *pData = (pkcdata *)pFile;

    pData->mpPositionData = (mVec3_c *)((u32)pFile + pHeader->pos_data_offset);
    pData->mpNormalData = (mVec3_c *)((u32)pFile + pHeader->nrm_data_offset);
    pData->mpPrismData = (KC_PrismData *)((u32)pFile + pHeader->prism_data_offset);
    pData->mpBlockData = (u32 *)((u32)pFile + pHeader->block_data_offset);
}

void dBgWKCol::Set(void *pKcl, void *pPlc) {
    ClrDBgWBase();
    mpKCHead = (pkcdata *)pKcl;
    mNumPrism = ((u32)mpKCHead->mpBlockData - (u32)mpKCHead->mpPrismData) / sizeof(KC_PrismData) + 1;
    mCode.setBase(pPlc);
    // Z,Y,X important?
    f32 z = mpKCHead->mAreaMinPos.z;
    f32 y = mpKCHead->mAreaMinPos.y;
    f32 x = mpKCHead->mAreaMinPos.x;
    u32 mz = mpKCHead->mAreaWidthMaskZ;
    u32 my = mpKCHead->mAreaWidthMaskY;
    u32 mx = mpKCHead->mAreaWidthMaskX;
    mBnd.Set(mVec3_c(x, y, z), mVec3_c(x + ~mx, y + ~my, z + ~mz));

    vt_0x3C();
    mAreaWidthMaskX = ~mpKCHead->mAreaWidthMaskX;
    mAreaWidthMaskY = ~mpKCHead->mAreaWidthMaskY;
    mAreaWidthMaskZ = ~mpKCHead->mAreaWidthMaskZ;
}

void dBgWKCol::GetTriNrm(KC_PrismData *pd, mVec3_c **ppTriNrm) const {
    *ppTriNrm = GetTriNrm(pd);
}

bool dBgWKCol::ChkNotReady() const {
    return mpKCHead == nullptr;
}

bool dBgWKCol::ChkLock() const {
    return false;
}

bool dBgWKCol::ChkMoveBg() const {
    return false;
}

u32 dBgWKCol::ChkMoveFlag() const {
    return 0;
}

void dBgWKCol::GetTriPla(cBgS_PolyInfo const &polyInf, cM3dGPla *plane) const {
    mVec3_c *pFaceNrm;
    KC_PrismData *pPrismData = GetPrismData(polyInf.GetPolyIndex());
    GetTriNrm(pPrismData, &pFaceNrm);

    mVec3_c *pPos = GetTriPos(pPrismData);
    plane->mNormal = *pFaceNrm;
    plane->mD = -VEC3Dot(plane->mNormal, *pPos);
}

bool dBgWKCol::GetTriPnt(cBgS_PolyInfo const &polyInf, mVec3_c *v1, mVec3_c *v2, mVec3_c *v3) const {
    return GetTriPnt(polyInf.GetPolyIndex(), v1, v2, v3);
}

bool dBgWKCol::GetTriPnt(int poly_index, mVec3_c *v1, mVec3_c *v2, mVec3_c *v3) const {
    return GetTriPnt(GetPrismData(poly_index), v1, v2, v3);
}

bool dBgWKCol::GetTriPnt(KC_PrismData const *pPrism, mVec3_c *v1, mVec3_c *v2, mVec3_c *v3) const {
    *v1 = *GetTriPos(pPrism);
    mVec3_c *pFaceNrm = GetTriNrm(pPrism);
    mVec3_c *pEdgeNrm1 = GetEdgeNrm1(pPrism);
    mVec3_c *pEdgeNrm2 = GetEdgeNrm2(pPrism);
    mVec3_c *pEdgeNrm3 = GetEdgeNrm3(pPrism);

    mVec3_c face_nrm1;
    PSVECCrossProduct(*pFaceNrm, *pEdgeNrm1, face_nrm1);

    // TODO
}

const cM3dGAab *dBgWKCol::GetBnd() const {
    return &mBnd;
}

u32 dBgWKCol::GetGrpInf(cBgS_PolyInfo const &pi) const {
    return mCode.getGrpCode(GetPrismData(pi.GetPolyIndex())->mAttribute);
}

void dBgWKCol::OffMoveFlag() {}

bool dBgWKCol::ChkPolyThrough(int id, dBgPc *, cBgS_Chk *) {}

bool dBgWKCol::ChkPolyThroughGnd(int id, dBgPc *, cBgS_Chk *) {}

void dBgWKCol::vt_0x3C() {
    mPartitionInfo.fn_803391f0(&mBnd);
}

bool dBgWKCol::LineCheck(cBgS_LinChk *pLine) {
    mVec3_c diff;
    VEC3Sub(diff, pLine->mLin.GetStartP(), pLine->mLin.GetEndP());
    f32 mag = PSVECMag(diff);
    if (mag < 500.0f) {
        int tmp0 = (int)(mag * 0.002f) + 1;
        f32 tmp1 = mag / (tmp0 + 1);
        VEC3Scale(diff, diff, tmp1 / mag);
    }

    // TODO Later lmao
}

bool dBgWKCol::GroundCross(cBgS_GndChk *pGnd) {}

void dBgWKCol::ShdwDraw(cBgS_ShdwDraw *pShdw) {}

void dBgWKCol::CaptPoly(dBgS_CaptPoly &capt) {}

bool dBgWKCol::WallCorrectSort(dBgS_Acch *pAcch) {}

bool dBgWKCol::WallCorrect(dBgS_Acch *pAcch) {}

bool dBgWKCol::RoofChk(dBgS_RoofChk *pRoof) {}

bool dBgWKCol::SplGrpChk(dBgS_SplGrpChk *) {}

bool dBgWKCol::SphChk(dBgS_SphChk *, void *) {}

bool dBgWKCol::GetTopUnder(f32 *pOutTop, f32 *pOutUnder) const {}

s32 dBgWKCol::GetGrpRoomIndex(cBgS_PolyInfo const &) const {}

s32 dBgWKCol::GetExitId(cBgS_PolyInfo const &) {}

s32 dBgWKCol::GetZTargetThrough(cBgS_PolyInfo const &) {}

int dBgWKCol::GetSpecialCode(cBgS_PolyInfo const &) {}

int dBgWKCol::GetSpecialCode(int) {}

int dBgWKCol::GetCode0_0x30000000(cBgS_PolyInfo const &) {}

int dBgWKCol::GetCode0_0x80000000(cBgS_PolyInfo const &) {}

u32 dBgWKCol::GetPolyObjThrough(int) {}

u32 dBgWKCol::GetPolyCamThrough(int) {}

u32 dBgWKCol::GetPolyLinkThrough(int) {}

u32 dBgWKCol::GetPolyArrowThrough(int) {}

u32 dBgWKCol::GetPolySlingshotThrough(int) {}

u32 dBgWKCol::GetPolyBeetleThrough(int) {}

u32 dBgWKCol::GetPolyClawshotThrough(int) {}

u32 dBgWKCol::GetPolyBombThrough(int) {}

u32 dBgWKCol::GetPolyWhipThrough(int) {}

u32 dBgWKCol::GetUnderwaterRoofCode(int) {}

u32 dBgWKCol::GetShdwThrough(int) {}

int dBgWKCol::GetLinkNo(cBgS_PolyInfo const &) {}

s32 dBgWKCol::GetWallCode(cBgS_PolyInfo const &) {}

int dBgWKCol::GetPolyAtt0Material(cBgS_PolyInfo const &) {}

int dBgWKCol::GetPolyAtt1(cBgS_PolyInfo const &) {}

int dBgWKCol::GetGroundCode(cBgS_PolyInfo const &) {}

u32 dBgWKCol::GetCode1_0x02000000(int) {}

u32 dBgWKCol::GetCode1_0x04000000(int) {}

u32 dBgWKCol::GetCode1_0x08000000(int) {}

u32 dBgWKCol::GetLightingCode(cBgS_PolyInfo const &) {}

s32 dBgWKCol::GetCamMoveBG(cBgS_PolyInfo const &) {}

s32 dBgWKCol::GetRoomCamId(cBgS_PolyInfo const &) {}

s32 dBgWKCol::GetRoomPathId(cBgS_PolyInfo const &) {}

s32 dBgWKCol::GetRoomPathPntNo(cBgS_PolyInfo const &) {}

void dBgWKCol::CrrPos(cBgS_PolyInfo const &, void *, bool, mVec3_c *, mAng3_c *, mAng3_c *) {}

void dBgWKCol::TransPos(cBgS_PolyInfo const &, void *, bool, mVec3_c *, mAng3_c *, mAng3_c *) {}

void dBgWKCol::MatrixCrrPos(cBgS_PolyInfo const &, void *, bool, mVec3_c *, mAng3_c *, mAng3_c *) {}

int dBgWKCol::GetMapCode(int polyIdx, KC_PrismData *prism, int *pOut) {}

bool dBgWKCol::UpdateDraw(mAllocator_c *alloc) {
    // TODO
    // To Generate Weak Ct and Dt of KC_Tri
    mField0x15C = new (alloc) KC_Tri[mNumPrism];
}

int dBgWKCol::GetPolyAtt0(cBgS_PolyInfo const &) {}

bool dBgWKCol::GetIsDraw(int) {}

void dBgWKCol::DrawMapSection(int, f32, f32, f32) {}

void dBgWKCol::DrawOnMap(int, bool) {}

int dBgWKCol::GetPrismIdx(const KC_PrismData *pPrismData) {
    return (pPrismData - mpKCHead->mpPrismData);
}
