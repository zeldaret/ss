#include "d/col/bg/d_bg_w.h"

#include "d/col/bg/d_bg_s.h"
#include "d/col/bg/d_bg_s_acch.h"
#include "d/col/bg/d_bg_w_kcol.h"
#include "d/col/c/c_bg_s_chk.h"
#include "d/col/c/c_bg_s_lin_chk.h"
#include "d/col/c/c_bg_s_shdw_draw.h"
#include "d/d_heap_alloc.h"
#include "toBeSorted/scgame.h"

bool cBgW::sLineCheck;
bool cBgW::sGndCheck;
bool cBgW::sWallCheck;
bool cBgW::sRoofCheck;
bool cBgW::sSplGrpCheck;
bool cBgW::sSphCheck;

mVec3_c dBgW::sWallCorrectPos(0.0f, 0.0f, 0.0f);

cBgW::cBgW()
    : mpMdlMtx(nullptr), mTransVel(0.0f, 0.0f, 0.0f), mpScale(nullptr), mFlags(GLOBAL_e), mRootGroupIdx(-1),
      mpTri(nullptr), mpRwg(nullptr), mpVtxTbl(nullptr), mpBgd(nullptr), mpBlk(nullptr), mpGrp(nullptr),
      mpNodeTree(nullptr), mNeedsFullTransform(true) {
    PSMTXIdentity(mInvMtx);
    PSMTXIdentity(mMtx);
    PSMTXIdentity(mMtxUnk);
}

cBgW::~cBgW() {
    FreeArea();
}

void cBgW::FreeArea() {
    if (mpTri) {
        delete[] mpTri;
    }
    if (mpRwg) {
        delete[] mpRwg;
    }
    if (mpNodeTree) {
        delete[] mpNodeTree;
    }
    if (mpBlk) {
        delete[] mpBlk;
    }
    if (mpGrp) {
        delete[] mpGrp;
    }
    mpTri = nullptr;
    mpRwg = nullptr;
    mpNodeTree = nullptr;
    mpBlk = nullptr;
    mpGrp = nullptr;
    mpVtxTbl = nullptr;
}

u32 cBgW::GetOldInvMtx(mMtx_c *m) {
    return PSMTXInverse(mMtx, *m);
}

void cBgW::UpdateMtx() {
    if (!mpMdlMtx) {
        PSMTXIdentity(mInvMtx);
        PSMTXIdentity(mMtx);
        PSMTXIdentity(mMtxUnk);
    } else {
        PSMTXCopy(*mpMdlMtx, mInvMtx);
        if (mpScale) {
            mMtx_c scale;
            PSMTXScale(scale, mpScale->x, mpScale->y, mpScale->z);
            PSMTXConcat(mInvMtx, scale, mInvMtx);
        }

        PSMTXCopy(mInvMtx, mMtx);
        PSMTXCopy(mInvMtx, mMtxUnk);
    }
}

void cBgW::GlobalVtx() {
    if (mpMdlMtx) {
        if (!mNeedsFullTransform) {
            int i = 0;
            do {
                nw4r::math::VEC3 *vtx = &mpVtxTbl[i];
                VEC3Add(vtx, mTransVel, vtx);
                i++;
            } while (i < mpBgd->mVtxNum);

        } else {
            int i = 0;
            do {
                PSMTXMultVec(mInvMtx, mpBgd->mVtxTbl[i], mpVtxTbl[i]);
                i++;
            } while (i < mpBgd->mVtxNum);
        }
    }
}

bool cBgW::SetVtx() {
    if (mFlags & NO_VTX_TBL_e) {
        mpVtxTbl = nullptr;
    } else if (mFlags & MOVE_BG_e) {
        // Im not sure about this, but it works
        mpVtxTbl = (cBgD_Vtx_t *)new (0x20) Vec[mpBgd->mVtxNum];

        if (!mpVtxTbl) {
            return true;
        }
        if (mFlags & CBGW_UNK_FLAG_40) {
            int i = 0;
            do {
                mpVtxTbl[i].x = 0.0f;
                mpVtxTbl[i].y = 0.0f;
                mpVtxTbl[i].z = 0.0f;
                i++;
            } while (i < mpBgd->mVtxNum);
        }
        GlobalVtx();

    } else {
        mpVtxTbl = mpBgd->mVtxTbl;
    }
    return false;
}

