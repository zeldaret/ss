#include "d/col/bg/d_bg_w_kcol.h"

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

bool dBgWKCol::ChkPolyThrough(dBgPc *pPoly, cBgS_Chk *pChk) const {}
