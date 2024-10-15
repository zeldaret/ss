#include "d/col/bg/d_bg_w_kcol.h"

#include "d/col/bg/d_bg_s_gnd_chk.h"
#include "d/col/bg/d_bg_s_lin_chk.h"
#include "toBeSorted/scgame.h"

char STAGE_FS_A[] = "D201";
char STAGE_FS_B[] = "D201_1";
char STAGE_SSH[] = "D301";

dBgWKCol::dBgWKCol() : mpKCHead(nullptr) {
    mNumPrism = 0;
    mpTri = 0;
}

dBgWKCol::~dBgWKCol() {
    mpKCHead = nullptr;
    mpTri = 0;
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

s32 dBgWKCol::GetGrpRoomIndex(cBgS_PolyInfo const &info) const {}

s32 dBgWKCol::GetExitId(cBgS_PolyInfo const &info) {}

s32 dBgWKCol::GetZTargetThrough(cBgS_PolyInfo const &info) {}

int dBgWKCol::GetSpecialCode(cBgS_PolyInfo const &info) {}

int dBgWKCol::GetSpecialCode(int polyIdx) {}

int dBgWKCol::GetCode0_0x30000000(cBgS_PolyInfo const &info) {}

int dBgWKCol::GetCode0_0x80000000(cBgS_PolyInfo const &info) {}

u32 dBgWKCol::GetPolyObjThrough(int polyIdx) {
    return GetPolyCode(polyIdx)->getObjThrough();
}

u32 dBgWKCol::GetPolyCamThrough(int polyIdx) {
    return GetPolyCode(polyIdx)->getCamThrough();
}

u32 dBgWKCol::GetPolyLinkThrough(int polyIdx) {
    return GetPolyCode(polyIdx)->getLinkThrough();
}

u32 dBgWKCol::GetPolyArrowThrough(int polyIdx) {
    return GetPolyCode(polyIdx)->getArrowThrough();
}

u32 dBgWKCol::GetPolySlingshotThrough(int polyIdx) {
    return GetPolyCode(polyIdx)->getSlingshotThrough();
}

u32 dBgWKCol::GetPolyBeetleThrough(int polyIdx) {
    return GetPolyCode(polyIdx)->getBeetleThrough();
}

u32 dBgWKCol::GetPolyClawshotThrough(int polyIdx) {
    return GetPolyCode(polyIdx)->getClawshotThrough();
}

u32 dBgWKCol::GetPolyBombThrough(int polyIdx) {
    return GetPolyCode(polyIdx)->getBombThrough();
}

u32 dBgWKCol::GetPolyWhipThrough(int polyIdx) {
    return GetPolyCode(polyIdx)->getWhipThrough();
}

u32 dBgWKCol::GetUnderwaterRoofCode(int polyIdx) {
    return GetPolyCode(polyIdx)->getUnderwaterRoof();
}

u32 dBgWKCol::GetShdwThrough(int polyIdx) {
    return GetPolyCode(polyIdx)->getShdwThrough();
}

int dBgWKCol::GetLinkNo(cBgS_PolyInfo const &info) {
    return GetPolyCode(info.GetPolyIndex())->getLinkNo();
}

s32 dBgWKCol::GetWallCode(cBgS_PolyInfo const &info) {
    return GetPolyCode(info.GetPolyIndex())->getWallCode();
}

int dBgWKCol::GetPolyAtt0Material(cBgS_PolyInfo const &info) {
    int mat = GetPolyCode(info.GetPolyIndex())->getAtt0Code();
    if (mat >= 0x20) {
        mat -= 0x20;
    }
    return mat;
}

int dBgWKCol::GetPolyAtt1(cBgS_PolyInfo const &info) {
    return GetPolyCode(info.GetPolyIndex())->getAtt1Code();
}

int dBgWKCol::GetGroundCode(cBgS_PolyInfo const &info) {
    return GetPolyCode(info.GetPolyIndex())->getGroundCode();
}

u32 dBgWKCol::GetCode1_0x02000000(int polyIdx) {
    return GetPolyCode(polyIdx)->getCode1_0x02000000();
}

u32 dBgWKCol::GetCode1_0x04000000(int polyIdx) {
    return GetPolyCode(polyIdx)->getCode1_0x04000000();
}

u32 dBgWKCol::GetCode1_0x08000000(int polyIdx) {
    return GetPolyCode(polyIdx)->getCode1_0x08000000();
}

u32 dBgWKCol::GetLightingCode(cBgS_PolyInfo const &info) {
    return GetPolyCode(info.GetPolyIndex())->getLightingCode();
}

s32 dBgWKCol::GetCamMoveBG(cBgS_PolyInfo const &info) {
    return GetPolyCode(info.GetPolyIndex())->getCamMoveBG();
}

s32 dBgWKCol::GetRoomCamId(cBgS_PolyInfo const &info) {
    return GetPolyCode(info.GetPolyIndex())->getRoomCam();
}

s32 dBgWKCol::GetRoomPathId(cBgS_PolyInfo const &info) {
    return GetPolyCode(info.GetPolyIndex())->getRoomPath();
}

s32 dBgWKCol::GetRoomPathPntNo(cBgS_PolyInfo const &info) {
    return GetPolyCode(info.GetPolyIndex())->getRoomPathPnt();
}

void dBgWKCol::CrrPos(cBgS_PolyInfo const &, void *, bool, mVec3_c *, mAng3_c *, mAng3_c *) {}

void dBgWKCol::TransPos(cBgS_PolyInfo const &, void *, bool, mVec3_c *, mAng3_c *, mAng3_c *) {}

void dBgWKCol::MatrixCrrPos(cBgS_PolyInfo const &, void *, bool, mVec3_c *, mAng3_c *, mAng3_c *) {}

int dBgWKCol::GetMapCode(KC_PrismData *prism, int *pOut) {}

bool dBgWKCol::UpdateDraw(mAllocator_c *alloc) {
    // TODO
    // To Generate Weak Ct and Dt of KC_Tri
    mpTri = new (alloc) KC_Tri[mNumPrism];
    if (mpTri == nullptr) {
        return false;
    }

    KC_PrismData *startPrism = mpKCHead->mpPrismData;
    KC_PrismData *currPrism = startPrism + 1;
    mBnd.ClearForMinMax();
    do {
        //
        mVec3_c a, b, c;
        if (!GetTriPnt(currPrism, &a, &b, &c)) {
            a = mpKCHead->mpPositionData[currPrism->mPosIdx];
            b = a;
            c = a;
        }
        int idx = currPrism - mpKCHead->mpPrismData;
        mpTri[idx].mA = a;
        mpTri[idx].mB = b;
        mpTri[idx].mC = c;

        mBnd.SetMinMax(a);
        mBnd.SetMinMax(b);
        mBnd.SetMinMax(c);
        currPrism++;
    } while (currPrism < (KC_PrismData *)mpKCHead->mpBlockData);
    mBnd.PlusR(1.0f);
    vt_0x3C();

    ClearMapCount();

    currPrism = mpKCHead->mpPrismData + 1;
    do {
        int mapCode = 0;
        if (!GetMapCode(currPrism, &mapCode)) {
            mMapRelated[mapCode].mCount++;
        }
        currPrism++;
    } while (currPrism < (KC_PrismData *)mpKCHead->mpBlockData);

    for (int i = 0; i < 0x1F; i++) {
        if (mMapRelated[i].mCount > 0) {
            mMapRelated[i].mpIdx = new (alloc) int[mMapRelated[i].mCount];
            if (mMapRelated[i].mpIdx == nullptr) {
                return false;
            }
        }
    }

    int local_cnt[0x1F];
    for (int i = 0; i < 0x1F; i++) {
        local_cnt[i] = 0;
    }
    currPrism = mpKCHead->mpPrismData + 1;
    do {
        int mapCode = 0;
        if (!GetMapCode(currPrism, &mapCode)) {
            mMapRelated[mapCode].mpIdx[local_cnt[mapCode]] = currPrism - mpKCHead->mpPrismData;
            local_cnt[mapCode]++;
        }
        currPrism++;
    } while (currPrism < (KC_PrismData *)mpKCHead->mpBlockData);

    return true;
}

int dBgWKCol::GetPolyAtt0(cBgS_PolyInfo const &info) {
    return GetPolyCode(info.GetPolyIndex())->getAtt0Code();
}

bool dBgWKCol::GetIsDraw(int idx) {
    if (mMapRelated[idx].mCount <= 0) {
        return false;
    }
    return mpTri != nullptr;
}

void dBgWKCol::DrawMapSection(int idx, f32 yPos, f32 upper, f32 lower) {
    int all = mMapRelated[idx].mCount;
    int count = 0;
    for (int i = 0; i < all; i++) {
        KC_Tri &tri = mpTri[mMapRelated[idx].mpIdx[i]];
        if (
            // If Tri height is not above or below the limits
            !((tri.mA.y > upper) && (tri.mB.y > upper) && (tri.mC.y > upper)) &&
            !((tri.mA.y < lower) && (tri.mB.y < lower) && (tri.mC.y < lower))) {
            count++;
        }
    };

    GXBegin(GX_TRIANGLES, GX_VTXFMT0, count * 3);
    for (int i = 0; i < all; i++) {
        KC_Tri &tri = mpTri[mMapRelated[idx].mpIdx[i]];
        if (
            // If Tri height is not above or below the limits
            !((tri.mA.y > upper) && (tri.mB.y > upper) && (tri.mC.y > upper)) &&
            !((tri.mA.y < lower) && (tri.mB.y < lower) && (tri.mC.y < lower))) 
        {
            f32 BAx = tri.mB.x - tri.mA.x;
            f32 CBx = tri.mC.x - tri.mB.x;
            f32 ACx = tri.mA.x - tri.mC.x;

            f32 BAz = tri.mB.z - tri.mA.z;
            f32 CBz = tri.mC.z - tri.mB.z;
            f32 ACz = tri.mA.z - tri.mC.z;

            f32 tmp0 = nw4r::math::FSqrt(BAx * BAx + BAz * BAz);
            f32 tmp1 = nw4r::math::FSqrt(CBx * CBx + CBz * CBz);
            f32 tmp2 = nw4r::math::FSqrt(ACx * ACx + ACz * ACz);

            tmp0 = cM3d_IsZero(tmp0) ? 0.0f : 10.0f / tmp0;
            tmp1 = cM3d_IsZero(tmp1) ? 0.0f : 10.0f / tmp1;
            tmp2 = cM3d_IsZero(tmp2) ? 0.0f : 10.0f / tmp2;

            f32 t0x = BAx * tmp0;
            f32 t0z = BAz * tmp0;

            f32 t1z = CBz * tmp1;
            f32 t1x = CBx * tmp1;

            f32 t2x = ACx * tmp2;
            f32 t2z = ACz * tmp2;

            f32 x0 = t2x + (tri.mA.x - t0x);
            f32 z0 = t2z + (tri.mA.z - t0z);

            f32 x2 = t0x + (tri.mB.x - t1x);
            f32 z2 = t0z + (tri.mB.z - t1z);

            f32 x1 = t1x + (tri.mC.x - t2x);
            f32 z1 = t1z + (tri.mC.z - t2z);

            GXPosition3f32(x0, yPos, z0);
            GXPosition3f32(x1, yPos, z1);
            GXPosition3f32(x2, yPos, z2);
        }
    }
}

void dBgWKCol::DrawOnMap(int idx, bool param2) {
    // uhhhhhh

    if (!GetIsDraw(idx)) {
        return;
    }

    if (param2) {
        // FS Room 9 /* Spiral Staircase  */
        if (GetRoomId() == 9 && (ScGame::isCurrentStage(STAGE_FS_A) || ScGame::isCurrentStage(STAGE_FS_B))) {
            int all = mMapRelated[idx].mCount;
            mVec3_c AB, CA, BC;
            mVec3_c v0, v1, v2;
            // Pass 1
            int count = 0;
            for (int i = 0; i < all; i++) {
                KC_Tri &tri = mpTri[mMapRelated[idx].mpIdx[i]];
                if (!((tri.mA.y < -550.0f) && (tri.mB.y < -550.0f) && (tri.mC.y < -550.0f))) {
                    count++;
                }
            }
            GXBegin(GX_TRIANGLES, GX_VTXFMT0, count * 3);
            for (int i = 0; i < all; i++) {
                KC_Tri &tri = mpTri[mMapRelated[idx].mpIdx[i]];
                if (!((tri.mA.y < -550.0f) && (tri.mB.y < -550.0f) && (tri.mC.y < -550.0f))) {
                    VEC3Sub(AB, tri.mB, tri.mA);
                    VEC3Sub(CA, tri.mA, tri.mC);
                    VEC3Sub(BC, tri.mC, tri.mB);

                    f32 len0 = VEC3Len(AB);
                    f32 len1 = VEC3Len(CA);
                    f32 len2 = VEC3Len(BC);

                    VEC3Scale(AB, AB, 10.0f / len0);
                    VEC3Scale(CA, CA, 10.0f / len1);
                    VEC3Scale(BC, BC, 10.0f / len2);

                    VEC3Sub(v0, tri.mA, AB);
                    VEC3Sub(v1, tri.mC, CA);
                    VEC3Sub(v2, tri.mB, BC);

                    VEC3Add(v0, v0, CA);
                    VEC3Add(v1, v1, BC);
                    VEC3Add(v2, v2, AB);

                    GXPosition3f32(v0.x, v0.y, v0.z);
                    GXPosition3f32(v1.x, v1.y, v1.z);
                    GXPosition3f32(v2.x, v2.y, v2.z);
                }
            }
            // Pass 2
            count = 0;
            for (int i = 0; i < all; i++) {
                KC_Tri &tri = mpTri[mMapRelated[idx].mpIdx[i]];
                if (!((tri.mA.y > -550.0f) && (tri.mB.y > -550.0f) && (tri.mC.y > -550.0f))) {
                    count++;
                }
            }
            GXBegin(GX_TRIANGLES, GX_VTXFMT0, count * 3);
            for (int i = 0; i < all; i++) {
                KC_Tri &tri = mpTri[mMapRelated[idx].mpIdx[i]];
                if (!((tri.mA.y > -550.0f) && (tri.mB.y > -550.0f) && (tri.mC.y > -550.0f))) {
                    VEC3Sub(AB, tri.mB, tri.mA);
                    VEC3Sub(CA, tri.mA, tri.mC);
                    VEC3Sub(BC, tri.mC, tri.mB);

                    f32 len0 = VEC3Len(AB);
                    f32 len1 = VEC3Len(CA);
                    f32 len2 = VEC3Len(BC);

                    VEC3Scale(AB, AB, 10.0f / len0);
                    VEC3Scale(CA, CA, 10.0f / len1);
                    VEC3Scale(BC, BC, 10.0f / len2);

                    VEC3Sub(v0, tri.mA, AB);
                    VEC3Sub(v2, tri.mB, BC);
                    VEC3Sub(v1, tri.mC, CA);

                    VEC3Add(v0, v0, CA);
                    VEC3Add(v1, v1, BC);
                    VEC3Add(v2, v2, AB);

                    // v0.y = (v0.y + 6500.0f) * 0.1680672f - 6500.0f;
                    // v1.y = (v1.y + 6500.0f) * 0.1680672f - 6500.0f;
                    // v2.y = (v2.y + 6500.0f) * 0.1680672f - 6500.0f;

                    GXPosition3f32(v0.x, (v0.y + 6500.0f) * 0.1680672f - 6500.0f, v0.z);
                    GXPosition3f32(v1.x, (v1.y + 6500.0f) * 0.1680672f - 6500.0f, v1.z);
                    GXPosition3f32(v2.x, (v2.y + 6500.0f) * 0.1680672f - 6500.0f, v2.z);
                    // GXPosition3f32(v0.x, v0.y, v0.z);
                    // GXPosition3f32(v1.x, v1.y, v1.z);
                    // GXPosition3f32(v2.x, v2.y, v2.z);
                }
            }
        }
        // Normal
        else {
            int all = mMapRelated[idx].mCount;
            GXBegin(GX_TRIANGLES, GX_VTXFMT0, all * 3);
            for (int i = 0; i < all; i++) {
                KC_Tri &tri = mpTri[mMapRelated[idx].mpIdx[i]];

                mVec3_c AB, CA, BC;
                mVec3_c v0, v1, v2;

                VEC3Sub(AB, tri.mB, tri.mA);
                VEC3Sub(CA, tri.mA, tri.mC);
                VEC3Sub(BC, tri.mC, tri.mB);

                f32 len0 = VEC3Len(AB);
                f32 len1 = VEC3Len(CA);
                f32 len2 = VEC3Len(BC);

                VEC3Scale(AB, AB, 10.0f / len0);
                VEC3Scale(CA, CA, 10.0f / len1);
                VEC3Scale(BC, BC, 10.0f / len2);

                VEC3Sub(v0, tri.mA, AB);
                VEC3Sub(v1, tri.mC, CA);
                VEC3Sub(v2, tri.mB, BC);

                VEC3Add(v0, v0, CA);
                VEC3Add(v1, v1, BC);
                VEC3Add(v2, v2, AB);

                GXPosition3f32(v0.x, v0.y, v0.z);
                GXPosition3f32(v1.x, v1.y, v1.z);
                GXPosition3f32(v2.x, v2.y, v2.z);
            }
        }
    }
    // Sections
    else {
        const cM3dGAab *bnd = GetBnd();
        f32 yPos = (bnd->GetMinP()->y + bnd->GetMaxP()->y) * 0.5f;

        // Sandship
        if (GetRoomId() == 15 && ScGame::isCurrentStage(STAGE_SSH)) {
            yPos = 0.0f;
        }
        if (idx == 0x12 /* Abyss `Common/tex/naraku.bti`)*/) {
            yPos -= 1.0f;
        }
        if (ScGame::isCurrentStage(STAGE_FS_A) || ScGame::isCurrentStage(STAGE_FS_B)) {
            int room = GetRoomId();
            /* FS Room 9 - Spiral Staircse */
            if (room == 9) {
                DrawMapSection(idx, yPos, 1e9f, -550.0f);
                DrawMapSection(idx, -6000.0f, -550.0f, -1e9f);
                return;
            }
            /* FS Room 6 - Bridges */
            if (room == 6) {
                DrawMapSection(idx, yPos, 1e9f, -5500.0f);
                DrawMapSection(idx, -6000.0f, -550.0f, -1e9f);
                return;
            }
        }
        DrawMapSection(idx, yPos, 1e9f, -1e9f);
    }
}

int dBgWKCol::GetPrismIdx(const KC_PrismData *pPrismData) {
    return (pPrismData - mpKCHead->mpPrismData);
}