void cBgW::CalcPlane() {
    cBgD_Tri_t *tris = mpBgd->mTriTbl;
    if (mpVtxTbl) {
        if (!mNeedsFullTransform) {
            int i = 0;
            do {
                mpTri[i].mD -= VEC3Dot(mpTri[i].mNormal, mTransVel);
                i++;
            } while (i < mpBgd->mTriNum);
        } else {
            int i = 0;
            do {
                mpTri[i].SetupFrom3Vtx(&mpVtxTbl[tris->mVtxIdx0], &mpVtxTbl[tris->mVtxIdx1], &mpVtxTbl[tris->mVtxIdx2]);
                i++;
                tris++;
            } while (i < mpBgd->mTriNum);
        }
    }
}

bool cBgW::SetTri() {
    mpTri = new cBgW_TriElm[mpBgd->mTriNum];

    if (!mpTri) {
        return true;
    }
    CalcPlane();
    return false;
}

void cBgW::BlckConnect(u16 *pStartIdx, int *pPrevIdx, int idx) {
    if (*pStartIdx == 0xFFFF) {
        *pStartIdx = idx;
    }
    if (*pPrevIdx != 0xFFFF) {
        mpRwg[*pPrevIdx].mNext = idx;
    }
    *pPrevIdx = idx;
    mpRwg[idx].mNext = 0xFFFF;
}

void cBgW::ClassifyPlane() {
    if (!mpVtxTbl) {
        return;
    }

    int i = 0;
    do {
        int startIdx = mpBgd->mBlkTbl[i].field_0x0;
        int endIdx;
        if (i != mpBgd->mBlkNum - 1) {
            endIdx = mpBgd->mBlkTbl[i + 1].field_0x0 - 1;
        } else {
            endIdx = mpBgd->mTriNum - 1;
        }

        mpBlk[i].mRoofIdx = 0xFFFF;
        mpBlk[i].mWallIdx = 0xFFFF;
        mpBlk[i].mGndIdx = 0xFFFF;

        int blkRoofIdx, blkWallIdx, blkGndIdx;
        blkRoofIdx = blkWallIdx = blkGndIdx = 0xFFFF;
        for (int j = startIdx; j <= endIdx; j++) {
            f32 norm_y = mpTri[j].GetNP()->y;

            if (!cM3d_IsZero(mpTri[j].GetNP()->x) || !cM3d_IsZero(mpTri[j].GetNP()->y) ||
                !cM3d_IsZero(mpTri[j].GetNP()->z)) {
                if (cBgW_CheckBGround(norm_y)) {
                    BlckConnect(&mpBlk[i].mGndIdx, &blkGndIdx, j);
                } else if (cBgW_CheckBRoof(norm_y)) {
                    if (!ChkRoofRegist()) {
                        BlckConnect(&mpBlk[i].mRoofIdx, &blkRoofIdx, j);
                    }
                } else {
                    BlckConnect(&mpBlk[i].mWallIdx, &blkWallIdx, j);
                }
            }
        }
        i++;
    } while (i < mpBgd->mBlkNum);
}

void cBgW::MakeBlckTransMinMax(mVec3_c *pMin, mVec3_c *pMax) {
    VEC3Add(pMin, &mTransVel, pMin);
    VEC3Add(pMax, &mTransVel, pMax);
}

void cBgW::MakeBlckMinMax(int vtxIdx, mVec3_c *pMin, mVec3_c *pMax) {
    nw4r::math::VEC3 *vtx = &mpVtxTbl[vtxIdx];

    if (pMin->x > vtx->x) {
        pMin->x = vtx->x;
    }

    if (pMax->x < vtx->x) {
        pMax->x = vtx->x;
    }

    if (pMin->y > vtx->y) {
        pMin->y = vtx->y;
    }

    if (pMax->y < vtx->y) {
        pMax->y = vtx->y;
    }

    if (pMin->z > vtx->z) {
        pMin->z = vtx->z;
    }

    if (pMax->z < vtx->z) {
        pMax->z = vtx->z;
    }
}

int cBgW::GetPolyAtt0(const cBgS_PolyInfo &info) {
    return 0;
}

void cBgW::MakeBlckBnd(int blkIdx, mVec3_c *pMin, mVec3_c *pMax) {
    if (mNeedsFullTransform == 0) {
        MakeBlckTransMinMax(pMin, pMax);
    } else {
        pMin->z = 1e9f;
        pMin->y = 1e9f;
        pMin->x = 1e9f;
        pMax->z = -1e9f;
        pMax->y = -1e9f;
        pMax->x = -1e9f;
        int max;
        int start = mpBgd->mBlkTbl[blkIdx].field_0x0;

        if (blkIdx != mpBgd->mBlkNum - 1) {
            max = mpBgd->mBlkTbl[blkIdx + 1].field_0x0 - 1;
        } else {
            max = mpBgd->mTriNum - 1;
        }
        for (int i = start; i <= max; i++) {
            MakeBlckMinMax(mpBgd->mTriTbl[i].mVtxIdx0, pMin, pMax);
            MakeBlckMinMax(mpBgd->mTriTbl[i].mVtxIdx1, pMin, pMax);
            MakeBlckMinMax(mpBgd->mTriTbl[i].mVtxIdx2, pMin, pMax);
        }

        pMin->x -= 1.0f;
        pMin->y -= 1.0f;
        pMin->z -= 1.0f;
        pMax->x += 1.0f;
        pMax->y += 1.0f;
        pMax->z += 1.0f;
    }
}

void cBgW::MakeNodeTreeRp(int treeIdx) {
    cBgD_Tree_t *treeData = &mpBgd->mTreeTbl[treeIdx];

    if (treeData->mFlag & 1) {
        int child_idx = treeData->mId[0];

        if (child_idx != 0xFFFF) {
            mVec3_c *max = (mVec3_c *)mpNodeTree[treeIdx].GetMaxP();
            mVec3_c *min = (mVec3_c *)mpNodeTree[treeIdx].GetMinP();

            MakeBlckBnd(child_idx, min, max);
        }
    } else {
        mpNodeTree[treeIdx].ClearForMinMax();

        u16 *id = treeData->mId;
        u16 *endIds = &id[8];

        do {
            if (id[0] != 0xFFFF) {
                MakeNodeTreeRp(id[0]);
                mpNodeTree[treeIdx].SetMinMax(*mpNodeTree[id[0]].GetMinP());
                mpNodeTree[treeIdx].SetMinMax(*mpNodeTree[id[0]].GetMaxP());
            }
            id++;
        } while (id < endIds);
    }
}

void cBgW::MakeNodeTreeGrpRp(int grpIdx) {
    if (mpBgd->mGrpTbl[grpIdx].mTreeIdx != 0xFFFF) {
        MakeNodeTreeRp(mpBgd->mGrpTbl[grpIdx].mTreeIdx);
        mpGrp[grpIdx].mAab.SetMin(*mpNodeTree[mpBgd->mGrpTbl[grpIdx].mTreeIdx].GetMinP());
        mpGrp[grpIdx].mAab.SetMax(*mpNodeTree[mpBgd->mGrpTbl[grpIdx].mTreeIdx].GetMaxP());
    }

    s32 child_idx = mpBgd->mGrpTbl[grpIdx].mFirstChild;
    while (child_idx != 0xFFFF) {
        MakeNodeTreeGrpRp(child_idx);
        mpGrp[grpIdx].mAab.SetMin(*mpGrp[child_idx].mAab.GetMinP());
        mpGrp[grpIdx].mAab.SetMax(*mpGrp[child_idx].mAab.GetMaxP());
        child_idx = mpBgd->mGrpTbl[child_idx].mNextSibling;
    }
}

void cBgW::MakeNodeTree() {
    if (mpVtxTbl == nullptr) {
        int i = 0;
        do {
            if (mpBgd->mGrpTbl[i].mParent == 0xFFFF) {
                mRootGroupIdx = i;
                return;
            }
            i++;
        } while (i < mpBgd->mGrpNum);
    } else {
        int i = 0;
        do {
            mpGrp[i].mAab.ClearForMinMax();
            i++;
        } while (i < mpBgd->mGrpNum);

        i = 0;
        do {
            if (mpBgd->mGrpTbl[i].mParent == 0xFFFF) {
                mRootGroupIdx = i;
                MakeNodeTreeGrpRp(i);
                break;
            }
            i++;
        } while (i < mpBgd->mGrpNum);
        i = 0;
        do {
            mpGrp[i].mAab.PlusR(1.0f);
            i++;
        } while (i < mpBgd->mGrpNum);
        vt_0x3C();
    }
}

bool cBgW::ChkMemoryError() {
    if (!mpTri || !mpRwg || !mpBlk || !mpNodeTree || !mpGrp) {
        return true;
    }
    return false;
}
bool cBgW::Set(cBgD_t *pbgd, PLC *pbgpc, u32 flags, mMtx_c *pMdlMtx, mVec3_c *pScale) {
    mFlags = GLOBAL_e;
    mpVtxTbl = nullptr;

    if (pbgd == nullptr) {
        return true;
    }

    mpPolyCodes.setBase(pbgpc);
    mFlags = flags & 0xFF;
    if (mFlags & GLOBAL_e) {
        mpMdlMtx = nullptr;
        mpScale = nullptr;
    } else {
        mpMdlMtx = pMdlMtx;
        mpScale = pScale;
    }

    UpdateMtx();

    mpBgd = pbgd;

    if (SetVtx() || SetTri()) {
        FreeArea();
        return true;
    }
    if (mpRwg) {
        delete[] mpRwg;
    }
    mpRwg = new cBgW_RwgElm[mpBgd->mTriNum];
    if (mpRwg == nullptr) {
        FreeArea();
        return true;
    }

    if (mpBlk) {
        delete[] mpBlk;
    }
    mpBlk = new cBgW_BlkElm[mpBgd->mBlkNum];
    if (mpBlk == nullptr) {
        FreeArea();
        return true;
    }

    if (mpNodeTree) {
        delete[] mpNodeTree;
    }
    mpNodeTree = new cBgW_NodeTree[mpBgd->mTreeNum];
    if (mpNodeTree == nullptr) {
        FreeArea();
        return true;
    }

    if (mpGrp) {
        delete[] mpGrp;
    }
    mpGrp = new cBgW_GrpElm[mpBgd->mGrpNum];
    if (mpGrp == nullptr) {
        FreeArea();
        return true;
    }

    ClassifyPlane();
    mNeedsFullTransform = 1;
    MakeNodeTree();
    return false;
}

bool cBgW::LineCheck(cBgS_LinChk *pLine) {
    sLineCheck = false;
    LineCheckGrpRp(pLine, mRootGroupIdx);
    return sLineCheck;
}

void cBgW::LineCheckGrpRp(cBgS_LinChk *pLine, int grpIdx) {
    cM3dGLin *pLin = &pLine->mLin;
    if (!mpGrp[grpIdx].mAab.Cross(pLin)) {
        return;
    }

    cBgD_Grp_t &grp = mpBgd->mGrpTbl[grpIdx];
    if (grp.mTreeIdx != 0xFFFF) {
        LineCheckRp(pLine, grp.mTreeIdx);
    }
    int childIdx = grp.mFirstChild;
    while (childIdx != 0xFFFF) {
        LineCheckGrpRp(pLine, childIdx);
        childIdx = mpBgd->mGrpTbl[childIdx].mNextSibling;
    }
}

void cBgW::LineCheckRp(cBgS_LinChk *pLine, int idx) {
    // Why is the ordering like this here lol
    cM3dGLin *pLin;
    cBgW_BlkElm *blk;
    u16 *id;

    cBgD_Tree_t &tree = mpBgd->mTreeTbl[idx];

    if (tree.mFlag & 1) {
        blk = &mpBlk[tree.mId[0]];
        if (blk->mWallIdx != 0xFFFF && !pLine->GetPreWallChk()) {
            RwgLineCheck(blk->mWallIdx, pLine);
        }
        if (blk->mGndIdx != 0xFFFF && !pLine->GetPreGroundChk()) {
            RwgLineCheck(blk->mGndIdx, pLine);
        }
        if (blk->mRoofIdx != 0xFFFF && !pLine->GetPreRoofChk()) {
            RwgLineCheck(blk->mRoofIdx, pLine);
        }
    } else {
        id = tree.mId;
        do {
            if (*id != 0xFFFF) {
                pLin = &pLine->mLin;
                if (mpNodeTree[*id].Cross(pLin)) {
                    LineCheckRp(pLine, *id);
                }
            }
            id++;
        } while (id < &tree.mId[8]);
    }
}

void cBgW::RwgLineCheck(int polyIdx, cBgS_LinChk *pLine) {
    // TODO
    do {
        cBgD_Tri_t *triTbl = mpBgd->mTriTbl;
        cBgD_Vtx_t *vtxTbl = mpVtxTbl;
        mVec3_c cross_pos;
        if (cM3d_Cross_LinTri(
                &pLine->mLin, &vtxTbl[triTbl[polyIdx].mVtxIdx0], &vtxTbl[triTbl[polyIdx].mVtxIdx1],
                &vtxTbl[triTbl[polyIdx].mVtxIdx2], &mpTri[polyIdx], &cross_pos, pLine->ChkFrontFlag(),
                pLine->ChkBackFlag()
            )) {
            dBgPc pc = *mpPolyCodes.GetDBgPc(mpBgd->mTriTbl[polyIdx].mId);
            if (!dBgWKCol::ChkPolyThrough(GetId(), &pc, pLine)
                // && fn_803599c0(/* params */)
            ) {
                pLine->mLin.mEnd = cross_pos;
                pLine->SetPolyIndex(polyIdx);
                sLineCheck = true;
            }
        }
        polyIdx = mpRwg[polyIdx].mNext;
    } while (polyIdx != 0xFFFF);
}

bool cBgW::GroundCross(cBgS_GndChk *pGnd) {
    sGndCheck = false;
    GroundCrossGrpRp(pGnd, mRootGroupIdx);
    return sGndCheck;
}

void cBgW::GroundCrossGrpRp(cBgS_GndChk *, int) {}

void cBgW::GroundCrossRp(cBgS_GndChk *, int) {}

void cBgW::RwgGroundCheckGnd(u16, cBgS_GndChk *) {}

void cBgW::RwgGroundCheckWall(u16, cBgS_GndChk *) {}

void cBgW::Lock() {}

void cBgW::CopyOldMtx() {}

void cBgW::Move() {}

void cBgW::ShdwDraw(cBgS_ShdwDraw *) {}

void cBgW::ShdwDrawGrpRp(cBgS_ShdwDraw *, int) {}

void cBgW::ShdwDrawRp(cBgS_ShdwDraw *, int) {}

void cBgW::RwgShdwDraw(int, cBgS_ShdwDraw *) {}

const cM3dGAab *cBgW::GetBnd() const {
    return &mpGrp[mRootGroupIdx].mAab;
}
bool cBgW::GetTriPnt(cBgS_PolyInfo const &info, mVec3_c *pA, mVec3_c *pB, mVec3_c *pC) const {
    cBgD_Tri_t &tri = mpBgd->mTriTbl[info.GetPolyIndex()];

    *pA = mpVtxTbl[tri.mVtxIdx0];
    *pB = mpVtxTbl[tri.mVtxIdx1];
    *pC = mpVtxTbl[tri.mVtxIdx2];

    return true;
}

void cBgW::GetTopUnder(f32 *pOutTop, f32 *pOutUnder) const {
    *pOutUnder = mpGrp[mRootGroupIdx].mAab.GetMinP()->y;
    *pOutTop = mpGrp[mRootGroupIdx].mAab.GetMaxP()->y;
}

void cBgW::GetTriPla(int idx, cM3dGPla *pPla) {
    *pPla = mpTri[idx];
}

void cBgW::GetTriPla(cBgS_PolyInfo const &info, cM3dGPla *pPla) const {
    *pPla = mpTri[info.GetPolyIndex()];
}

u32 cBgW::GetGrpInf(cBgS_PolyInfo const &info) const {
    return mpPolyCodes.getGrpCode(mpBgd->mTriTbl[info.GetPolyIndex()].mId);
}

s32 dBgW::GetGrpRoomIndex(cBgS_PolyInfo const &info) const {
    return mpPolyCodes.GetDBgPc(mpBgd->mTriTbl[info.GetPolyIndex()].mId)->getGrpRoomIdx();
}

s32 dBgW::GetExitId(cBgS_PolyInfo const &info) {
    return mpPolyCodes.GetDBgPc(mpBgd->mTriTbl[info.GetPolyIndex()].mId)->getExitId();
}

s32 dBgW::GetZTargetThrough(cBgS_PolyInfo const &info) {
    return mpPolyCodes.GetDBgPc(mpBgd->mTriTbl[info.GetPolyIndex()].mId)->getTargetThrough();
}

int dBgW::GetSpecialCode(cBgS_PolyInfo const &info) {
    return GetSpecialCode(info.GetPolyIndex());
}

int dBgW::GetSpecialCode(int polyIdx) {
    return mpPolyCodes.GetDBgPc(mpBgd->mTriTbl[polyIdx].mId)->getSpecialCode();
}

int dBgW::GetCode0_0x30000000(cBgS_PolyInfo const &info) {
    return mpPolyCodes.GetDBgPc(mpBgd->mTriTbl[info.GetPolyIndex()].mId)->getCode0_0x30000000();
}

int dBgW::GetCode0_0x80000000(cBgS_PolyInfo const &info) {
    return mpPolyCodes.GetDBgPc(mpBgd->mTriTbl[info.GetPolyIndex()].mId)->getCode0_0x80000000();
}

u32 dBgW::GetPolyObjThrough(int polyIdx) {
    return mpPolyCodes.GetDBgPc(mpBgd->mTriTbl[polyIdx].mId)->getObjThrough();
}
u32 dBgW::GetPolyCamThrough(int polyIdx) {
    return mpPolyCodes.GetDBgPc(mpBgd->mTriTbl[polyIdx].mId)->getCamThrough();
}
u32 dBgW::GetPolyLinkThrough(int polyIdx) {
    return mpPolyCodes.GetDBgPc(mpBgd->mTriTbl[polyIdx].mId)->getLinkThrough();
}
u32 dBgW::GetPolyArrowThrough(int polyIdx) {
    return mpPolyCodes.GetDBgPc(mpBgd->mTriTbl[polyIdx].mId)->getArrowThrough();
}
u32 dBgW::GetPolySlingshotThrough(int polyIdx) {
    return mpPolyCodes.GetDBgPc(mpBgd->mTriTbl[polyIdx].mId)->getSlingshotThrough();
}
u32 dBgW::GetPolyBeetleThrough(int polyIdx) {
    return mpPolyCodes.GetDBgPc(mpBgd->mTriTbl[polyIdx].mId)->getBeetleThrough();
}
u32 dBgW::GetPolyClawshotThrough(int polyIdx) {
    return mpPolyCodes.GetDBgPc(mpBgd->mTriTbl[polyIdx].mId)->getClawshotThrough();
}
u32 dBgW::GetPolyBombThrough(int polyIdx) {
    return mpPolyCodes.GetDBgPc(mpBgd->mTriTbl[polyIdx].mId)->getBombThrough();
}
u32 dBgW::GetPolyWhipThrough(int polyIdx) {
    return mpPolyCodes.GetDBgPc(mpBgd->mTriTbl[polyIdx].mId)->getWhipThrough();
}
u32 dBgW::GetUnderwaterRoofCode(int polyIdx) {
    return mpPolyCodes.GetDBgPc(mpBgd->mTriTbl[polyIdx].mId)->getUnderwaterRoof();
}
u32 dBgW::GetShdwThrough(int polyIdx) {
    return mpPolyCodes.GetDBgPc(mpBgd->mTriTbl[polyIdx].mId)->getShdwThrough();
}

int dBgW::GetLinkNo(cBgS_PolyInfo const &info) {
    return mpPolyCodes.GetDBgPc(mpBgd->mTriTbl[info.GetPolyIndex()].mId)->getLinkNo();
}

s32 dBgW::GetWallCode(cBgS_PolyInfo const &info) {
    return GetWallAtt(info.GetPolyIndex());
}

u32 dBgW::GetWallAtt(int polyIdx) {
    return mpPolyCodes.GetDBgPc(mpBgd->mTriTbl[polyIdx].mId)->getWallCode();
}

int dBgW::GetPolyAtt0Material(cBgS_PolyInfo const &info) {
    int att = mpPolyCodes.GetDBgPc(mpBgd->mTriTbl[info.GetPolyIndex()].mId)->getAtt0Code();
    if (att >= 0x20) {
        return att - 0x20;
    }
    return att;
}

int dBgW::GetPolyAtt1(cBgS_PolyInfo const &info) {
    return mpPolyCodes.GetDBgPc(mpBgd->mTriTbl[info.GetPolyIndex()].mId)->getAtt1Code();
}

int dBgW::GetGroundCode(cBgS_PolyInfo const &info) {
    return mpPolyCodes.GetDBgPc(mpBgd->mTriTbl[info.GetPolyIndex()].mId)->getGroundCode();
}

u32 dBgW::GetCode1_0x02000000(int polyIdx) {
    return mpPolyCodes.GetDBgPc(mpBgd->mTriTbl[polyIdx].mId)->getCode1_0x02000000();
}

u32 dBgW::GetCode1_0x04000000(int polyIdx) {
    return mpPolyCodes.GetDBgPc(mpBgd->mTriTbl[polyIdx].mId)->getCode1_0x04000000();
}

u32 dBgW::GetCode1_0x08000000(int polyIdx) {
    return mpPolyCodes.GetDBgPc(mpBgd->mTriTbl[polyIdx].mId)->getCode1_0x08000000();
}

u32 dBgW::GetLightingCode(cBgS_PolyInfo const &info) {
    return mpPolyCodes.GetDBgPc(mpBgd->mTriTbl[info.GetPolyIndex()].mId)->getLightingCode();
}

s32 dBgW::GetCamMoveBG(cBgS_PolyInfo const &info) {
    return mpPolyCodes.GetDBgPc(mpBgd->mTriTbl[info.GetPolyIndex()].mId)->getCamMoveBG();
}

s32 dBgW::GetRoomCamId(cBgS_PolyInfo const &info) {
    return mpPolyCodes.GetDBgPc(mpBgd->mTriTbl[info.GetPolyIndex()].mId)->getRoomCam();
}

s32 dBgW::GetRoomPathId(cBgS_PolyInfo const &info) {
    return mpPolyCodes.GetDBgPc(mpBgd->mTriTbl[info.GetPolyIndex()].mId)->getRoomPath();
}

s32 dBgW::GetRoomPathPntNo(cBgS_PolyInfo const &info) {
    return mpPolyCodes.GetDBgPc(mpBgd->mTriTbl[info.GetPolyIndex()].mId)->getRoomPathPnt();
}

bool cBgW::ChkNotReady() const {
    return mpVtxTbl == nullptr;
}

bool cBgW::ChkLock() const {
    return mFlags & LOCK_e;
}

bool cBgW::ChkMoveBg() const {
    return mFlags & MOVE_BG_e;
}

dBgW::dBgW() {
    mpCrrFunc = nullptr;
    mpRide_cb = nullptr;
    mpArrowStick_cb = nullptr;
    mpUnk_cb = nullptr;
    mFlags = 0;
}

dBgW::~dBgW() {}

void dBgW::Move() {
    OnMoveFlag();
    cBgW::Move();
}

void dBgW::positionWallCorrect(dBgS_Acch *pAcch, f32 p1, cM3dGPla &pla, mVec3_c *pUpperPos, f32 p2) {
    pAcch->SetWallHit();
    if (p2 - 1.0f < 0.0f) {
        return;
    }
    f32 dist = (p2 - 1.0f) * p1;
    pUpperPos->x += dist * pla.mNormal.x;
    pUpperPos->z += dist * pla.mNormal.z;
}
void dBgW::RwgWallCorrect(dBgS_Acch *, u16) {}

void dBgW::WallCorrectRp(dBgS_Acch *, int) {}

void dBgW::WallCorrectGrpRp(dBgS_Acch *, int) {}

bool dBgW::WallCorrect(dBgS_Acch *) {
    // TODO - only satisfy warning
    return false;
}

void dBgW::RwgWallCorrectSort(dBgS_Acch *, u16) {}

void dBgW::WallCorrectRpSort(dBgS_Acch *, int) {}

void dBgW::WallCorrectGrpRpSort(dBgS_Acch *, int) {}

bool dBgW::WallCorrectSort(dBgS_Acch *) {
    // TODO - only satisfy warning
    return false;
}

void dBgW::RwgRoofChk(u16, dBgS_RoofChk *, f32) {}

void dBgW::RwgRoofChkRoof(u16, dBgS_RoofChk *) {}

void dBgW::RwgRoofChkWall(u16, dBgS_RoofChk *) {}

void dBgW::RoofChkRp(dBgS_RoofChk *, int) {}

void dBgW::RoofChkGrpRp(dBgS_RoofChk *, int) {}

bool dBgW::RoofChk(dBgS_RoofChk *) {
    // TODO - only satisfy warning
    return false;
}

void dBgW::RwgSplGrpChk(u16, dBgS_SplGrpChk *) {}

void dBgW::SplGrpChkRp(dBgS_SplGrpChk *, int) {}

void dBgW::SplGrpChkGrpRp(dBgS_SplGrpChk *, int) {}

bool dBgW::SplGrpChk(dBgS_SplGrpChk *) {
    // TODO - only satisfy warning
    return false;
}

void dBgW::RwgCaptPoly(u16, dBgS_CaptPoly &) {}

void dBgW::CaptPolyRp(dBgS_CaptPoly &, int) {}

void dBgW::CaptPolyGrpRp(dBgS_CaptPoly &, int) {}

void dBgW::CaptPoly(dBgS_CaptPoly &) {}

void dBgW::RwgSphChk(u16, dBgS_SphChk *, void *) {}

void dBgW::SphChkRp(dBgS_SphChk *, void *, int) {}

void dBgW::SphChkGrpRp(dBgS_SphChk *, void *, int) {}

bool dBgW::SphChk(dBgS_SphChk *pSph, void *pV) {
    sSphCheck = false;
    SphChkRp(pSph, pV, mRootGroupIdx);
    return sSphCheck;
}

void dBgW::CrrPos(cBgS_PolyInfo const &info, void *pObj, bool b, mVec3_c *pPos, mAng3_c *pAngle, mAng3_c *pShapeAngle) {
    if (mpCrrFunc && static_cast<dAcObjBase_c *>(pObj)->baseProperties & dBase_c::BASE_PROP_0x4) {
        mpCrrFunc(this, pObj, info, b, pPos, pAngle, pShapeAngle);
    }
}

void dBgW::TransPos(
    cBgS_PolyInfo const &info, void *pObj, bool b, mVec3_c *pPos, mAng3_c *pAngle, mAng3_c *pShapeAngle
) {
    if (mpCrrFunc && static_cast<dAcObjBase_c *>(pObj)->baseProperties & dBase_c::BASE_PROP_0x4) {
        mpCrrFunc(this, pObj, info, b, pPos, pAngle, pShapeAngle);
    }
}

void dBgW::MatrixCrrPos(
    cBgS_PolyInfo const &info, void *pObj, bool b, mVec3_c *pPos, mAng3_c *pAngle, mAng3_c *pShapeAngle
) {
    CrrPos(info, pObj, b, pPos, pAngle, pShapeAngle);
}

void dBgW::CallRideCallback(dAcObjBase_c *p1, dAcObjBase_c *p2) {
    if (mpRide_cb) {
        mpRide_cb(this, p1, p2);
    }
}

void dBgW::CallArrowStickCallback(dAcObjBase_c *p1, dAcObjBase_c *p2, mVec3_c &p3) {
    if (mpArrowStick_cb) {
        mpArrowStick_cb(this, p1, p2, p3);
    }
}

bool dBgW::CallUnkCallback(dAcObjBase_c *p1, dAcObjBase_c *p2) {
    if (mpUnk_cb) {
        return mpUnk_cb(this, p1, p2);
    }
    return false;
}

void dBgW::OffMoveFlag() {
    mFlags &= ~MOVE_BG_e;
}

u32 dBgW::ChkMoveFlag() const {
    return mFlags & MOVE_BG_e;
}

void dBgW::vt_0x3C() {
    cM3dGAab bnd = mpGrp[mRootGroupIdx].mAab;
    GetPartition().fn_803391f0(&bnd);
}

bool dBgW::GetMapCode(int polyIdx, int *pOut) {
    cBgS_PolyInfo info;
    info.SetActorInfo(BG_ID_MAX, nullptr, 0);
    info.SetPolyIndex(polyIdx);

    int att0 = GetPolyAtt0(info);
    int att1 = GetPolyAtt1(info);
    bool target = GetZTargetThrough(info);
    if (att0 >= 0x20) {
        *pOut = dBgS::GetMapCode(att0 - 0x20, att1, target);
        if (Chk0x24_0x20() && ScGame::currentSpawnInfo.trial == 1) {
            *pOut = 0x1E;
        }
        return false;
    } else {
        return true;
    }
}

bool dBgW::UpdateDraw(mAllocator_c *alloc) {
    for (int i = 0; i < 0x1F; ++i) {
        mMapRelated[i].mCount = 0;
    }
    for (int i = 0; i < mpBgd->mTriNum; i++) {
        int idx = 0;
        if (!GetMapCode(i, &idx)) {
            mMapRelated[idx].mCount++;
        }
    }
    for (int i = 0; i < 0x1F; ++i) {
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
    for (int i = 0; i < mpBgd->mTriNum; i++) {
        int idx = 0;
        if (!GetMapCode(i, &idx)) {
            mMapRelated[idx].mpIdx[local_cnt[idx]] = i;
            local_cnt[idx]++;
        }
    }
    return true;
}

int dBgW::GetPolyAtt0(const cBgS_PolyInfo &info) {
    return mpPolyCodes.GetDBgPc(mpBgd->mTriTbl[info.GetPolyIndex()].mId)->getAtt0Code();
}

bool dBgW::GetIsDraw(int idx) {
    return mMapRelated[idx].mCount > 0;
}

void dBgW::DrawOnMap(int idx, bool) {}
